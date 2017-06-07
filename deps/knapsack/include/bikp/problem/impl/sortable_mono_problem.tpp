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
 * \file sortable_mono_problem.tpp
 * \brief Implementation of the sortable_mono_problem class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Reorder the variables of a real solution to create a solution of this
 *        problem.
 * \param real_sol A solution for the multi-objective problem.
 * \param sol (out) The equivalent solution of the current problem.
 * \pre (sol.size() == real_sol.size()) && real_sol.is_feasible()
 */
template<class RealSolution, class Solution>
void sortable_mono_problem::create_solution
( const RealSolution& real_sol, Solution& sol ) const
{
  assert( real_sol.is_feasible() );
  assert( sol.size() == real_sol.size() );

  sol.clear();

  for (unsigned int i=0; i!=real_sol.size(); ++i)
    if ( real_sol.is_set( m_correspondence[i] ) )
      sol.set( i );
    else if ( real_sol.is_unset( m_correspondence[i] ) )
      sol.unset( i );

  assert( sol.is_feasible() );
  assert( real_sol.is_full() ? sol.is_full() : true );
} // sorting_tools::restore_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reorder the variables of a solution to match the order in the
 *        parent problem.
 * \param sol A solution of the current problem.
 * \param real_sol (out) The equivalent solution for the parent problem.
 * \pre (sol.size() == real_sol.size()) && sol.is_feasible()
 */
template<class RealSolution, class Solution>
void sortable_mono_problem::restore_solution
( const Solution& sol, RealSolution& real_sol ) const
{
  assert( sol.is_feasible() );
  assert( sol.size() == real_sol.size() );

  real_sol.clear();

  for (unsigned int i=0; i!=sol.size(); ++i)
    if ( sol.is_set(i) )
      real_sol.set( m_correspondence[i] );
    else if ( sol.is_unset(i) )
      real_sol.unset( m_correspondence[i] );

  assert( real_sol.is_feasible() );
  assert( sol.is_full() ? real_sol.is_full() : true );
} // sorting_tools::restore_solution()
