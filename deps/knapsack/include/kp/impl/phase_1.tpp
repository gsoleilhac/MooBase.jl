/**
 * \file phase_1.cpp
 * \brief Implementation of the kp::phase_1 class.
 * \author Julien Jorge
 */
#include <kp/lexicographic_solver.hpp>
#include <kp/bounded_region.hpp>
#include <kp/dynamic_programming_multiple.hpp>
#include <kp/view_order.hpp>
#include <kp/subset_view.hpp>
#include <bikp/phase/dp_first_phase.hpp>
#include <bikp/reduction/simple_variable_fixing.hpp>

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param inst The instance to solve.
 */
template<typename View>
kp::phase_1<View>::phase_1( const view_type& inst )
  : m_kp(inst)
{

} // phase_1::phase_1()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum set of supported solutions.
 */
template<typename View>
const std::list<kp::knapsack_solution>& kp::phase_1<View>::get_XSEm() const
{
  return m_XSEm;
} // phase_1::get_XSEm()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum set of supported solutions.
 */
template<typename View>
const std::list<kp::knapsack_solution>& kp::phase_1<View>::get_XSEM() const
{
  return m_XSEM;
} // phase_1::get_XSEM()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the hull of supported solutions.
 */
template<typename View>
const std::set< typename kp::phase_1<View>::constraint_type >&
kp::phase_1<View>::get_hull() const
{
  return m_hull;
} // phase_1::get_hull()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the coordinates of supported efficient solutions.
 */
template<typename View>
void kp::phase_1<View>::run()
{
  m_XSEm.clear();
  m_XSEM.clear();
  m_adjacent.clear();
  m_potentially_adjacent.clear();

  lexicographic_optimal();

  std::list<knapsack_solution>::const_iterator it;

  for (it=m_XSEm.begin(); it!=m_XSEm.end(); ++it)
    {
      const cost_vector y(it->image());
      weight_set_type w_p(y);

      compute_polytope(w_p);

      while ( !m_potentially_adjacent[y].empty() && !w_p.empty() )
        {
          cost_vector neighbor( m_potentially_adjacent[y].front() );
          m_potentially_adjacent[y].pop_front();

          assert( std::find(m_potentially_adjacent[y].begin(),
                            m_potentially_adjacent[y].end(),
                            neighbor)
                  == m_potentially_adjacent[y].end() );

          std::list<knapsack_solution> X;
          neighbor_type f( choose_facet(w_p, neighbor) );
          cost_vector lambda_1(3, 0);
          cost_vector lambda_2(3, 0);

          lambda_1[0] = f.second.p[0][0];
          lambda_1[1] = f.second.p[0][1];
          lambda_1[2] = 1.0 - lambda_1[1] - lambda_1[0];

          lambda_2[0] = f.second.p[1][0];
          lambda_2[1] = f.second.p[1][1];
          lambda_2[2] = 1.0 - lambda_2[1] - lambda_2[0];

          bicriteria_solver(lambda_1, lambda_2, X);
          update_adjacency(lambda_1, lambda_2, X, y, f);
          update_polytope(X, w_p);

          if ( !w_p.empty() )
            rebuild_potentially_adjacents(w_p);
        }

      if ( !w_p.empty() )
        {
          weight_set_type::const_facet_iterator itf;

          for ( itf=w_p.facets_begin(); itf!=w_p.facets_end(); ++itf )
            if ( itf->first != NULL )
              add_constraint_to_hull(*itf);
        }
    }

  find_equivalent_solutions();
} // phase_1::run()

/*----------------------------------------------------------------------------*/
template<typename View>
typename kp::phase_1<View>::neighbor_type
kp::phase_1<View>::choose_facet
( const weight_set_type& w_p, const cost_vector& neighbor ) const
{
  neighbor_type result;

  weight_set_type::const_facet_iterator it;
  bool found = false;

  for ( it=w_p.facets_begin(); !found && it!=w_p.facets_end(); ++it )
    if ( it->first != NULL )
      if ( *it->first == neighbor )
        {
          found = true;
          result = *it;
        }

  assert( found == true );

  return result;
} // phase_1::choose_facet()

