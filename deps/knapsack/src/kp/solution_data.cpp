/**
 * \file astar.cpp
 * \brief Implementation of the kp::astar class.
 * \author Julien Jorge
 */
#include <kp/solution_data.hpp>

#include <kp/cardinality_lower_bound.hpp>
#include <kp/order_on_items.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param instance The instance to solve.
 * \param X (in/out) The set of efficient solutions.
 * \param o The order applied to the variables.
 */
kp::solution_data::solution_data
( const knapsack_instance& instance, maximum_set& X, int o )
  : m_instance(instance), m_kp(m_instance),
    m_bound_set( cost_vector(m_kp.get_nb_objectives(), 0) ), m_solutions(X),
    m_modified(false)
{
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

  const unsigned int n = instance.get_size();

  m_dominates.resize(n);
  std::fill( m_dominates.begin(), m_dominates.end(),
             std::vector<bool>(n, false) );

  for (unsigned int i=0; i!=n; ++i)
    for (unsigned int j=i+1; j!=n; ++j)
      {
        m_dominates[i][j] = dominates(i, j);
        m_dominates[j][i] = dominates(j, i);
      }
} // solution_data::solution_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the initial instance.
 */
const kp::knapsack_instance& kp::solution_data::get_instance() const
{
  return m_instance;
} // solution_data::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the view on the instance.
 */
const kp::view_order<> kp::solution_data::get_view() const
{
  return m_kp;
} // solution_data::get_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the lower bound on the cardinality of the knapsack.
 */
std::size_t kp::solution_data::get_cardinality_lower_bound() const
{
  return m_kp_card_lb;
} // solution_data::get_cardinality_lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a solution.
 * \param sol The solution to add.
 */
void kp::solution_data::insert( const knapsack_solution& sol )
{
  int r = m_solutions.insert(sol);

  if ( r!=0 ) // the solution was added
    {
      m_modified = true;
      m_bound_set.reduce(sol.image());
    }
} // solution_data::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is dominated by an other solution.
 * \param sol The solution to check.
 */
bool kp::solution_data::is_dominated( const knapsack_solution& sol ) const
{
  return m_solutions.is_dominated(sol);
} // solution_data::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a point is dominated by the image of a solution.
 * \param y The point to check.
 */
bool kp::solution_data::is_dominated( const cost_vector& y ) const
{
  return m_solutions.is_dominated(y);
} // solution_data::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the data of an item dominates the data of an other item.
 * \param i Check if the data of this item dominates the data of item \a j.
 * \param j Check if the data of this item is dominated by the data of item
 *        \a i.
 */
bool
kp::solution_data::first_dominates_second( std::size_t i, std::size_t j ) const
{
  return m_dominates[i][j];
} // solution_data::first_dominates_second()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the bound set.
 */
kp::solution_data::bound_set_type::const_iterator
kp::solution_data::bound_set_begin() const
{
  return m_bound_set.begin();
} // solution_data::bound_set_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the end of the bound set.
 */
kp::solution_data::bound_set_type::const_iterator
kp::solution_data::bound_set_end() const
{
  return m_bound_set.end();
} // solution_data::bound_set_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if m_solutions has received a solution corresponding to a new non
 *        dominated point.
 */
bool kp::solution_data::is_modified() const
{
  return m_modified;
} // solution_data::is_modified()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ensure is_modified() == false.
 */
void kp::solution_data::unset_modified()
{
  m_modified = false;
} // solution_data::unset_modified()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a variables \a i dominates a variables \a j.
 * \param i The index of the first variable.
 * \param j The index of the second variable.
 */
bool kp::solution_data::dominates( std::size_t i, std::size_t j ) const
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
} // solution_data::dominates()
