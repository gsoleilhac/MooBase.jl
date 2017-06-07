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
 * \file klp_second_phase.hpp
 * \brief Use a k-shortest-paths algorithm to find all not supported solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_KLP_SECOND_PHASE_HPP__
#define __BIKP_KLP_SECOND_PHASE_HPP__

#include "bikp/phase/base_second_phase.hpp"
#include "bikp/solver/k_longest_paths.hpp"

/**
 * \brief Use a k-shortest-paths algorithm to find all not supported solutions.
 * \author Julien Jorge
 */
class klp_second_phase : public base_second_phase
{
public:
  /** \brief The type of the k-shortest-paths algorithm. */
  typedef k_longest_paths solver_type;

public:
  klp_second_phase();

private:
  void pre_sort( std::vector<triangle>& triangles ) const;

  combined_reduction_type* reduce
  ( const combined_problem& p, const combined_profit& z_left,
    const combined_profit& z_right, real_type lb_lambda ) const;

  void solve( const combined_problem& p, combined_triangle_front& region,
	      std::list<combined_solution>& sols,
	      std::list<combined_profit>& out_sols ) const;

};  // class klp_second_phase

#endif // __BIKP_KLP_SECOND_PHASE_HPP__
