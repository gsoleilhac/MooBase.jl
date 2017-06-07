/*
  Copyright Faculté des Sciences et Techniques de Nantes
  contributor:
    Julien Jorge <julien.jorge@univ-nantes.fr>

  This software is a computer program whose purpose is to exactly solve
  instances of the bi-objective unidimensional 0-1 knapsack problem.

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 
*/
/**
 * \file solution.hpp
 * \brief A solution for a problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_SOLUTION_HPP__
#define __BIKP_SOLUTION_HPP__

#include <vector>
#include "bikp/concept/profit_traits.hpp"

/**
 * \brief A solution for a problem.
 *
 * \b Template \b arguments :
 * - \a Problem : The type of the problems for which it is a solution.
 *
 * \author Julien Jorge
 */
template<typename Problem>
class solution
{
public:
  /** \brief The type of the problem for which it is a solution. */
  typedef Problem problem_type;

  /** \brief The type of the objective value. */
  typedef typename problem_type::profit_type profit_type;

  /** \brief The type of the weight of the solution. */
  typedef typename problem_type::weight_type weight_type;

  /** \brief Basic operations on objective value. */
  typedef typename problem_type::profit_traits_type profit_traits_type;

private:
  /** \brief The type of the current class. */
  typedef solution<problem_type> self_type;

  /** \brief Status of a variable. */
  enum variable_status
    {
      /** \brief Variable is free. */
      var_free,

      /** \brief Variable is in the solution. */
      var_set,

      /** \brief Variable is out of the solution. */
      var_unset

    }; // enum variable_status

public:
  solution();
  solution( const problem_type& p );

  //self_type& operator=( const self_type& that );
  bool operator==( const self_type& that ) const;
  bool operator!=( const self_type& that ) const;
  bool operator<( const self_type& that ) const;

  const profit_type& objective_value() const;
  operator const profit_type&() const;
  const weight_type& weight() const;

  void set( unsigned int index );
  void unset( unsigned int index );
  void free( unsigned int index );

  void unsafe_set_objective_value( const profit_type& v );

  void clear();

  bool is_set( unsigned int index ) const;
  bool is_unset( unsigned int index ) const;
  bool is_free( unsigned int index ) const;

  unsigned int size() const;

  bool is_feasible() const;
  bool is_full() const;

private:
  /** \brief The problem for which we are a solution. */
  const problem_type* m_problem;

  /** \brief The status of the variables. */
  std::vector<variable_status> m_variables;

  /** \brief Current value of the objective function. */
  profit_type m_objective_value;

  /** \brief Current weight of the solution. */
  weight_type m_weight;

}; // class solution

#include "bikp/problem/impl/solution.tpp"

#endif // __BIKP_SOLUTION_HPP__
