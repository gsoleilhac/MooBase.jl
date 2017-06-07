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
 * \file solver_framework.hpp
 * \brief A meta-class and some macros to pass the algorithms to the solvers.
 * \author Julien Jorge
 */
#ifndef __BIKP_SOLVER_FRAMEWORK__
#define __BIKP_SOLVER_FRAMEWORK__

template<class Problem, class Solver, class Region, class Relax>
class solver_framework
{
public:
  typedef Problem problem_type;
  typedef Solver solver_type;
  typedef Region region_type;
  typedef Relax relaxation_type;
}; // class solver_framework

#define SOLVER_FRAMEWORK( Mono, Solver, Region, Relax )	       \
  solver_framework< Mono,				       \
		   Solver< Mono,			       \
			   Region<Mono::profit_traits_type>,   \
			   Relax<Mono> >,		       \
		   Region<Mono::profit_traits_type>,	       \
		   Relax<Mono> >

#define TEMPLATE_SOLVER_FRAMEWORK( Mono, Solver, Region, Relax )		\
  solver_framework< Mono,						\
		   Solver< Mono,					\
			   Region<typename Mono::profit_traits_type>,	\
			   Relax<Mono> >,				\
		   Region<typename Mono::profit_traits_type>,		\
		   Relax<Mono> >

#endif // __BIKP_SOLVER_FRAMEWORK__
