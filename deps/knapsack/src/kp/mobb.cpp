/**
 * \file mobb.cpp
 * \brief Implementation of the kp::mobb class.
 * \author Julien Jorge
 */
#include <kp/mobb.hpp>

#include <kp/cardinality_lower_bound.hpp>
#include <kp/cardinality_upper_bound.hpp>
#include <kp/linear_relaxation.hpp>
#include <kp/view_weighted_sum.hpp>
#include <kp/phase_1.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param instance The instance to solve.
 * \param X (in/out) The set of efficient solutions.
 * \param o The order applied to the variables.
 */
kp::mobb::bb_procedure::bb_procedure
( const knapsack_instance& instance, maximum_set& X, int o )
  : m_instance(instance), m_kp(m_instance),
    m_bound_set( cost_vector(m_kp.get_nb_objectives(), 0) ),
    m_solutions(X)
{
  total_nodes =
    closed_diffuse_dominance_one_counter =
    closed_cardinality_counter =
    closed_utopian_counter =
    closed_relaxation_counter = 0;

  cardinality_lower_bound<> card;
  m_kp_card_lb = card.compute(instance);

  switch(o)
    {
    case order_on_items::order_topological:
      m_kp.topological_efficiency();
      break;
    case order_on_items::order_max_rank: m_kp.max_rank(); break;
    case order_on_items::order_min_rank: m_kp.min_rank(); break;
    case order_on_items::order_sum_rank: m_kp.sum_rank(); break;
    case order_on_items::order_frequency: m_kp.frequency(m_solutions); break;
    case order_on_items::order_random: m_kp.random(); break;
    case order_on_items::order_domination_rank: m_kp.domination_rank(); break;
    case order_on_items::order_domination_count: m_kp.domination_count(); break;
    default: m_kp.decreasing_efficiency(o);
    }

  maximum_set::const_iterator it;

  for (it=X.begin(); it!=X.end(); ++it)
    m_bound_set.reduce( it->image() );

  const unsigned int n = m_instance.get_size();

  m_dominates.resize(n);
  std::fill( m_dominates.begin(), m_dominates.end(),
             std::vector<bool>(n, false) );

  for (unsigned int i=0; i!=n; ++i)
    for (unsigned int j=i+1; j!=n; ++j)
      {
        m_dominates[i][j] = dominates(i, j);
        m_dominates[j][i] = dominates(j, i);
      }

  /*std::ofstream f("ordered.coord");
  for (unsigned int i=0; i!=n; ++i)
    {
      f << m_kp.get_variable(i).index;

      for (unsigned int j=0; j!=m_kp.get_nb_objectives(); ++j)
        f << ' ' << m_kp.get_variable(i).cost[j];

      f << ' ' << m_kp.get_variable(i).weight << '\n';
    }*/
} // mobb::bb_procedure::bb_procedure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Solve an instance of a multi-objective knapsack problem using a branch
 *        and bound algorithm.
 * \param inst The instance to solve.
 * \param X (in) known solutions, (out) all efficient solutions.
 */
void kp::mobb::bb_procedure::run()
{
  knapsack_solution sol(m_instance);

  bb( m_kp.get_capacity(), sol, m_kp.get_size() );
} // mobb::bb_procedure::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a variables \a i dominates a variables \a j.
 * \param i The index of the first variable.
 * \param j The index of the second variable.
 */
bool kp::mobb::bb_procedure::dominates( unsigned int i, unsigned int j ) const
{
  cost_compare compare;
  bool result = false;

  const variable& vi( m_kp.get_variable(i) );
  const variable& vj( m_kp.get_variable(j) );

  if ( compare( vi.cost, vj.cost ) & moo::general_dominance )
    result = vi.weight <= vj.weight;
  else if ( vi.cost == vj.cost )
    result = vi.weight < vj.weight;;

  return result;
} // mobb::bb_procedure::dominates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the branch and bound to the current instance.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param n The number of free variables.
 */
