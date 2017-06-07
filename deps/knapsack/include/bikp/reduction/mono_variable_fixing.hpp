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
 * \file mono_variable_fixing.hpp
 * \brief Variable reduction applyable to a combined_problem seen as a
 *        mono_problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_MONO_VARIABLE_FIXING_HPP__
#define __BIKP_MONO_VARIABLE_FIXING_HPP__

#include "bikp/reduction/mtr_variable_fixing.hpp"
#include "bikp/problem/combined_problem.hpp"

/**
 * \brief Variable reduction on the combined objective of a combined_problem.
 * \author Julien Jorge
 */
class mono_variable_fixing
  : public base_variable_fixing<combined_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<combined_problem> super;

  /** \brief A set of optimal solutions. */
  typedef optimal_set<combined_solution> optimal_solution_set;

private:
  /** \brief The variable reduction to apply to the mono objective problem. */
  typedef mtr_variable_fixing reduction_type;

public:
  mono_variable_fixing( const combined_problem& p, combined_solution& lb );
  mono_variable_fixing( const combined_problem& p, optimal_solution_set& sols,
			bool keep_equivalent );

private:
  void
  do_reduction( const combined_problem& p, optimal_solution_set& sols );

}; // class mono_variable_fixing

#endif // __BIKP_MONO_VARIABLE_FIXING_HPP__
