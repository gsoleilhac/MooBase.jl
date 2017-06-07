/**
 * \file cost_vector.hpp
 * \brief The cost vector of a knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_COST_VECTOR_HPP__
#define __KP_COST_VECTOR_HPP__

#include <moo/types.hpp>
#include <moo/vector.hpp>
#include <moo/direction.hpp>

namespace kp
{
  /**
   * \brief The cost vector of a knapsack problem.
   * \author Julien Jorge
   */
  typedef moo::vector<moo::real_type> cost_vector;

  typedef moo::compare_unidirectional_max<moo::real_type> cost_compare;

} // namespace kp

#endif // __KP_COST_VECTOR_HPP__