/*----------------------------------------------------------------------------*/
template<typename View>
void kp::phase_1<View>::compute_polytope( weight_set_type& w_p )
{
  std::list<cost_vector>::const_iterator it;

  for ( it=m_adjacent[w_p.get_point()].begin();
        it!=m_adjacent[w_p.get_point()].end(); ++it )
    w_p.add_constraint( *it );

  for ( it=m_potentially_adjacent[w_p.get_point()].begin();
        it!=m_potentially_adjacent[w_p.get_point()].end(); ++it )
    w_p.add_constraint( *it );

  // Some potentially adjacent points may define not proper facets. So we must
  // update the list.

  rebuild_potentially_adjacents(w_p);
} // phase_1::compute_polytope()

/*----------------------------------------------------------------------------*/
template<typename View>
void kp::phase_1<View>::update_polytope
( const std::list<kp::knapsack_solution>& X, weight_set_type& w_p )
{
  std::list<knapsack_solution>::const_iterator it;

  for ( it=X.begin(); it!=X.end(); ++it )
    w_p.add_constraint( it->image() );
} // phase_1::update_polytope()

/*----------------------------------------------------------------------------*/
template<typename View>
void kp::phase_1<View>::rebuild_potentially_adjacents( weight_set_type& w_p )
{
  const cost_vector& y = w_p.get_point();

  m_potentially_adjacent[y].clear();

  weight_set_type::const_facet_iterator it;

  for ( it=w_p.facets_begin(); it!=w_p.facets_end(); ++it )
    if ( it->first != NULL )
      {
        cost_vector adj(*it->first);

        if ( std::find( m_adjacent[y].begin(), m_adjacent[y].end(), adj )
             == m_adjacent[y].end() )
          m_potentially_adjacent[y].push_front(adj);
      }
} // phase_1::rebuild_potentially_adjacents()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the coordinates of the lexicographic optimal solutions.
 */
template<typename View>
void kp::phase_1<View>::lexicographic_optimal()
{
  lexicographic_solver<view_type> solver;
  std::list<knapsack_solution> X;

  solver.solve(m_kp, X);

  std::list<knapsack_solution>::const_iterator it;

  for ( it=X.begin(); it!=X.end(); ++it )
    {
      m_XSEm.push_back(*it);
      m_adjacent[it->image()].clear();

      for ( adjacency_type::iterator ita=m_potentially_adjacent.begin();
            ita!=m_potentially_adjacent.end(); ++ita )
        ita->second.push_back(it->image());

      m_potentially_adjacent[it->image()].clear();
    }
} // phase_1::lexicographic_optimal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Solve a bi-objective problem.
 * \param lambda_1 The coefficients of the combination of the first objective.
 * \param lambda_2 The coefficients of the combination of the second objective.
 * \param X (out) The solutions, for the not combined problem.
 */
template<typename View>
void kp::phase_1<View>::bicriteria_solver
( const cost_vector& lambda_1, const cost_vector& lambda_2,
  std::list<kp::knapsack_solution>& X ) const
{
  const unsigned int n = m_kp.get_size();
  std::vector<moo::real_type> c1(n);
  std::vector<moo::real_type> c2(n);
  std::vector<moo::real_type> w(n);

  for (unsigned int i=0; i!=n; ++i )
    {
      const variable& v = m_kp.get_variable(i);
      c1[i] = lambda_1.scalar_product( v.cost );
      c2[i] = lambda_2.scalar_product( v.cost );
      w[i] = v.weight;
    }

  bi_problem bikp( c1, c2, w, m_kp.get_capacity() );
  dp_first_phase phase;

  phase.run(bikp);

  std::vector<bi_problem::solution_type> solutions
    ( phase.get_extreme_solutions().begin(),
      phase.get_extreme_solutions().end() );
  std::vector<bi_problem::solution_type>::const_iterator it;

  // lexicographic sort
  std::sort( solutions.begin(), solutions.end() );

  for ( it=solutions.begin(); it!=solutions.end(); ++it )
    {
      moo::binary_solution sol(n);

      for (unsigned int i=0; i!=n; ++i)
        if ( it->is_set(i) )
          sol[m_kp.get_variable(i).index] = moo::bin_set;
        else
          sol[m_kp.get_variable(i).index] = moo::bin_unset;

      X.push_front( knapsack_solution( m_kp, sol ) );
    }
} // phase_1::bicriteria_solver()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the lists of (potentially) adjacent points for a given point.
 * \param lambda_1 The coefficients of the combination of the first objective.
 * \param lambda_2 The coefficients of the combination of the second objective.
 * \param X The solutions, for the not combined problem.
 * \param y The point for which we update the lists
 */
