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
 * \file sortable_mono_problem.hpp
 * \brief Parent class for mono-objective problems for which we want to be able
 *        to sort items by efficiency.
 * \author Julien Jorge
 */
#ifndef __BIKP_SORTABLE_MONO_PROBLEM_HPP__
#define __BIKP_SORTABLE_MONO_PROBLEM_HPP__

#include <vector>
#include "bikp/tool/types.hpp"

/**
 * \brief Parent class for mono-objective problems for which we want to be able
 *        to sort items by efficiency.
 * \author Julien Jorge
 */
class sortable_mono_problem
{
public:
  sortable_mono_problem( const unsigned int size );

  unsigned int real_index( unsigned int index ) const;

  template<class RealSolution, class Solution>
  void create_solution( const RealSolution& real_sol, Solution& sol ) const;

  template<class RealSolution, class Solution>
  void restore_solution( const Solution& sol, RealSolution& real_sol ) const;

  void sort_by_decreasing_efficiency();

protected:
  /** \brief Correspondence of the index of the variables between the current
      sort and the parent problem sort. */
  std::vector<std::size_t> m_correspondence;

  /** \brief Efficiency of the variables. */
  std::vector<real_type> m_efficiency;

}; // class sortable_mono_problem

#include "bikp/problem/impl/sortable_mono_problem.tpp"

#endif // __BIKP_SORTABLE_MONO_PROBLEM_HPP__
