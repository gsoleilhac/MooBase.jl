/**
 * \file phase_2.tpp
 * \brief Implementation of the kp::phase_2 class.
 * \author Julien Jorge
 */
#include <kp/ranking.hpp>
#include <kp/view_order.hpp>
#include <kp/subset_view.hpp>

#include <bikp/solver/bi_two_phases.hpp>
#include <bikp/reduction/simple_variable_fixing.hpp>

#include <cmath>

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
kp::phase_2<View>::phase_2
( const view_type& inst, const solution_list& XSEM, const constraint_set& hull )
  : m_kp(inst), m_XSEM(XSEM), m_hull(hull)
{

} // phase_2::phase_2()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
const kp::maximum_set& kp::phase_2<View>::get_XNSEM() const
{
  return m_XNSEM;
} // phase_2::get_XNSEM()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::run()
{
  typename solution_list::const_iterator it_s;

  std::list<constraint_type> H(m_hull.begin(), m_hull.end());

  cost_vector nadir( compute_nadir() );

  bound_set E(nadir);
  build_bound_set(E);

  unsigned int nb_it = m_hull.size();
  unsigned int i=1;
  
  typename std::map<moo::real_type, constraint_set>::const_iterator it_v;
  typename constraint_set::const_iterator it_c;

  while (!E.empty())
    {
      constraint_type h;
      cost_vector_list U;
      choose_weight_and_points(H, E, h, U);

      std::cout << "ranking " << i << "/" << nb_it << ", "
                << m_XNSEM.size() << " solutions" << std::endl;
      ++i;

      bounded_region region( h.get_coefficient(), nadir );

      region.insert_nadirs(U.begin(), U.end());

      /*if ( region.bound_set_size() != U.size() )
        std::cout << "phase_2<View>::run(): "
                  << region.bound_set_size() << " != " << U.size()
                  << std::endl;*/

      pareto_set out_profits;
      reduce_and_solve( region, out_profits );

      H.erase( std::find(H.begin(), H.end(), h) );

      for ( ; !U.empty(); U.pop_front() )
        E.erase( U.front() );

      for ( pareto_set::const_iterator it=out_profits.begin();
            it!=out_profits.end(); ++it )
        E.reduce(*it);
    }
} // phase_2::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::solve
( const bounded_region& region, pareto_set& out_profits )
{
  solution_list sols;

  bounded_region r(region);
  view_order<view_type> mono_p( m_kp );
  mono_p.decreasing_efficiency( region.get_direction() );

#if 0
  std::cout << "  lambda=(" << region.get_direction()[0] << ", "
            << region.get_direction()[1] << ", "
            << region.get_direction()[2] << ')' << '\n';
  std::cout << "  nadir=(" << region.get_nadir()[0] << ", "
            << region.get_nadir()[1] << ", "
            << region.get_nadir()[2] << ')' << '\n';
  std::cout << "  bound set size: " << region.bound_set_size() << std::endl;
#endif

  ranking< view_order<view_type> > solver( mono_p, r, sols, out_profits );

  m_XNSEM.insert( sols.begin(), sols.end() );
} // phase_2::solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::reduce_and_solve
( const bounded_region& region, pareto_set& out_profits )
{
  mono_problem mono_p(m_kp.get_size(), m_kp.get_capacity());

  for (std::size_t i=0; i!=m_kp.get_size(); ++i)
    {
      const variable var(m_kp.get_variable(i));
      mono_p.set_variable
        (i, var.cost.scalar_product(region.get_direction()), var.weight);
    }
  mono_p.sort_by_decreasing_efficiency();
  simple_variable_fixing red(mono_p, region.get_lower_bound());

  std::cout << "Fixing " << red.set_variables().size()
            << " variables to one, " << red.unset_variables().size()
            << " to zero ("
            << ( m_kp.get_size() - red.set_variables().size()
                 - red.unset_variables().size() ) << '/' << m_kp.get_size()
            << " remaining)." << std::endl;

  if ( red.set_variables().size() + red.unset_variables().size() == 0 )
    solve(region, out_profits);
  else
    {
      // get the guaranteed profit and compute the remaining capacity
      std::vector<std::size_t> keep, fix_0, fix_1;
      moo::real_type cap( m_kp.get_capacity() );
      cost_vector guaranteed( m_kp.get_nb_objectives(), 0 );

      for (unsigned int i=0; i!=m_kp.get_size(); ++i)
        if ( red.set_variables().find(i) != red.set_variables().end() )
          {
            guaranteed += m_kp.get_variable( mono_p.real_index(i) ).cost;
            cap -= m_kp.get_variable( mono_p.real_index(i) ).weight;
            fix_1.push_back( mono_p.real_index(i) );
          }
        else if ( red.unset_variables().find(i) == red.unset_variables().end() )
          keep.push_back( mono_p.real_index(i) );
        else
          fix_0.push_back( mono_p.real_index(i) );

      // build the sub problem and sort the items
      subset_view<view_type> red_p( m_kp, keep, cap );
      view_order< subset_view<view_type> > sub_p( red_p );
      sub_p.decreasing_efficiency( region.get_direction() );

      // scale the region
      bounded_region sub_region
        ( region.get_direction(), region.get_nadir() - guaranteed );
      std::list<cost_vector> nadir;

      for ( bound_set::const_iterator it=region.bound_set_begin();
            it!=region.bound_set_end(); ++it )
        {
          assert( (*it)[0] >= guaranteed[0] );
          assert( (*it)[1] >= guaranteed[1] );
          assert( (*it)[2] >= guaranteed[2] );

          nadir.push_back(*it - guaranteed);
        }

      sub_region.insert_nadirs(nadir.begin(), nadir.end());

      // solve the sub problem
      solution_list sub_sols;
      pareto_set out_sub_sols;
      
      ranking< view_order< subset_view<view_type> > > solver
        ( sub_p, sub_region, sub_sols, out_sub_sols );

      for ( pareto_set::const_iterator it=out_sub_sols.begin();
            it!=out_sub_sols.end(); ++it )
        out_profits.insert( *it + guaranteed );

      // restore the solutions
      typename solution_list::const_iterator it_s;
      knapsack_solution base_sol(m_kp);

      for (std::size_t i=0; i!=fix_0.size(); ++i)
        base_sol.unset(m_kp.get_variable(fix_0[i]));

      for (std::size_t i=0; i!=fix_1.size(); ++i)
        base_sol.set(m_kp.get_variable(fix_1[i]));

      for ( it_s=sub_sols.begin(); it_s!=sub_sols.end(); ++it_s )
        if ( !m_XNSEM.is_dominated( it_s->image() + guaranteed ) )
          {
            knapsack_solution sol(base_sol);

            for (std::size_t i=0; i!=sub_p.get_size(); ++i)
              if ( it_s->status_of( sub_p.get_variable(i) ) == moo::bin_set )
                sol.set( red_p.get_source_variable( sub_p.get_variable(i) ) );
              else
                sol.unset( red_p.get_source_variable( sub_p.get_variable(i) ) );

            m_XNSEM.insert(sol);
          }
    }
} // phase_2::reduce_and_solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::choose_weight_and_points
( const std::list<constraint_type>& H, const bound_set& E,
  constraint_type& h, cost_vector_list& U) const
{
  U.clear();

  typename bound_set::const_iterator it_n;
  std::map<constraint_type, cost_vector_list> U_c;

  for ( it_n=E.begin(); it_n!=E.end(); ++it_n )
    {
      typename std::list<constraint_type>::const_iterator it_c;
      moo::real_type min_dist = std::numeric_limits<moo::real_type>::infinity();
      constraint_type h_prime;

      for ( it_c=H.begin(); it_c!=H.end(); ++it_c )
        {
          const moo::real_type d( dist(*it_n, *it_c) );

          if ( d < min_dist )
            {
              h_prime = *it_c;
              min_dist = d;
            }
        }

      U_c[h_prime].push_front(*it_n);
    }

  typename std::map<constraint_type, cost_vector_list>::const_iterator it_h;
  moo::real_type min_dist = std::numeric_limits<moo::real_type>::infinity();

  for ( it_h=U_c.begin(); it_h!=U_c.end(); ++it_h )
    {
      moo::real_type max_dist = 0;
      typename cost_vector_list::const_iterator it_s;

      for ( it_s=it_h->second.begin(); it_s!=it_h->second.end(); ++it_s )
        max_dist = std::max( max_dist, dist(*it_s, it_h->first) );

      if (max_dist < min_dist)
        {
          min_dist = max_dist;
          h = it_h->first;
          U = it_h->second;
        }
    }
} // phase_2::choose_weight_and_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::compute_distances
( std::map<moo::real_type, constraint_set>& val,
  std::map<constraint_type, cost_vector_set>& U,
  const cost_vector& nadir ) const
{
  bound_set bs(nadir);

  build_bound_set(bs);

  typename bound_set::const_iterator it_n;

  for ( it_n=bs.begin(); it_n!=bs.end(); ++it_n )
    {
      typename constraint_set::const_iterator it_c;
      moo::real_type min_dist = std::numeric_limits<moo::real_type>::infinity();
      std::list<constraint_type> h_prime;

      for ( it_c=m_hull.begin(); it_c!=m_hull.end(); ++it_c )
        {
          const moo::real_type d( dist(*it_n, *it_c) );

          if ( d < min_dist )
            {
              h_prime.clear();
              h_prime.push_back(*it_c);
              min_dist = d;
            }
          else if ( d==min_dist )
            h_prime.push_back(*it_c);
        }

      for ( ; !h_prime.empty(); h_prime.pop_front() )
        U[h_prime.front()].insert(*it_n);
    }

  typename std::map<constraint_type, cost_vector_set>::const_iterator it_h;

  for ( it_h=U.begin(); it_h!=U.end(); ++it_h )
    {
      moo::real_type max_dist = 0;
      typename cost_vector_set::const_iterator it_s;

      for ( it_s=it_h->second.begin(); it_s!=it_h->second.end(); ++it_s )
        max_dist = std::max( max_dist, dist(*it_s, it_h->first) );

      val[max_dist].insert(it_h->first);
    }
} // phase_2::compute_distances()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::compute_distances_2
( std::map<moo::real_type, constraint_set>& val,
  std::map<constraint_type, cost_vector_set>& U,
  const cost_vector& nadir ) const
{
  bound_set bs(nadir);

  build_bound_set(bs);

  cost_vector max_cost(nadir);
  for ( std::size_t i=0; i!=max_cost.size(); ++i )
    max_cost[i] = std::numeric_limits<cost_vector::value_type>::max();

  typename bound_set::const_iterator it_n;
  typename constraint_set::const_iterator it_c;
  typename solution_list::const_iterator it_s;

  std::map<constraint_type, cost_vector> local_nadir;
  cost_compare comp;

  for ( it_c=m_hull.begin(); it_c!=m_hull.end(); ++it_c )
    {
      local_nadir[*it_c] = max_cost;

      for ( it_s=m_XSEM.begin(); it_s!=m_XSEM.end(); ++it_s )
        {
          const cost_vector y(it_s->image());

          if ( it_c->get_coefficient().scalar_product(y)
               == it_c->get_right_hand_side() )
            for ( it_n=bs.begin(); it_n!=bs.end(); ++it_n )
              if ( comp(y, *it_n) & moo::general_dominance )
                for ( std::size_t i=0; i!=y.size(); ++i )
                  local_nadir[*it_c][i] =
                    std::min( local_nadir[*it_c][i], (*it_n)[i] );
        }
    }

  typename std::map<constraint_type, cost_vector>::const_iterator it_l;

  for (it_l=local_nadir.begin(); it_l!=local_nadir.end(); ++it_l )
    {
      typename std::map<constraint_type, cost_vector>::const_iterator it;
      bool do_it(true);

      for (it=local_nadir.begin(); do_it && (it!=local_nadir.end()); ++it )
        if (it!=it_l)
          if ( comp(it_l->second, it->second) & moo::general_dominance )
            do_it = false;

      if ( do_it )
        for ( it_n=bs.begin(); it_n!=bs.end(); ++it_n )
          if ( comp(*it_n, it_l->second) & moo::general_dominance )
            U[it_l->first].insert(*it_n);
    }

  typename std::map<constraint_type, cost_vector_set>::const_iterator it_h;

  for ( it_h=U.begin(); it_h!=U.end(); ++it_h )
    {
      moo::real_type max_dist(0);
      typename cost_vector_set::const_iterator it_s;

      for ( it_s=it_h->second.begin(); it_s!=it_h->second.end(); ++it_s )
        max_dist = std::max( max_dist, dist(*it_s, it_h->first) );

      val[max_dist].insert(it_h->first);
    }
} // phase_2::compute_distances_2()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
moo::real_type
kp::phase_2<View>::dist( const cost_vector& u, const constraint_type& h ) const
{
  const cost_vector coeff = h.get_coefficient();

  return ( h.get_right_hand_side() - u.scalar_product(coeff) )
    / std::sqrt( (double)coeff.scalar_product(coeff) );
  //return h.get_right_hand_side() - u.scalar_product(coeff);
} // phase_2::dist()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
void kp::phase_2<View>::build_bound_set( bound_set& bs ) const
{
  // calculer les nadirs
  typename solution_list::const_iterator it;

  for ( it=m_XSEM.begin(); it!=m_XSEM.end(); ++it )
    bs.reduce(it->image());

  for ( it=m_XNSEM.begin(); it!=m_XNSEM.end(); ++it )
    bs.reduce(it->image());
} // phase_2::build_bound_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 */
template<typename View>
kp::cost_vector kp::phase_2<View>::compute_nadir()
{
  assert( m_kp.get_nb_objectives() == 3 );

  solve_bi( 0, 1 );
  solve_bi( 0, 2 );
  solve_bi( 1, 2 );

  cost_vector result
    ( m_kp.get_nb_objectives(), std::numeric_limits<moo::real_type>::max() );
  typename solution_list::const_iterator it;

  for ( it=m_XNSEM.begin(); it!=m_XNSEM.end(); ++it )
    for ( unsigned int i=0; i!=m_kp.get_nb_objectives(); ++i )
      if ( it->image()[i] < result[i] )
        result[i] = it->image()[i];

  std::cout << "nadir: " << result << std::endl;
  return result;
} // phase_2::compute_nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief
 * \param a The first objective to keep.
 * \param b The second objective to keep.
 */
template<typename View>
void kp::phase_2<View>::solve_bi( unsigned int a, unsigned int b )
{
  bi_problem bi( m_kp.get_size(), m_kp.get_capacity() );

  for (unsigned int i=0; i!=m_kp.get_size(); ++i)
    bi.set_variable
      ( i,
        bi_profit( m_kp.get_variable(i).cost[a], m_kp.get_variable(i).cost[b] ),
        m_kp.get_variable(i).weight );

  bi_two_phases solver;

  std::list<bi_solution> sols;
  maximum_set real_sols;

  solver.run( bi, sols );

  std::list<bi_solution>::const_iterator it;

  for ( it=sols.begin(); it!=sols.end(); ++it )
    {
      knapsack_solution sol( m_kp );

      for (unsigned int i=0; i!=bi.size(); ++i)
        if ( it->is_set(i) )
          sol.set( m_kp.get_variable(i) );
        else
          sol.unset( m_kp.get_variable(i) );

      m_XNSEM.insert( sol );
    }
} // phase_2::compute_nadir()
