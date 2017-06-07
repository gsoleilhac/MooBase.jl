/**
 * \file astar.cpp
 * \brief Implementation of the kp::astar class.
 * \author Julien Jorge
 */
#include <kp/astar.hpp>

#include <kp/cardinality_lower_bound.hpp>
#include <kp/cardinality_upper_bound.hpp>
#include <kp/linear_relaxation.hpp>
#include <kp/view_weighted_sum.hpp>

#include <bikp/problem/mono_problem.hpp>
#include <bikp/reduction/mtr_variable_fixing.hpp>
#include <bikp/solver/dynamic_programming_single.hpp>

#include <climits>

// fastest: 0 1 x
// less nodes: 1 1 x

// Tell if we compute the supported solutions in each node
#define SUPPORTED_SOLUTIONS 1

// Tell if we compute an idea point instead of an utopian one
#define IDEAL_POINT 1

// Tell if we sort the nodes (otherwise it is a deep-first search)
#define SORT_NODES 1

#if !SORT_NODES
#include <stack>
#endif

static std::size_t g_diffuse_dominance(0);
static std::size_t g_cardinality(0);
static std::size_t g_utopian(0);
static std::size_t g_relax(0);
static std::size_t g_hull(0);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param data Some data to help solving the knapsack.
 */
kp::astar::node::node( solution_data& data )
  : m_data(data), m_pattern(m_data.get_instance().get_size()),
    m_nb_dominated_solutions(0), m_pending_solution(data.get_instance()),
    m_free_variables_count(m_data.get_instance().get_size()),
    m_remaining_capacity(m_data.get_instance().get_capacity()),
    m_tightness_ratio(0), m_utopian_upper_bound(0)
{
#if SUPPORTED_SOLUTIONS
  if ( interesting() )
    {
      m_dead = false;
      fill_my_solutions();
    }
  else
    m_dead = true;
#else
  m_dead = !interesting();
#endif
} // astar::node::node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the remaining capacity.
 */
moo::real_type kp::astar::node::get_remaining_capacity() const
{
  return m_remaining_capacity;
} // astar::node::get_remaining_capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the tightness ratio on the free variables.
 */
moo::real_type kp::astar::node::get_tightness_ratio() const
{
  return m_tightness_ratio;
} // astar::node::get_tightness_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of free variables.
 */
std::size_t kp::astar::node::get_free_variables_count() const
{
  return m_free_variables_count;
} // astar::node::get_free_variables_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of items in the pending solution.
 */
std::size_t kp::astar::node::get_cardinality() const
{
  return m_pending_solution.solution().cardinality();
} // astar::node::get_cardinality()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solutions of this node.
 */
const kp::maximum_set& kp::astar::node::get_solutions() const
{
  return m_my_solutions;
} // astar::node::get_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of dominated solutions in this node.
 */
std::size_t kp::astar::node::get_nb_dominated_solutions() const
{
  return m_nb_dominated_solutions;
} // astar::node::get_nb_dominated_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of dominated solutions in this node.
 */
void kp::astar::node::update_nb_dominated_solutions()
{
  if (m_dead)
    std::cout << "Updating dead node." << std::endl;

  m_nb_dominated_solutions = 0;

  for ( maximum_set::const_iterator it=m_my_solutions.begin();
        it!=m_my_solutions.end(); ++it )
    if ( m_data.is_dominated(*it) )
      ++m_nb_dominated_solutions;
} // astar::node::update_nb_dominated_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extends the node by setting one new variable.
 * \return Two nodes, having respectively the variable set to one and to zero.
 */
kp::astar::node_pair kp::astar::node::extends() const
{
  std::size_t i = next_index();
  node_pair result(new node(*this), new node(*this));

  if ( my_solutions_all_have( i, moo::bin_set ) )
    {
      result.first->m_my_solutions = m_my_solutions;
      result.first->m_pattern = m_pattern;
      result.first->m_reduced_hull = m_reduced_hull;
      result.first->m_nb_dominated_solutions = m_nb_dominated_solutions;
    }
  else if ( my_solutions_all_have( i, moo::bin_unset ) )
    {
      result.second->m_my_solutions = m_my_solutions;
      result.second->m_pattern = m_pattern;
      result.second->m_reduced_hull = m_reduced_hull;
      result.second->m_nb_dominated_solutions = m_nb_dominated_solutions;
    }

  if ( !result.first->branch_one(i) )
    {
      delete result.first;
      result.first = NULL;
    }

  if ( !result.second->branch_zero(i) )
    {
      delete result.second;
      result.second = NULL;
    }

  if ( m_dead && (result.first != NULL) && (result.second != NULL) )
    std::cout << "Dead node extended." << std::endl;

  return result;
} // astar::node::extends()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound and tell if the node seems interesting.
 */