template<typename View>
void kp::phase_1<View>::update_adjacency
( const cost_vector& lambda_1, const cost_vector& lambda_2,
  const std::list<knapsack_solution>& X, const cost_vector& y,
  const neighbor_type& f )
{
  update_border_adjacency( lambda_1, X.front().image(), y, *f.first );
  update_border_adjacency( lambda_2, X.back().image(), y, *f.first );

  std::list<knapsack_solution>::const_iterator it;
  std::list<cost_vector> new_points;
  cost_vector bi_y(2, 0);

  bi_y[0] = lambda_1.scalar_product(y);
  bi_y[1] = lambda_2.scalar_product(y);

  for ( it=X.begin(); it!=X.end(); ++it )
    {
      cost_vector new_y( it->image() );
      cost_vector bi_new_y(2, 0);

      bi_new_y[0] = lambda_1.scalar_product(new_y);
      bi_new_y[1] = lambda_2.scalar_product(new_y);

      if ( bi_y == bi_new_y )
        {
          m_adjacent[*f.first].push_back(y);
          m_adjacent[y].push_back(*f.first);

          if ( !new_points.empty() )
            {
              update_local_adjacency(new_points);
              new_points.clear();
            }
        }
      else if ( m_adjacent.find(new_y) == m_adjacent.end() )
        {
          m_adjacent[new_y].clear();
          m_potentially_adjacent[new_y].clear();

          m_potentially_adjacent[y].push_back(new_y);
          m_potentially_adjacent[*f.first].push_back(new_y);

          m_XSEm.push_back(*it);
          new_points.push_back(new_y);
        }
    }

  if ( !new_points.empty() )
    update_local_adjacency(new_points);
} // phase_1::update_adjacency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the adjacency between a point on the border of a facet and the
 *        known points.
 * \param lambda The weights applied to the points.
 * \param p The point that can be adjacent to existing points.
 * \param y The point for which we are building the polytope. To not take into
 *        account for adjacency.
 * \param neighbor The point for that makes the explored facet. To not take into
 *        account for adjacency.
 */
template<typename View>
void kp::phase_1<View>::update_border_adjacency
( const cost_vector& lambda, const cost_vector& p,
  const cost_vector& y, const cost_vector& neighbor )
{
  std::list<knapsack_solution>::const_iterator it;

  const moo::real_type r_p = lambda.scalar_product(p);

  for ( it=m_XSEm.begin(); it!=m_XSEm.end(); ++it )
    if ( (it->image() != y) && (it->image() != neighbor) )
      if ( lambda.scalar_product(it->image()) == r_p )
        {
          m_potentially_adjacent[it->image()].push_back(p);
          m_potentially_adjacent[neighbor].push_back(it->image());
        }
} // phase_1::update_border_adjacency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the adjacency between the new points found in a facet.
 * \param Z The new points found in th facet.
 */
template<typename View>
void
kp::phase_1<View>::update_local_adjacency( const std::list<cost_vector>& Z )
{
  std::list<cost_vector>::const_iterator it(Z.begin());
  std::list<cost_vector>::const_iterator prev(it);

  for ( ++it; it!=Z.end(); ++it, ++prev )
    m_potentially_adjacent[*prev].push_back(*it);
} // phase_1::update_local_adjacency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint to the description of the hull.
 * \param f The facet to add.
 */
