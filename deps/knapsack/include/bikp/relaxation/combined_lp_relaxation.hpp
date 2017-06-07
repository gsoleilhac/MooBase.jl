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
 * \file combined_lp_relaxation.hpp
 * \brief The combined_lp_relaxation class compute an upper bound on the
 *        combined objective of a combined_problem using a linear solver.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_LP_RELAXATION_HPP__
#define __BIKP_COMBINED_LP_RELAXATION_HPP__

#include "lpx.h"
#include "bikp/relaxation/combined_relaxation.hpp"

extern "C" {
	#include <glpk.h>
}


/**
 * \brief The combined_lp_relaxation class compute an upper bound on the
 *        combined objective of a combined_problem using a linear solver.
 *
 * Two constraints are added to the initial knapsack problem to assure that the
 * relaxed solution is inside the triangle.
 *
 * \pre Problem is combined_problem.
 *
 * \author Julien Jorge
 */
class combined_lp_relaxation : public combined_relaxation
{
public:
  /** \brief The type of the parent class. */
  typedef combined_relaxation super;

public:
  combined_lp_relaxation( const problem_type& p );
  ~combined_lp_relaxation();

  bool interesting( const profit_type& profit, const profit_type& bound,
		    unsigned int from_var, weight_type cap ) const;

private:
  real_type lp_relax( const profit_type& profit, const profit_type& bound,
			 unsigned int from_var, weight_type cap ) const;

  LPX* create_problem
  ( unsigned int n, double* capacity_coeff, double* z1_coeff, double* z2_coeff,
    double* lambda_coeff, int* index ) const;

private:
  /** \brief The linear programs. For a given variable index i,
      m_linear_programs[i] is the program to use for the relaxation. Only the
      capacity constraint and the bounds on objectives z1 and z2 have to be
      updated. */
  std::vector<LPX*> m_linear_program;

}; // combined_lp_relaxation

#endif // __BIKP_COMBINED_LP_RELAXATION_HPP__
