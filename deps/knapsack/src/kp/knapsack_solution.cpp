/**
 * \file knapsack_solution.cpp
 * \brief Implementation of the kp::knapsack_solution class.
 * \author Julien Jorge
 */
#include "kp/knapsack_solution.hpp"
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The solution to copy from.
 */
kp::knapsack_solution::knapsack_solution( const knapsack_solution& that )
  : m_solution(that.m_solution), m_value(that.m_value)
{

} // knapsack_solution::knapsack_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the problem.
 * \param p The number of objectives.
 */
kp::knapsack_solution::knapsack_solution( unsigned int n, unsigned int p )
  : m_solution(n), m_value(p)
{

} // knapsack_solution::knapsack_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a variable in the solution.
 * \param v The variable to add.
 */
void kp::knapsack_solution::set( const variable& v )
{
  assert( v.index < m_solution.size() );

  if ( (m_solution.status_of(v.index) == moo::bin_unset)
       || (m_solution.status_of(v.index) == moo::bin_free) )
    {
      m_solution[v.index] = moo::bin_set;
      m_value.cost += v.cost;
      m_value.weight += v.weight;
    }
  /*else
    std::clog << "Warning: setting a variable already set: (" << v << ") "
	      << m_solution.size() << std::endl;*/
} // knapsack_solution::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a variable from the solution.
 * \param v The variable to remove.
 */
void kp::knapsack_solution::unset( const variable& v )
{
  assert( v.index < m_solution.size() );

  if ( m_solution.status_of(v.index) == moo::bin_set )
    {
      m_value.cost -= v.cost;
      m_value.weight -= v.weight;
    }

  m_solution[v.index] = moo::bin_unset;
} // knapsack_solution::unset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Free a variable in the solution.
 * \param v The to free.
 */
void kp::knapsack_solution::free( const variable& v )
{
  assert( v.index < m_solution.size() );

  if ( m_solution.status_of(v.index) == moo::bin_set )
    {
      m_value.cost -= v.cost;
      m_value.weight -= v.weight;
    }

  m_solution[v.index] = moo::bin_free;
} // knapsack_solution::free()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the status of a variable.
 * \param v The variable to check.
 */
moo::bin_status kp::knapsack_solution::status_of( const variable& v ) const
{
  return m_solution.status_of( v.index );
} // knapsack_solution::status_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solution vector.
 */
const moo::binary_solution& kp::knapsack_solution::solution() const
{
  return m_solution;
} // knapsack_solution::solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solution value.
 */
const kp::solution_value& kp::knapsack_solution::value() const
{
  return m_value;
} // knapsack_solution::value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the image of the solution in the objective space.
 */
const kp::knapsack_solution::vector_type& kp::knapsack_solution::image() const
{
  return m_value.cost;
} // knapsack_solution::image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator==( const knapsack_solution& that ) const
{
  return (m_value == that.m_value) && (m_solution == that.m_solution);
} // knapsack_solution::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Not equal.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator!=( const knapsack_solution& that ) const
{
  return (m_value != that.m_value) || (m_solution != that.m_solution);
} // knapsack_solution::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower than' operator.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator<( const knapsack_solution& that ) const
{
  if ( m_value == that.m_value )
    return m_solution < that.m_solution;
  else
    return m_value < that.m_value;
} // knapsack_solution::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower or equal' operator.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator<=( const knapsack_solution& that ) const
{
  if ( m_value == that.m_value )
    return m_solution <= that.m_solution;
  else
    return m_value < that.m_value;
} // knapsack_solution::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater than' operator.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator>( const knapsack_solution& that ) const
{
  return that < *this;
} // knapsack_solution::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater or equal' operator.
 * \param that The solution to compare to.
 */
bool kp::knapsack_solution::operator>=( const knapsack_solution& that ) const
{
  return that <= *this;
} // knapsack_solution::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a solution in an output stream.
 * \param os The stream in which we write.
 * \param sol The solution to write.
 */
std::ostream& operator<<( std::ostream& os, const kp::knapsack_solution& sol )
{
  return os << sol.value() << "\t" << sol.solution();
} // operator<<() [knapsack_solution]