bool kp::astar::node::bound_is_interesting()
{
  bool result = false;

  if ( bound_test() )
    {
#if SUPPORTED_SOLUTIONS
      fill_my_solutions();
      result = hull_test();
#else
      result = true;
#endif
    }

  return result;
} // node::bound_is_interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The node to copy information from.
 * \remark m_my_solutions is not copied.
 */
kp::astar::node::node( const node& that )
  : m_data(that.m_data), m_pattern(m_data.get_instance().get_size()),
    m_nb_dominated_solutions(0),
    m_pending_solution(that.m_pending_solution),
    m_free_variables_count(that.m_free_variables_count),
    m_remaining_capacity(that.m_remaining_capacity),
    m_tightness_ratio(that.m_tightness_ratio), m_utopian(that.m_utopian),
    m_utopian_upper_bound(that.m_utopian_upper_bound), m_dead(that.m_dead)
{

} // astar::node::node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all supported solutions have a given variable with the same
 *        given value.
 */
bool
kp::astar::node::my_solutions_all_have( std::size_t i, moo::bin_status s ) const
{
  return m_pattern[ m_data.get_view().get_variable(i).index ] == s;
} // astar::node::my_solutions_all_have()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill m_pattern.
 */
void kp::astar::node::make_pattern()
{
  if ( !m_my_solutions.empty() )
    {
      m_pattern = m_my_solutions.begin()->solution();

      for( maximum_set::const_iterator it=m_my_solutions.begin();
           it!=m_my_solutions.end(); ++it )
        for ( std::size_t i=0; i!=m_pattern.size(); ++i )
          {
            const variable& v( m_data.get_view().get_variable(i) );

            if ( it->status_of( v ) != m_pattern[ v.index ] )
              m_pattern[ v.index ] = moo::bin_free;
          }
    }
} // astar::node::make_pattern()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a free variable.
 */
std::size_t kp::astar::node::next_index() const
{
  std::size_t i = 0;

#if 0
  while ( m_pending_solution.status_of
          ( m_data.get_view().get_variable(i) ) != moo::bin_free )
    ++i;

  return i;
#endif

  std::size_t result = m_data.get_instance().get_size();
  bool stop=false;

  while ( !stop && (i!=m_data.get_instance().get_size()) )
    if ( m_pending_solution.status_of
         ( m_data.get_view().get_variable(i) ) != moo::bin_free )
      ++i;
    else if ( my_solutions_all_have( i, moo::bin_set )
              || my_solutions_all_have( i, moo::bin_unset ) )
      {
        result = i;
        stop = true;
      }
    else
      {
        if ( result == m_data.get_instance().get_size() )
          result = i;
        ++i;
      }

  return result;
} // astar::node::next_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Branch by setting a variable to 1.
 * \param i The index of the variable to set.
 */
bool kp::astar::node::branch_one( std::size_t i )
{
  assert( i < m_data.get_view().get_size() );
  assert( m_data.get_view().get_capacity() - m_pending_solution.value().weight
          == m_remaining_capacity );
  assert( m_data.get_view().get_variable(i).weight <= m_remaining_capacity );
  assert( m_free_variables_count > 0 );
  assert( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
          == moo::bin_free );
  
  bool result(false);

  m_pending_solution.set( m_data.get_view().get_variable(i) );
  m_remaining_capacity -= m_data.get_view().get_variable(i).weight;
  --m_free_variables_count;

  if ( diffuse_dominance_one() )
    {
      unset_big_items();
      diffuse_dominance_zero();

      if ( interesting() )
        {
#if SUPPORTED_SOLUTIONS
          fill_my_solutions();

          if ( !m_my_solutions.empty() )
            result = hull_test();
#else
          result = true;
#endif
        }
    }
  else
    ++g_diffuse_dominance;

  m_dead = m_dead || !result;

  return result;
} // astar::node::branch_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Branch by setting a variable to 0.
 * \param i The index of the variable to set.
 */