void kp::mobb::bb_procedure::bb
( moo::real_type w, knapsack_solution& sol, unsigned int n )
{
  assert( m_kp.get_capacity() - sol.value().weight == w );

  ++total_nodes;

  if ( n==0 )
    {
      assert( m_instance.check(sol) );
      m_solutions.insert(sol);
      m_bound_set.reduce(sol.image());
    }
  else
    {
      unsigned int i = next_index(sol);

      branch_one(w, sol, i, n);
      branch_zero(w, sol, i, n);
    }
} // mobb::bb_procedure::bb()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a free variable.
 * \param sol The solution that we are building.
 */
unsigned int
kp::mobb::bb_procedure::next_index( const knapsack_solution& sol ) const
{
  unsigned int i = 0;

  while ( sol.status_of( m_kp.get_variable(i) ) != moo::bin_free )
    ++i;

  return i;
} // mobb::bb_procedure::next_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Branch by setting a variable to 1.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param i The index of the variable to set.
 * \param n The number of free variables.
 */
void kp::mobb::bb_procedure::branch_one
( moo::real_type w, knapsack_solution& sol, unsigned int i, unsigned int n )
{
  assert( i < m_kp.get_size() );
  assert( m_kp.get_capacity() - sol.value().weight == w );
  assert( m_kp.get_variable(i).weight <= w );
  assert( n > 0);
  assert( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free );
  
  std::list<unsigned int> set_to_zero;
  std::list<unsigned int> set_to_one;

  sol.set( m_kp.get_variable(i) );
  --n;
  w -= m_kp.get_variable(i).weight;

  if ( diffuse_dominance_one(w, sol, n, set_to_one) )
    {
      n -= unset_big_items(w, sol, set_to_zero);
      diffuse_dominance_zero(sol, n, set_to_zero);

      if ( interesting(w, sol, n) )
        bb(w, sol, n);
    }
  else
    ++closed_diffuse_dominance_one_counter;

  sol.free( m_kp.get_variable(i) );

  std::list<unsigned int>::const_iterator it;

  for (it=set_to_zero.begin(); it!=set_to_zero.end(); ++it)
    sol.free( m_kp.get_variable(*it) );

  for (it=set_to_one.begin(); it!=set_to_one.end(); ++it)
    sol.free( m_kp.get_variable(*it) );
} // mobb::bb_procedure::branch_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Branch by setting a variable to 0.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param i The index of the variable to set.
 * \param n The number of free variables.
 */
void kp::mobb::bb_procedure::branch_zero
( moo::real_type w, knapsack_solution& sol, unsigned int i, unsigned int n )
{
  assert( i < m_kp.get_size() );
  assert( n > 0 );
  assert( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free );
  assert( m_kp.get_capacity() - sol.value().weight == w );

  std::list<unsigned int> set_to_zero;

  sol.unset( m_kp.get_variable(i) );
  --n;

  diffuse_dominance_zero(sol, n, set_to_zero );

  if ( interesting(w, sol, n) )
    bb(w, sol, n);

  sol.free( m_kp.get_variable(i) );

  std::list<unsigned int>::const_iterator it;

  for (it=set_to_zero.begin(); it!=set_to_zero.end(); ++it)
    sol.free( m_kp.get_variable(*it) );
} // mobb::bb_procedure::branch_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to zero all items that are too big for the knapsack.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param set_to_zero The indices of the variables unset by this function.
 */
unsigned int kp::mobb::bb_procedure::unset_big_items
( moo::real_type w, knapsack_solution& sol,
  std::list<unsigned int>& set_to_zero ) const
{
  unsigned int result = 0;
  const unsigned int s = m_kp.get_size();

  for (unsigned int i=0; i!=s; ++i)
    if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free )
      if ( m_kp.get_variable(i).weight > w )
        {
          sol.unset( m_kp.get_variable(i) );
          set_to_zero.push_front(i);
          ++result;
        }

  return result;
} // mobb::bb_procedure::unset_big_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if it seems interesting to explore the subtree.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param n The number of free variables.
 * \return true if the subtree seems interesting.
 */
