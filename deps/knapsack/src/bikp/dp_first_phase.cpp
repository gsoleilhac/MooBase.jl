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
 * \file dp_first_phase.cpp
 * \brief Implementation of the dp_first_phase class.
 * \author Julien Jorge
 */
#include "bikp/phase/dp_first_phase.hpp"

#include "bikp/concept/minimum_complete_set.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
dp_first_phase::dp_first_phase()
  : base_first_phase("dp")
{

} // dp_first_phase::dp_first_phase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate one optimal solution of a mono-objective problem.
 *
 * \param p Mono problem to solve.
 * \param sol (in/out) The calculated solution.
 * \param lb A lower bound on the solutions.
 */
void dp_first_phase::solve
( const mono_problem& p, mono_solution& sol, real_type lb ) const
{
  assert( sol.is_feasible() );

  mono_solver::region_type search_region(lb, false);

  mono_solver solver(p, search_region);
  solver.run(sol);
} // dp_first_phase::solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate one optimal solution of a linear combination of a
 *        bi-objective problem.
 *
 * \param p Combined problem to solve.
 * \param sol (in/out) The calculated solution.
 * \param lb A lower bound on the solutions.
 */
void dp_first_phase::solve( const combined_problem& p, combined_solution& sol,
			    const combined_profit& lb ) const
{
  assert( sol.is_feasible() );

  combined_mono_solver::region_type search_region(lb, false);

  combined_mono_solver solver(p, search_region);
  solver.run(sol);
} // dp_first_phase::solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the set of solutions by a branch and bound on a linear
 *        combination of a bi-objective problem.
 *
 * \param p Combined problem to solve.
 * \param new_solutions (in/out) The calculated solutions.
 * \param lb A lower bound on the solutions.
 */
void dp_first_phase::solve
( const combined_problem& p, combined_optimal_set_type& new_solutions,
  const combined_profit& lb ) const
{
  combined_solver::region_type search_region(lb, true);

  std::list<combined_solution> sols;

  combined_solver solver(p, search_region);
  solver.run(sols);

  new_solutions.insert( sols.begin(), sols.end() );
} // dp_first_phase::solve()
