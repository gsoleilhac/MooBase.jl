/**
 * \file knapsack_instance.hpp
 * \brief An instance of a multi-objective unidimensional knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_KNAPSACK_INSTANCE_HPP__
#define __KP_KNAPSACK_INSTANCE_HPP__

#include "kp/variable.hpp"
#include <iostream>

namespace kp
{
  class knapsack_solution;

  /**
   * \brief An instance of a multi-objective unidimensional knapsack problem.
   * \author Julien Jorge
   */
  class knapsack_instance
  {
  public:
    knapsack_instance( std::istream& f );
    knapsack_instance( int nb_obj, int sz, float *obj, float *weigths ,float capacity);
    knapsack_instance( unsigned int n, unsigned int p, moo::real_type w );

    variable& get_variable( unsigned int i );
    const variable& get_variable( unsigned int i ) const;
    moo::real_type get_capacity() const;
    unsigned int get_size() const;
    unsigned int get_nb_objectives() const;

    void set_capacity( moo::real_type c );
    void save( std::ostream& os ) const;

    unsigned int card_lb() const;
    unsigned int card_ub() const;

    bool check( const knapsack_solution& sol ) const;

  private:
    /** \brief The variables of the problem. */
    std::vector<variable> m_variables;

    /** \brief The total capcity of the knapsack. */
    moo::real_type m_capacity;

    /** \brief The number of objectives. */
    unsigned int m_nb_objectives;

  }; // class knapsack_instance
} // namespace kp

#endif // __KP_KNAPSACK_INSTANCE_HPP__
