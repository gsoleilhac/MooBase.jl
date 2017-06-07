/**
 * \file order_on_variables.hpp
 * \brief A view of a knapsack instance on which. This class manages the order
 *        of the variables.
 * \author Julien Jorge
 */
#ifndef __KP_ORDER_ON_VARIABLES_HPP__
#define __KP_ORDER_ON_VARIABLES_HPP__

#include "kp/variable.hpp"
#include <iostream>

namespace kp
{
  /**
   * \brief A view of a knapsack instance on which. This class manages the order
   *        of the variables.
   * \author Julien Jorge
   */
  template<typename Problem>
  class order_on_variables
  {
  public:
    order_on_variables( problem_type& p );

    variable& get_variable( unsigned int i );
    const variable& get_variable( unsigned int i ) const;
    moo::real_type get_capacity() const;
    unsigned int get_size() const;
    unsigned int get_nb_objectives() const;

    void set_capacity( moo::real_type c );
    void save( std::ostream& os ) const;

    unsigned int card_lb() const;
    unsigned int card_ub() const;

  private:
    /** \brief The problem real problem. */
    problem_type& m_problem;

    /** \brief The order on the variables. */
    std::vector<std::size_t> m_order;

  }; // class order_on_variables
} // namespace kp

#endif // __KP_ORDER_ON_VARIABLES_HPP__
