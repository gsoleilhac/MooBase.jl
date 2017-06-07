/**
 * \file variable.hpp
 * \brief A variable in a knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_VARIABLE_HPP__
#define __KP_VARIABLE_HPP__

#include "kp/cost_vector.hpp"

namespace kp
{
  /**
   * \brief A variable in a knapsack problem.
   * \author Julien Jorge
   */
  class variable
  {
  public:
    /**
     * \brief Function object to get the weight of a variable.
     */
    struct weight_func
    {
      moo::real_type operator()( const variable& v ) const;
    }; // struct weight_func

  public:
    variable( unsigned int i, const cost_vector& c, moo::real_type w );

    moo::vector<moo::real_type> efficiency() const;

  public:
    /** \brief The index of the variable in the problem. */
    unsigned int index;

    /** \brief The costs associated with this variable. */
    cost_vector cost;

    /** \brief The weight of the variable. */
    moo::real_type weight;

  }; // class variable
} // namespace kp

std::ostream& operator<<( std::ostream& os, const kp::variable& v );

#endif // __KP_VARIABLE_HPP__
