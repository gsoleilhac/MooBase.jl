/**
 * \file solution_value.hpp
 * \brief The value (cost and capacity) of a solution of a knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_SOLUTION_VALUE_HPP__
#define __KP_SOLUTION_VALUE_HPP__

#include <kp/knapsack_instance.hpp>

namespace kp
{
  /**
   * \brief The value (cost and capacity) of a solution of a knapsack problem.
   * \author Julien Jorge
   */
  class solution_value
  {
  public:
    solution_value( unsigned int p );

    bool operator==( const solution_value& that ) const;
    bool operator!=( const solution_value& that ) const;
    bool operator<( const solution_value& that ) const;
    bool operator<=( const solution_value& that ) const;
    bool operator>( const solution_value& that ) const;
    bool operator>=( const solution_value& that ) const;

  public:
    /** \brief Total value of the solution. */
    cost_vector cost;

    /** \brief Total weight of the solution. */
    moo::real_type weight;

  }; // class solution_value
} // namespace kp

std::ostream& operator<<( std::ostream& os, const kp::solution_value& sol );

#endif // __KP_SOLUTION_VALUE_HPP__