bool kp::mobb::bb_procedure::interesting
( moo::real_type w, const knapsack_solution& sol, unsigned int n ) const
{
  bool result = true;

  if ( n==0 )
    result = !m_solutions.is_dominated(sol);
  else 
    {
      std::vector<std::size_t> free_vars(n);
      unsigned int j=0;
      const unsigned int s = m_kp.get_size();

      for (unsigned int i=0; i!=s; ++i)
        if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free )
          {
            free_vars[j] = i;
            ++j;
          }

      bool do_rl = false;

      if ( sol.solution().cardinality() < m_kp_card_lb )
        {
          cardinality_upper_bound< view_order<> > card_ub;
          const std::size_t card_ub_val = card_ub.compute(m_kp, free_vars, w);
          
          if ( sol.solution().cardinality() + card_ub_val < m_kp_card_lb )
            {
              result = false;
              ++closed_cardinality_counter;
            }
          else
            do_rl = true;
        }
      else
        do_rl = true;

      if ( do_rl )
        {
          linear_relaxation< view_order<> > rl;

          const cost_vector utopian =
            sol.value().cost + rl.compute(m_kp, free_vars, w);

          result = bound_test( w, sol, free_vars, utopian );

#ifdef MOBB_DO_HULL_TEST_EXACT
          if ( result )
            {
              result = hull_test( w, sol, free_vars);
              if ( !result )
                std::cout << "EXACT HULL CUT " << free_vars.size()
                          << " remaining." << std::endl;
            }
#endif
        }
    }

  return result;
} // mobb::bb_procedure::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound and check if the subtree seems interesting,
 *        using an exact convex cover of the solutions of the subtree.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param free_vars The indices of the free variables.
 * \return true if the subtree seems interesting.
 */
bool kp::mobb::bb_procedure::hull_test
( moo::real_type w, const knapsack_solution& sol,
  const std::vector<std::size_t>& free_vars ) const
{
  bool result = false;

  typedef subset_view< view_order<> > sub_problem_type;
  typedef phase_1<sub_problem_type> phase_1_type;

  sub_problem_type sub_p(m_kp, free_vars, w);
  phase_1_type phase(sub_p);

  phase.run();

/*  for ( )
    {
      
      assert( m_instance.check(sol) );

      m_solutions.insert(sol);
      m_bound_set.reduce(sol.image());
    }*/

  bound_set_type::const_iterator it_n;

  // check if all nadir-like points are above the hull
  for ( it_n=m_bound_set.begin();
        !result && (it_n!=m_bound_set.end()); ++it_n )
    {
      std::set<phase_1_type::constraint_type>::const_iterator it_c;
      result = true; // suppose that this nadir satisfies all constraints

      // test the constraints that define the weight set of the hull
      for ( it_c=phase.get_hull().begin();
            result && (it_c!=phase.get_hull().end()); ++it_c )
        result = it_c->check( *it_n - sol.image() );
    }

  return result || (phase.get_XSEm().size() == 0);
} // mobb::bb_procedure::hull_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound and check if the subtree seems interesting.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param free_vars The indices of the free variables.
 * \param utopian An utopian point for the subtree.
 * \return true if the subtree seems interesting.
 */
bool kp::mobb::bb_procedure::bound_test
( moo::real_type w, const knapsack_solution& sol,
  const std::vector<std::size_t>& free_vars, const cost_vector& utopian ) const
{
  bool result = false;

  if ( !m_solutions.is_dominated(utopian) )
    {
      cost_compare compare;

      view_weighted_sum< view_order<> > mono(m_kp, utopian);
      linear_relaxation< view_weighted_sum< view_order<> > > rl;

      const moo::real_type upper_bound =
        rl.compute( mono, free_vars, w )[0]
        + sol.value().cost.scalar_product(utopian);

      bound_set_type::const_iterator it_n;

      for ( it_n=m_bound_set.begin();
            !result && (it_n!=m_bound_set.end()); ++it_n )
        if ( compare(utopian, *it_n) & moo::general_dominance )
          {
            const moo::real_type lower_bound = it_n->scalar_product(utopian);
            result = upper_bound >= lower_bound;
          }

      if ( result == false )
        ++closed_relaxation_counter;
    }
  else
    ++closed_utopian_counter;

  return result;
} // mobb::bb_procedure::bound_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to one all variables prefered to a variable set to one.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param n (in/out) The number of free variables.
 * \param set_to_one (out) The indices of the variables set to one.
 * \return false if there is a free variable prefered to a variable set to zero
 *         and too big to fit in the knapsack.
 */
