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
 * \file minimum_complete_set.hpp
 * \brief A set of non-dominated solutions, without equivalent ones.
 * \author Julien Jorge
 */
#ifndef __BIKP_MINIMUM_COMPLETE_SET_HPP__
#define __BIKP_MINIMUM_COMPLETE_SET_HPP__

#include "bikp/concept/solution_traits.hpp"

#include "bikp/concept/bi_minimum_complete_set.hpp"
#include "bikp/concept/mono_minimum_complete_set.hpp"
#include "bikp/problem/mono_problem.hpp"

/**
 * \brief A set of non-dominated solutions, without equivalent ones.
 *
 * \b Template \b arguments :
 * - \a Solution : The type of the stored solutions.
 * - \a SolutionTraits : The type of the traits on the solutions.
 *
 * \author Julien Jorge
 */
template <class Solution, class SolutionTraits = solution_traits<Solution> >
class minimum_complete_set :
  public bi_minimum_complete_set<Solution, SolutionTraits>
{

}; // minimum_complete_set

/**
 * \brief A set of non-dominated solutions, without equivalent ones,
 *        specialized for combined_solution.
 * \author Julien Jorge
 */
/*template<>
class minimum_complete_set< combined_solution,
			    solution_traits<combined_solution> > :
  public bi_minimum_complete_set< combined_solution,
				  solution_traits<combined_solution> >
{

};*/ // class minimum_complete_set<combined_solution>

/**
 * \brief A set of non-dominated solutions, without equivalent ones,
 *        specialized for real_typeegers.
 * \author Julien Jorge
 */
template<>
class minimum_complete_set< mono_solution,
			    solution_traits<mono_solution> > :
  public mono_minimum_complete_set< mono_solution,
				    solution_traits<mono_solution> >
{

}; // class minimum_complete_set<mono_solution>

#endif // __BIKP_MINIMUM_COMPLETE_SET_HPP__
