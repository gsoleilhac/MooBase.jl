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
 * \file maximum_complete_set.hpp
 * \brief A maximum complete set of solutions (ie. all solutions corresponging
 *        to for each not dominated point found in the objective space).
 * \author Julien Jorge
 */
#ifndef __BIKP_MAXIMUM_COMPLETE_SET_HPP__
#define __BIKP_MAXIMUM_COMPLETE_SET_HPP__

#include "bikp/concept/combined_maximum_complete_set.hpp"
#include "bikp/concept/mono_maximum_complete_set.hpp"

/**
 * \brief A maximum complete set of solutions (ie. all solutions corresponging
 *        to for each not dominated point found in the objective space).
 * \author Julien Jorge
 */
template< class Solution, class SolutionTraits = solution_traits<Solution> >
class maximum_complete_set :
  public combined_maximum_complete_set<Solution, SolutionTraits>
{

}; // class maximum_complete_set



/**
 * \brief A maximum complete set of solutions (ie. all solutions corresponging
 *        to for each not dominated point found in the objective space),
 *        specialized for combined_solution.
 * \author Julien Jorge
 */
/*template<>
class maximum_complete_set< combined_solution,
			    solution_traits<combined_solution> > :
  public combined_maximum_complete_set
{

};*/ // class maximum_complete_set<combined_solution>

/**
 * \brief A maximum complete set of solutions (ie. all solutions corresponging
 *        to for each not dominated point found in the objective space),
 *        specialized for mono_solution.
 * \author Julien Jorge
 */
template<>
class maximum_complete_set< mono_solution, solution_traits<mono_solution> > :
  public mono_maximum_complete_set< mono_solution,
				    solution_traits<mono_solution> >
{

}; // class maximum_complete_set<mono_solution>

#endif // __BIKP_MAXIMUM_COMPLETE_SET_HPP__
