/**
 * \file kp/cardinality_upper_bound.hpp
 * \brief A class that computes an upper bound on the caridinality of the
 *        efficient solutions of a knapsack instance.
 * \author Julien Jorge
 */
#ifndef __KP_CARDINALITY_UPPER_BOUND_HPP__
#define __KP_CARDINALITY_UPPER_BOUND_HPP__

#include <kp/knapsack_instance.hpp>

namespace kp
{
  /**
   * \brief A class that computes an upper bound on the caridinality of the
   *        efficient solutions of a knapsack instance.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class cardinality_upper_bound
  {
  public:
    typedef View view_type;

  public:
    std::size_t compute
    ( const view_type& inst, const std::vector<std::size_t>& free_vars,
      moo::real_type w ) const;

  }; // class cardinality_upper_bound
} // namespace kp

#include <kp/impl/cardinality_upper_bound.tpp>

#endif // __KP_CARDINALITY_UPPER_BOUND_HPP__
