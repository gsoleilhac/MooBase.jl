/**
 * \file knapsack_solution.hpp
 * \brief A solution (solution vector and value) of a multi objective
 *        unidimensional knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_KNAPSACK_SOLUTION_HPP__
#define __KP_KNAPSACK_SOLUTION_HPP__

#include <kp/solution_value.hpp>
#include <moo/binary_solution.hpp>

namespace kp
{
  /**
   * \brief A solution (solution vector and value) of a multi objective
   *        unidimensional knapsack problem.
   * \author Julien Jorge
   */
  class knapsack_solution
  {
  public:
    typedef cost_vector vector_type;
    typedef cost_compare compare_type;

  public:
    knapsack_solution( const knapsack_solution& that );
    knapsack_solution( unsigned int n, unsigned int p );

    template<typename View>
    knapsack_solution
    ( const View& inst, const moo::binary_solution& s );

    template<typename View>
    knapsack_solution( const View& kp );

    void set( const variable& v );
    void unset( const variable& v );
    void free( const variable& v );

    moo::bin_status status_of( const variable& v ) const;

    const moo::binary_solution& solution() const;
    const solution_value& value() const;
    const vector_type& image() const;

    bool operator==( const knapsack_solution& that ) const;
    bool operator!=( const knapsack_solution& that ) const;
    bool operator<( const knapsack_solution& that ) const;
    bool operator<=( const knapsack_solution& that ) const;
    bool operator>( const knapsack_solution& that ) const;
    bool operator>=( const knapsack_solution& that ) const;

  private:
    /** \brief The solution vector. */
    moo::binary_solution m_solution;

    /** \brief The value of the solution. */
    solution_value m_value;

  }; // class knapsack_solution
} // namespace kp

std::ostream& operator<<( std::ostream& os, const kp::knapsack_solution& sol );

#include <kp/impl/knapsack_solution.tpp>

#endif // __KP_KNAPSACK_SOLUTION_HPP__