bool kp::astar::node::branch_zero( std::size_t i )
{
  assert( i < m_data.get_view().get_size() );
  assert( m_data.get_view().get_capacity() - m_pending_solution.value().weight
          == m_remaining_capacity );
  assert( m_free_variables_count > 0 );
  assert( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
          == moo::bin_free );
  
  bool result(false);

  m_pending_solution.unset( m_data.get_view().get_variable(i) );
  --m_free_variables_count;

  diffuse_dominance_zero();

  if ( interesting() )
    {
#if SUPPORTED_SOLUTIONS
      fill_my_solutions();

      if ( !m_my_solutions.empty() )
        result = hull_test();
#else
    result = true;
#endif
    }

  m_dead = m_dead || !result;

  return result;
} // astar::node::branch_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to zero all items that are too big for the knapsack.
 */
void kp::astar::node::unset_big_items()
{
  const std::size_t s = m_data.get_view().get_size();

  for (std::size_t i=0; i!=s; ++i)
    {
      const variable& v( m_data.get_view().get_variable(i) );

      if ( m_pending_solution.status_of( v ) == moo::bin_free )
        if ( v.weight > m_remaining_capacity )
          {
            m_pending_solution.unset( v );
            --m_free_variables_count;
          }
    }
} // astar::node::unset_big_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if it seems interesting to explore this node, and compute the set
 *        of supported solutions reachable from this node.
 */
bool kp::astar::node::interesting()
{
  assert( m_free_variables.empty() );
  bool result = true;

  if ( m_free_variables_count == 0 )
    {
      m_data.insert(m_pending_solution);
      result = false;
    }
  else 
    {
      fill_free_variables();
      bool do_rl = false;

      if ( m_pending_solution.solution().cardinality()
           < m_data.get_cardinality_lower_bound() )
        {
          cardinality_upper_bound< view_order<> > card_ub;
          const std::size_t card_ub_val =
            card_ub.compute
            ( m_data.get_view(), m_free_variables, m_remaining_capacity );
          
          if ( m_pending_solution.solution().cardinality() + card_ub_val
               < m_data.get_cardinality_lower_bound() )
            {
              result = false;
              ++g_cardinality;
            }
          else
            do_rl = true;
        }
      else
        do_rl = true;

      assert( do_rl == result );
      if ( do_rl )
        {
          compute_utopian();
          result = bound_test();
        }
    }

  return result;
} // astar::node::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound and check if the subtree seems interesting,
 *        using an exact convex cover of the solutions of the subtree.
 * \return true if the subtree seems interesting.
 */
bool kp::astar::node::hull_test() const
{
  bool result;

  if ( m_reduced_hull.empty() )
    result = false;
  else if ( m_my_solutions.empty() )
    result = true;
  else
    {
      cost_compare compare;
      bound_set_type::const_iterator it_n;

      result = false;

      // check that not all nadir-like points are below the hull
      for ( it_n=m_data.bound_set_begin();
            !result && (it_n!=m_data.bound_set_end()); ++it_n )
        if ( compare( m_utopian, *it_n ) & moo::general_dominance )
          {
            constraint_set_type::const_iterator it_c;
            result = true; // suppose that this nadir satisfies all constraints

            // test the constraints that define the weight set of the hull
            for ( it_c=m_reduced_hull.begin();
                  result && (it_c!=m_reduced_hull.end()); ++it_c )
              result = it_c->check( *it_n - m_pending_solution.image() );
          }
    }

  if (!result)
    ++g_hull;

  return result;
} // astar::node::hull_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound and check if the subtree seems interesting.
 * \return true if the subtree seems interesting.
 */
