/**
 * \file dynamic_programming.hpp
 * \brief A dynamic programming algorithm for the knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_DYNAMIC_PROGRAMMING_HPP__
#define __KP_DYNAMIC_PROGRAMMING_HPP__

#include <kp/knapsack_instance.hpp>
#include <kp/types.hpp>

namespace kp
{
  /**
   * \brief A dynamic programming algorithm for the knapsack problem.
   * \author Julien Jorge
   */
  class dynamic_programming
  {
  public:
    void solve( const knapsack_instance& inst, pareto_set& Z ) const;
    void solve( const knapsack_instance& inst, minimum_set& Z ) const;

  }; // class dynamic_programming
} // namespace kp

#endif // __KP_DYNAMIC_PROGRAMMING_HPP__
