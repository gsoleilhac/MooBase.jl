/**
 * \file solution_value.cpp
 * \brief Implementation of the solution_value class.
 * \author Julien Jorge
 */
#include "kp/solution_value.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The number of objectives.
 */
kp::solution_value::solution_value( unsigned int p )
  : cost( p, 0 ), weight(0)
{

} // solution_value::solution_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator==( const solution_value& that ) const
{
  return (cost == that.cost) && (weight == that.weight);
} // solution_value::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Not equal.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator!=( const solution_value& that ) const
{
  return (weight != that.weight) || (cost != that.cost);
} // solution_value::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower than' operator.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator<( const solution_value& that ) const
{
  if (cost == that.cost)
    return weight < that.weight;
  else
    return cost < that.cost;
} // solution_value::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower or equal' operator.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator<=( const solution_value& that ) const
{
  if (cost == that.cost)
    return weight <= that.weight;
  else
    return cost < that.cost;
} // solution_value::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater than' operator.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator>( const solution_value& that ) const
{
  return that < *this;
} // solution_value::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater or equal' operator.
 * \param that The solution to compare to.
 */
bool kp::solution_value::operator>=( const solution_value& that ) const
{
  return that <= *this;
} // solution_value::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a solution value in an output stream.
 * \param os The stream in which we write.
 * \param sol The value to write.
 */
std::ostream& operator<<( std::ostream& os, const kp::solution_value& sol )
{
  return os << sol.cost << "\t" << sol.weight;
} // operator<<() [solution_value]
