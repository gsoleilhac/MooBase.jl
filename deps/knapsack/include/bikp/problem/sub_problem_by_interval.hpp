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
 * \file sub_problem_by_interval.hpp
 * \brief A sub problem by interval is a problem reduced to a continuous
 *        subset of its variables.
 * \author Julien Jorge
 */
#ifndef __BIKP_SUB_PROBLEM_BY_INTERVAL_HPP__
#define __BIKP_SUB_PROBLEM_BY_INTERVAL_HPP__

#include "bikp/problem/solution.hpp"

/**
 * \brief A sub problem by interval is a problem reduced to a continuous
 *        subset of its variables.
 *
 * \b Template \b arguments :
 * - \a Problem : The type of the parent problem.
 *
 * \author Julien Jorge
 */
template<typename Problem>
class sub_problem_by_interval 
{
public:
  /** \brief The type of the parent problem. */
  typedef Problem problem_type;

  /** \brief The type of the profits. */
  typedef typename problem_type::profit_type profit_type;

  /** \brief The type of the weights. */
  typedef typename problem_type::weight_type weight_type;

  /** \brief The type of the current class. */
  typedef sub_problem_by_interval<problem_type> self_type;

  /** \brief The type of the solution for this problem. */
  typedef solution<self_type> solution_type;

public:
  sub_problem_by_interval
  ( const problem_type& p, unsigned int first, unsigned int last,
    weight_type cap );

  profit_type profit( unsigned int index ) const;
  weight_type weight( unsigned int index ) const;

  /**
   * \brief Get the efficiency of a variable.
   * \param obj The objective on which you want the efficiency.
   * \param index The index of the variable.
   */
  inline real_type get_efficiency( unsigned int obj, unsigned int index ) const
  {
    assert( size() > 0 );
    assert( index < size() );

    return m_problem.get_efficiency( obj, m_first + index );
  }

  unsigned int size() const;
  weight_type capacity() const;

private:
  /** \brief The parent problem. */
  const problem_type& m_problem;

  /** \brief Index of the first variable. */
  const unsigned int m_first;

  /** \brief Index of the last variable. */
  const unsigned int m_last;

  /** \brief Remaining capacity. */
  const weight_type m_capacity;

}; // class sub_problem_by_interval

#include "bikp/problem/impl/sub_problem_by_interval.tpp"

#endif // __BIKP_SUB_PROBLEM_BY_INTERVAL_HPP__
