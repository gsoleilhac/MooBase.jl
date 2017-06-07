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
 * \file dynamic_programming_multiple.hpp
 * \brief Solve a knapsack problem using the dynamic programming approach.
 * \author Julien Jorge.
 */
#ifndef __BIKP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__
#define __BIKP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__

#include "bikp/solver/dynamic_programming_single.hpp"

#include <list>
#include "bikp/problem/combined_problem.hpp"

/**
 * \brief Solve a knapsack problem using the dynamic programming approach.
 * \author Julien Jorge.
 */
class dynamic_programming_multiple :
  public dynamic_programming_single<combined_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef dynamic_programming_single<combined_problem> super;

public:
  dynamic_programming_multiple
  ( const problem_type& p, const region_type& region );

  void run( std::list<solution_type>& sols ) const;

private:
  void build_all_solutions
  ( std::list<solution_type>& sols, unsigned int index ) const;

  void build_all_solutions_from
  ( std::list<solution_type>& sols, solution_type& sol,
    const vertex_type& v ) const;

}; // class dynamic_programming_multiple

#endif // __BIKP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__