template<typename View>
void kp::phase_1<View>::add_constraint_to_hull( const neighbor_type& f )
{
  assert( f.first != NULL );

  facet_type::vector_type lambda_0( m_kp.get_nb_objectives() );
  facet_type::vector_type lambda_1( m_kp.get_nb_objectives() );

  bool lambda_0_in_border = make_lambda( f.second.p[0], lambda_0 );
  bool lambda_1_in_border = make_lambda( f.second.p[1], lambda_1 );

  if ( lambda_0_in_border )
    {
      if ( lambda_1_in_border )
        {
          facet_type::vector_type w = (lambda_0 + lambda_1) / 2;
          m_hull.insert( constraint_type(w, constraint_type::less_equal,
                                         w.scalar_product(*f.first)) );
        }
      else
        m_hull.insert( constraint_type(lambda_1, constraint_type::less_equal,
                                       lambda_1.scalar_product(*f.first)) );
    }
  else if ( lambda_1_in_border )
    m_hull.insert( constraint_type(lambda_0, constraint_type::less_equal,
                                   lambda_0.scalar_product(*f.first)) );
  else
    {
      m_hull.insert( constraint_type(lambda_0, constraint_type::less_equal,
                                     lambda_0.scalar_product(*f.first)) );
      m_hull.insert( constraint_type(lambda_1, constraint_type::less_equal,
                                     lambda_1.scalar_product(*f.first)) );
    }
} // phase_1::add_constraint_to_hull()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coefficients of a facet.
 * \param w The weight of one of the bounds of the facet.
 * \param lambda (out) The coefficients. Equal ( w, 1 - sum_i(w) ).
 * \return true if the resulting coefficients are on the border of the weight
 *         set.
 */
template<typename View>
bool kp::phase_1<View>::make_lambda
( const facet_type::vector_type& w, facet_type::vector_type& lambda ) const
{
  assert( w.size() + 1 == lambda.size() );

  bool result = false;
  const unsigned int s = w.size();
  lambda[s] = 1;

  for (unsigned int i=0; i!=s; ++i)
    {
      if ( w[i] == 0 )
        result = true;

      lambda[i] = w[i];
      lambda[s] -= w[i];
    }

  return result || (lambda[s] == 0);
} // phase_1::make_lambda()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coefficients of a facet.
 * \param w The weight of one of the bounds of the facet.
 * \param lambda (out) The coefficients. Equal ( w, 1 - sum_i(w) ).
 * \return true if the resulting coefficients are on the border of the weight
 *         set.
 */
template<typename View>
void kp::phase_1<View>::find_equivalent_solutions()
{
  std::set<knapsack_solution> result( m_XSEm.begin(), m_XSEm.end() );
  typename std::set<constraint_type>::const_iterator it;

  for (it=m_hull.begin(); it!=m_hull.end(); ++it)
    {
      mono_problem mono_p(m_kp.get_size(), m_kp.get_capacity());

      for (std::size_t i=0; i!=m_kp.get_size(); ++i)
        {
          const variable var(m_kp.get_variable(i));
          mono_p.set_variable
            (i, var.cost.scalar_product(it->get_coefficient()), var.weight);
        }
      mono_p.sort_by_decreasing_efficiency();
      simple_variable_fixing red(mono_p, it->get_right_hand_side());

      std::cout << "Fixing " << red.set_variables().size()
                << " variables to one, " << red.unset_variables().size()
                << " to zero ("
                << ( m_kp.get_size() - red.set_variables().size()
                     - red.unset_variables().size() ) << '/' << m_kp.get_size()
                << " remaining)." << std::endl;

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

      // scale the region
      bounded_region region
        ( it->get_coefficient(),
          it->get_right_hand_side()
          - guaranteed.scalar_product(it->get_coefficient()) );

      sub_p.decreasing_efficiency( region.get_direction() );
      dynamic_programming_multiple< view_order< subset_view<view_type> > >
        solver( sub_p, region );

      std::list<knapsack_solution> sub_sols;
      solver.run( sub_sols );

      // restore the solutions
      typename std::list<knapsack_solution>::const_iterator it_s;
      knapsack_solution base_sol(m_kp);

      for (std::size_t i=0; i!=fix_0.size(); ++i)
        base_sol.unset(m_kp.get_variable(fix_0[i]));

      for (std::size_t i=0; i!=fix_1.size(); ++i)
        base_sol.set(m_kp.get_variable(fix_1[i]));

      for ( it_s=sub_sols.begin(); it_s!=sub_sols.end(); ++it_s )
        {
          knapsack_solution sol(base_sol);

          for (std::size_t i=0; i!=sub_p.get_size(); ++i)
            if ( it_s->status_of( sub_p.get_variable(i) ) == moo::bin_set )
              sol.set( red_p.get_source_variable( sub_p.get_variable(i) ) );
            else
              sol.unset( red_p.get_source_variable( sub_p.get_variable(i) ) );

          result.insert(sol);
        }
    }

  m_XSEM.insert( m_XSEM.begin(), result.begin(), result.end() );
} // phase_1::find_equivalent_solutions()
