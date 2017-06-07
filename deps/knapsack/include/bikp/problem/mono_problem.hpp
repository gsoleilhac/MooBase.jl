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
 * \file mono_problem.hpp
 * \brief Multi-objective problem reduced to one of its objectives.
 * \author Julien Jorge
 */
#ifndef __BIKP_MONO_PROBLEM_HPP__
#define __BIKP_MONO_PROBLEM_HPP__

#include "bikp/problem/sortable_mono_problem.hpp"
#include "bikp/problem/solution.hpp"

/**
 * \brief A knapsack problem with only one objective.
 * \author Julien Jorge
 */
class mono_problem
  : public sortable_mono_problem
{
public:
  /** \brief The type of the current class. */
  typedef mono_problem self_type;

  /** \brief The type of the profits. */
  typedef real_type profit_type;

  /** \brief The type of the weights. */
  typedef real_type weight_type;

  /** \brief The type of the solution for this problem. */
  typedef solution<self_type> solution_type;

  /** \brief Basic operations on profits. */
  typedef profit_traits<profit_type> profit_traits_type;

private:
  /** \brief The type of the parent class. */
  typedef sortable_mono_problem super;

public:
  mono_problem( unsigned int n, weight_type cap );

  void set_variable( unsigned int i, const profit_type& p, weight_type w );

  /**
   * \brief Get the profit of a variable.
   * \param index The index of the variable from which we want the profit.
   */
  inline profit_type profit( unsigned int index ) const
  {
    assert ( index < size() );
    return m_profit[ this->m_correspondence[index] ];
  } // profit()

  /**
   * \brief Get the weight of a variable.
   * \param index The index of the variable from which we want the weight.
   */
  inline weight_type weight( unsigned int index ) const
  {
    assert( index < size() );
    return m_weight[ this->m_correspondence[index] ];
  } // weight()

  unsigned int size() const;
  weight_type capacity() const;

  /**
   * \brief Get the efficiency of a variable.
   * \param obj Must be zero.
   * \param index The index of the variable.
   */
  inline real_type get_efficiency( unsigned int obj, unsigned int index ) const
  {
    assert( obj == 0 );
    return m_efficiency[ this->m_correspondence[index] ];
  }

  unsigned int local_index( unsigned int index ) const;

  void sort_by_decreasing_efficiency();

private:
  
  /** \brief The profits of the variables. */
  std::vector<profit_type> m_profit;

  /** \brief The weight of the variables. */
  std::vector<weight_type> m_weight;

  const weight_type m_capacity;

  /** \brief Correspondence : real index -> local index. */
  std::vector<std::size_t> m_local_index;

}; // class mono_problem

/** \brief The type of the solution for this problem. */
typedef solution<mono_problem> mono_solution;

/** \brief Basic operations on profits. */
typedef profit_traits<real_type> mono_profit_traits;

#endif // __BIKP_MONO_PROBLEM_HPP__

