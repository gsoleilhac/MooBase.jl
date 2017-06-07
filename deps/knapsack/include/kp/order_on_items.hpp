/**
 * \file order_on_items.hpp
 * \brief How to order the items of an instance.
 * \author Julien Jorge
 */
#ifndef __KP_ORDER_ON_ITEMS_HPP__
#define __KP_ORDER_ON_ITEMS_HPP__

#include <string>

namespace kp
{
  /**
   * \brief How to order the items of an instance.
   * \author Julien Jorge
   */
  struct order_on_items
  {
    static const int order_topological = -1;
    static const int order_max_rank = -2;
    static const int order_min_rank = -3;
    static const int order_sum_rank = -4;
    static const int order_frequency = -5;
    static const int order_random = -6;
    static const int order_domination_rank = -7;
    static const int order_domination_count = -8;

    static int from_string( const std::string& s );
    static std::string to_string( int o );

  }; // struct order_on_items
} // namespace kp

#endif // __KP_ORDER_ON_ITEMS_HPP__
