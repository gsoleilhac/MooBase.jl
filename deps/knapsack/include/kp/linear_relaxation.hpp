/**
 * \file kp/linear_relaxation.hpp
 * \brief A class that computes a linear relaxation on a knapsack instance.
 * \author Julien Jorge
 */
#ifndef __KP_LINEAR_RELAXATION_HPP__
#define __KP_LINEAR_RELAXATION_HPP__

#include <kp/cost_vector.hpp>

namespace kp
{
  /**
   * \brief A class that computes a linear relaxation on a knapsack instance.
   * \author Julien Jorge
   */
  template<typename View>
  class linear_relaxation
  {
  public:
    typedef View view_type;

  public:
    cost_vector compute
    ( const view_type& inst, const std::vector<std::size_t>& free_vars,
      moo::real_type w ) const;

  }; // class linear_relaxation
} // namespace kp

#include <kp/impl/linear_relaxation.tpp>

#endif // __KP_LINEAR_RELAXATION_HPP__