bool kp::astar::node::bound_test() const
{
  bool result = false;

  if ( !m_data.is_dominated(m_utopian) )
    {
      cost_compare compare;
      bound_set_type::const_iterator it_n;

      for ( it_n=m_data.bound_set_begin();
            !result && (it_n!=m_data.bound_set_end()); ++it_n )
        if ( compare(m_utopian, *it_n) & moo::general_dominance )
          {
            const moo::real_type lower_bound = it_n->scalar_product(m_utopian);
            result = m_utopian_upper_bound >= lower_bound;
          }

      if (!result)
        ++g_relax;
    }
  else
    ++g_utopian;

  return result;
} // astar::node::bound_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to one all variables prefered to a variable set to one.
 * \return false if there is a free variable prefered to a variable set to zero
 *         and too big to fit in the knapsack.
 */
bool kp::astar::node::diffuse_dominance_one()
{ 
  std::list<std::size_t> free_vars;
  const std::size_t s = m_data.get_view().get_size();

  // \todo Récupérer aussi l'ensemble des variables à 1
  for (std::size_t i=0; i!=s; ++i)
    if ( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
         == moo::bin_free )
      free_vars.push_front(i);

  bool result = true;

  for (std::size_t i=0; (i!=s) && result && (m_free_variables_count!=0); ++i)
    if ( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
         == moo::bin_set )
      {
        std::list<std::size_t>::iterator it=free_vars.begin();

        while ( (it!=free_vars.end()) && result )
          if ( m_data.first_dominates_second(*it, i) )
            {
              if ( m_data.get_view().get_variable(*it).weight
                   > m_remaining_capacity )
                result = false;
              else
                {
                  m_pending_solution.set( m_data.get_view().get_variable(*it) );
                  --m_free_variables_count;
                  m_remaining_capacity -=
                    m_data.get_view().get_variable(*it).weight;

                  std::list<std::size_t>::iterator tmp(it);
                  ++it;
                  free_vars.erase(tmp);
                }
            }
          else
            ++it;
      }

  return result;
} // astar::node::diffuse_dominance_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set to zero all variables dominated by a variable set to zero.
 */
void kp::astar::node::diffuse_dominance_zero()
{
  std::list<std::size_t> free_vars;
  const std::size_t s = m_data.get_view().get_size();

  // todo Récupérer aussi les variables à 0
  for (std::size_t i=0; i!=s; ++i)
    if ( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
         == moo::bin_free )
      free_vars.push_front(i);

  for (std::size_t i=0; (i!=s) && (m_free_variables_count!=0); ++i)
    if ( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
         == moo::bin_unset )
      {
        std::list<std::size_t>::iterator it=free_vars.begin();

        while ( it!=free_vars.end() )
          if ( m_data.first_dominates_second(i, *it) )
            {
              assert
                ( m_pending_solution.status_of
                  ( m_data.get_view().get_variable(*it) ) == moo::bin_free );

              m_pending_solution.unset( m_data.get_view().get_variable(*it) );
              --m_free_variables_count;

              std::list<std::size_t>::iterator tmp(it);
              ++it;
              free_vars.erase(tmp);
            }
          else
            ++it;
      }
} // astar::node::diffuse_dominance_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the vector of the indices of the free variables.
 * \return The minimum weight of the free variables.
 */
moo::real_type kp::astar::node::fill_free_variables()
{
  assert( m_free_variables.empty() );

  m_free_variables.resize(m_free_variables_count);
  m_tightness_ratio = 0;
  std::size_t j=0;
  moo::real_type min_w = m_data.get_view().get_capacity();
  const std::size_t s = m_data.get_view().get_size();

  for (std::size_t i=0; i!=s; ++i)
    if ( m_pending_solution.status_of( m_data.get_view().get_variable(i) )
         == moo::bin_free )
      {
        assert( m_data.get_view().get_variable(i).weight
                <= m_remaining_capacity );

        m_free_variables[j] = i;
        ++j;

        m_tightness_ratio += m_data.get_view().get_variable(i).weight;

        if ( m_data.get_view().get_variable(i).weight < min_w )
          min_w = m_data.get_view().get_variable(i).weight;
      }

  m_tightness_ratio /= m_data.get_view().get_capacity();

  return min_w;
} // node::fill_free_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the supported solutions reachable from this node.
 */
