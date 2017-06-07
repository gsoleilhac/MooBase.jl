/**
 * \file order_on_items.cpp
 * \brief Implementation of the kp::order_on_items class.
 * \author Julien Jorge
 */
#include "kp/order_on_items.hpp"

#include <stdexcept>
#include <sstream>

#define from_string_if(s, o) if ( (s) == #o ) return (o)
#define to_string_case(o) case (o): return #o

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the order from its name.
 * \param s The name of the order.
 */
int kp::order_on_items::from_string( const std::string& s )
{
  from_string_if(s, order_topological);
  else from_string_if(s, order_max_rank);
  else from_string_if(s, order_min_rank);
  else from_string_if(s, order_sum_rank);
  else from_string_if(s, order_frequency);
  else from_string_if(s, order_random);
  else from_string_if(s, order_domination_rank);
  else from_string_if(s, order_domination_count);
  else
    throw std::range_error("not a valid order");
} // order_on_items::from_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the string representation of an order.
 * \param o the order.
 */
std::string kp::order_on_items::to_string( int o )
{
  switch(o)
    {
      to_string_case(order_topological);
      to_string_case(order_max_rank);
      to_string_case(order_min_rank);
      to_string_case(order_sum_rank);
      to_string_case(order_frequency);
      to_string_case(order_random);
      to_string_case(order_domination_rank);
      to_string_case(order_domination_count);
    default:
      {
        std::ostringstream oss;
        oss << o;
        return oss.str();
      }
    }
} // order_on_items::from_string()