bool kp::mobb::bb_procedure::diffuse_dominance_one
( moo::real_type& w, knapsack_solution& sol, unsigned int& n,
  std::list<unsigned int>& set_to_one ) const
{ 

  std::list<unsigned int> free_vars;
  const unsigned int s = m_kp.get_size();

  for (unsigned int i=0; i!=s; ++i)
    if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free )
      free_vars.push_front(i);

  bool result = true;

  for (unsigned int i=0; (i!=s) && result && (n!=0); ++i)
    if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_set )
      {
        std::list<unsigned int>::iterator it=free_vars.begin();

        while ( (it!=free_vars.end()) && result )
          if ( m_dominates[*it][i] )
            {
              if ( m_kp.get_variable(*it).weight > w )
                result = false;
              else
                {
                  sol.set( m_kp.get_variable(*it) );
                  set_to_one.push_front(*it);
                  --n;
                  w -= m_kp.get_variable(*it).weight;

                  std::list<unsigned int>::iterator tmp(it);
                  ++it;
                  free_vars.erase(tmp);
                }
            }
          else
            ++it;
      }

  return result;
} // mobb::bb_procedure::diffuse_dominance_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to zero all variables dominated by a variable set to zero.
 * \param w The remaining capacity.
 * \param sol The solution that we are building.
 * \param n (in/out) The number of free variables.
 * \param set_to_zero (out) The indices of the variables set to zero.
 */
void kp::mobb::bb_procedure::diffuse_dominance_zero
( knapsack_solution& sol, unsigned int& n,
  std::list<unsigned int>& set_to_zero ) const
{
  std::list<unsigned int> free_vars;
  const unsigned int s = m_kp.get_size();

  for (unsigned int i=0; i!=s; ++i)
    if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_free )
      free_vars.push_front(i);

  for (unsigned int i=0; (i!=s) && (n!=0); ++i)
    if ( sol.status_of( m_kp.get_variable(i) ) == moo::bin_unset )
      {
        std::list<unsigned int>::iterator it=free_vars.begin();

        while ( it!=free_vars.end() )
          if ( m_dominates[i][*it] )
            {
              assert( sol.status_of(m_kp.get_variable(*it)) == moo::bin_free );
              sol.unset( m_kp.get_variable(*it) );
              set_to_zero.push_front(*it);
              --n;

              std::list<unsigned int>::iterator tmp(it);
              ++it;
              free_vars.erase(tmp);
            }
          else
            ++it;
      }
} // mobb::bb_procedure::diffuse_dominance_zero()




/*----------------------------------------------------------------------------*/
/**
 * \brief Solve an instance of a multi-objective knapsack problem using a branch
 *        and bound algorithm.
 * \param inst The instance to solve.
 * \param X (in) known solutions, (out) all efficient solutions.
 * \param o The order applied to the variables.
 */
void kp::mobb::solve( const knapsack_instance& inst, maximum_set& X, int o )
{
  bb_procedure proc( inst, X, o );
  proc.run();
  std::cout << "nodes=" << proc.total_nodes
            << " diff=" << proc.closed_diffuse_dominance_one_counter
            << " card=" << proc.closed_cardinality_counter
            << " utopian=" << proc.closed_utopian_counter
            << " relax=" << proc.closed_relaxation_counter
            << std::endl;
} // mobb::solve()