void kp::astar::node::fill_my_solutions()
{
  assert( !m_free_variables.empty() );

  if ( m_my_solutions.empty() )
    {
#if ! SUPPORTED_SOLUTIONS
      std::cout << "Computing supported solutions (should not)." << std::endl;
#endif
      sub_problem_type sub_p
        (m_data.get_view(), m_free_variables, m_remaining_capacity);
      phase_1_type phase(sub_p);

      phase.run();
      m_reduced_hull = phase.get_hull();

      for (std::size_t i=0; i!=m_utopian.size(); ++i)
        m_utopian[i] = 0;

      std::list<knapsack_solution>::const_iterator it;

      for (it=phase.get_XSEM().begin(); it!=phase.get_XSEM().end(); ++it)
        {
          knapsack_solution sol( m_pending_solution );

          for (std::size_t i=0; i!=m_free_variables.size(); ++i)
            if ( it->status_of( sub_p.get_variable(i) ) & moo::bin_unset )
              sol.unset( m_data.get_view().get_variable(m_free_variables[i]) );
            else
              sol.set( m_data.get_view().get_variable(m_free_variables[i]) );

          m_data.insert(sol);
          m_my_solutions.insert(sol);

          for (std::size_t i=0; i!=m_utopian.size(); ++i)
            m_utopian[i] = std::max(m_utopian[i], sol.image()[i]);

          assert( sol.image() == m_pending_solution.image() + it->image() );
          assert( sol.value().weight
                  == m_pending_solution.value().weight + it->value().weight );
        }

      compute_utopian_upper_bound();
    }

  make_pattern();
} // astar::node::fill_my_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the utopian point.
 */
void kp::astar::node::compute_utopian()
{
  assert( !m_free_variables.empty() );

  cost_vector u(m_data.get_view().get_nb_objectives(), 0);

#if IDEAL_POINT
  for ( std::size_t j=0; j!=m_data.get_view().get_nb_objectives(); ++j )
    {
      // create a mono objective problem
      mono_problem p(m_free_variables.size(), m_remaining_capacity);

      for ( std::size_t i=0; i!=m_free_variables.size(); ++i )
        p.set_variable
          ( i, m_data.get_view().get_variable(m_free_variables[i]).cost[j],
            m_data.get_view().get_variable(m_free_variables[i]).weight );

      p.sort_by_decreasing_efficiency();

      // reduce the problem
      mono_problem::solution_type sol(p);
      mtr_variable_fixing red(p, sol);

      // solve the reduced problem
      const mono_problem& sub_p(red.get_sub_problem());

      if (sub_p.size() == 0)
        u[j] = m_pending_solution.image()[j] + sol.objective_value();
      else
        {
          mono_problem::solution_type sub_sol(sub_p);
          half_line<mono_problem::profit_traits_type> region(0, true);
          dynamic_programming_single<mono_problem> solver(sub_p, region);
          solver.run(sub_sol);

          u[j] = m_pending_solution.image()[j] +
            std::max( sol.objective_value(),
                      sub_sol.objective_value() + red.guaranteed_profit() );
        }
    }
#else
  linear_relaxation< view_order<> > rl;

  u = ( m_pending_solution.value().cost
      + rl.compute
      (m_data.get_view(), m_free_variables, m_remaining_capacity) );
#endif

  bool new_utopian(false);

  if ( m_utopian.empty() )
    {
      new_utopian = true;
      m_utopian = u;
    }
  else
    for ( std::size_t i=0; i!=u.size(); ++i )
      if ( u[i] < m_utopian[i] )
        {
          m_utopian[i] = u[i];
          new_utopian = true;
        }

  if (new_utopian)
    compute_utopian_upper_bound();
} // astar::node::compute_utopian()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute an upper bound toward the utopian point.
 */
void kp::astar::node::compute_utopian_upper_bound()
{
  assert( !m_free_variables.empty() );

  if ( !m_data.is_dominated(m_utopian) )
    {
      view_weighted_sum< view_order<> > mono(m_data.get_view(), m_utopian);
      linear_relaxation< view_weighted_sum< view_order<> > > rl;

      m_utopian_upper_bound =
        rl.compute( mono, m_free_variables, m_remaining_capacity )[0]
        + m_pending_solution.value().cost.scalar_product(m_utopian);
    }
} // astar::node::compute_utopian_upper_bound()




