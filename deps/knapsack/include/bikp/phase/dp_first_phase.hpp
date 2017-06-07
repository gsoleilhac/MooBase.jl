/*
  Copyright FacultÃ© des Sciences et Techniques de Nantes
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
 * \file dp_first_phase.hpp
 * \brief First phase of the Visée et al. algorithm "Two-phases Method and
 *        Branch and Bound" : calculate supported solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_DP_FIRST_PHASE_HPP__
#define __BIKP_DP_FIRST_PHASE_HPP__

#include "bikp/phase/base_first_phase.hpp"
#include "bikp/solver/dynamic_programming_single.hpp"
#include "bikp/solver/dynamic_programming_multiple.hpp"
#include "bikp/concept/minimum_complete_set.hpp"

/**
 * \brief First phase : calculated suported solutions.
 *
 * This class use the dynamic programming approach.
 * \author Julien Jorge
 */
class dp_first_phase : public base_first_phase
{
private:
  /** \brief The algorithm to use to solve a mono problem. */
  typedef dynamic_programming_single<mono_problem> mono_solver;

  /** \brief The algorithm to use to solve a combined problem seen as a mono
      problem. */
  typedef dynamic_programming_single<combined_problem> combined_mono_solver;

  /** \brief The algorithm to use to solve a combined problem. */
  typedef dynamic_programming_multiple combined_solver;

public:
  dp_first_phase();

private:
  void
  solve( const mono_problem& p, mono_solution& sol, real_type lb ) const;

  void solve( const combined_problem& p, combined_solution& sol,
	      const combined_profit& lb ) const;

  void solve( const combined_problem& p, combined_optimal_set_type& sols,
	      const combined_profit& lb ) const;

}; // class dp_first_phase

#endif // __BIKP_DP_FIRST_PHASE_HPP__
