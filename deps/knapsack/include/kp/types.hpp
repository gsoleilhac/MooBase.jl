/**
 * \file kp/types.hpp
 * \brief Some types to 
 * \author Julien Jorge
 */
#ifndef __KP_TYPES_HPP__
#define __KP_TYPES_HPP__

#include <moo/bound_set.hpp>
#include <moo/pareto_set.hpp>
#include <moo/minimum_set.hpp>
#include <moo/maximum_set.hpp>
#include <kp/knapsack_solution.hpp>

namespace kp
{
  typedef moo::bound_set<cost_compare> bound_set;
  typedef moo::pareto_set<cost_compare> pareto_set;
  typedef moo::minimum_set<knapsack_solution> minimum_set;
  typedef moo::maximum_set<knapsack_solution> maximum_set;

} // namespace kp

#endif // __KP_TYPES_HPP__