/*----------------------------------------------------------------------------*/
bool kp::astar::node_comparator::operator()
  ( const node* const a, const node* const b ) const
{
  int result = 0;

  if ( result == 0 )
    result = compare_max
      ( std::abs(0.5 - a->get_tightness_ratio()),
        std::abs(0.5 - b->get_tightness_ratio()) );

  if ( result == 0 )
    result = compare_min
      ( a->get_nb_dominated_solutions(), b->get_nb_dominated_solutions() );

  if ( result == 0 )
    result = compare_max
      ( a->get_solutions().size(), b->get_solutions().size() );

  if ( result == 0 )
    result = compare_max
      ( a->get_solutions().size(), b->get_solutions().size() );

  if ( result == 0 )
    result = compare_min
      ( a->get_remaining_capacity(), b->get_remaining_capacity() );

  if ( result == 0 )
    result = compare_max
      ( a->get_cardinality(), b->get_cardinality() );

  return result == 1;
} // astar::node_comparator::operator()()

/*----------------------------------------------------------------------------*/
int
kp::astar::node_comparator::compare_min( std::size_t a, std::size_t b ) const
{
  if ( a<b )
    return -1;
  else if ( a==b )
    return 0;
  else
    return 1;
} // astar::node_comparator::compare_min()

/*----------------------------------------------------------------------------*/
int
kp::astar::node_comparator::compare_max( std::size_t a, std::size_t b ) const
{
  if ( a>b )
    return -1;
  else if ( a==b )
    return 0;
  else
    return 1;
} // astar::node_comparator::compare_max()




/*----------------------------------------------------------------------------*/
/**
 * \brief Solve an instance of a multi-objective knapsack problem using a branch
 *        and bound algorithm.
 * \param inst The instance to solve.
 * \param X (in) known solutions, (out) all efficient solutions.
 * \param o The order applied to the variables.
 */
void kp::astar::solve( const knapsack_instance& inst, maximum_set& X, int o )
{
  solution_data data(inst, X, o);
#if SORT_NODES
  queue_type pending;
#else
  std::stack<node*> pending;
#endif

  std::size_t nodes_explored(0);
  std::size_t nodes_fathomed(0);
  std::size_t overflow(0);
  std::size_t ref_n(0);
  time_t ref_date( time(NULL) );

  pending.push( new node(data) );

  std::cout << "starting A*" << std::endl;

  while ( !pending.empty() )
    {
      time_t now = time(NULL);

      if ( now - ref_date >= 5 ) // 5 seconds interval log
        {
          const std::size_t n(pending.size());
          std::cout << n << " remaining nodes ("
                    << ((double)(nodes_explored - ref_n)
                        / (double)(now - ref_date))
                    << " nodes per second, " << nodes_explored
                    << " explored [+" << (nodes_explored - ref_n) << "]) and "
                    << X.size() << " solutions."
                    << std::endl;
          ref_n = nodes_explored;
          ref_date = now;
        }

      node* n = pending.top();
      pending.pop();

      if ( n->bound_is_interesting() )
        {
          ++nodes_explored;
          if (nodes_explored == 0)
            ++overflow;

          data.unset_modified();
          node_pair new_n = n->extends();

          if ( new_n.second != NULL )
            pending.push(new_n.second);

          if ( new_n.first != NULL )
            pending.push(new_n.first);

#if SORT_NODES
          if ( data.is_modified() )
            update_nb_dominated_solutions( pending );
#endif
        }
      else
        ++nodes_fathomed;

      delete n;
    }

  std::cout << nodes_explored << " nodes explored (+2^"
            << (sizeof(std::size_t) * CHAR_BIT) << " * " << overflow << "), "
            << nodes_fathomed << " nodes fathomed. |"
            << " diff=" << g_diffuse_dominance
            << " card=" << g_cardinality
            << " utopian=" << g_utopian
            << " relax=" << g_relax
            << " hull=" << g_hull
            << std::endl;
} // astar::solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the number of dominated solutions in each node.
 * \param pending The pending nodes.
 */
void kp::astar::update_nb_dominated_solutions( queue_type& pending )
{
  queue_type old_pending;
  std::swap( old_pending, pending );

  for ( ; !old_pending.empty(); old_pending.pop() )
    {
      node* n = old_pending.top();
      n->update_nb_dominated_solutions();
      pending.push(n);
    }
} // astar::update_nb_dominated_solutions()
