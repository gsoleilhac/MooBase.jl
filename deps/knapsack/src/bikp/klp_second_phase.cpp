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
 * \file klp_second_phase.cpp
 * \brief Implementation of the klp_second_phase class.
 * \author Julien Jorge
 */
#include "bikp/phase/klp_second_phase.hpp"

#include "bikp/reduction/simple_variable_fixing.hpp"

#include "bikp/concept/minimum_complete_set.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
klp_second_phase::klp_second_phase()
  : base_second_phase("klp")
{

} // klp_second_phase::klp_second_phase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the triangles in the order you want to explore them.
 * \param triangles The triangles to order.
 */
void klp_second_phase::pre_sort( std::vector<triangle>& triangles ) const
{
  std::sort( triangles.begin(), triangles.end(), area_lesser() );
} // klp_second_phase::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate the reduction algorithm to use.
 * \param p The problem to reduce.
 * \param z_left Top left point of the current triangle.
 * \param z_right Bottom right point of the current triangle.
 * \param lb_lambda Lower bound on the combined objective.
 */
base_second_phase::combined_reduction_type*
klp_second_phase::reduce
( const combined_problem& p, const combined_profit& z_left,
  const combined_profit& z_right, real_type lb_lambda ) const
{
  // do the reduction on the combined objective only
  mono_problem* one_objective = p.convert_to_mono(0);

  simple_variable_fixing s_red( *one_objective, lb_lambda );
  delete one_objective;

  // then insert the fixed variables into the wanted reduction
  combined_reduction_type* reduction = new combined_reduction_type( p );
  reduction->absorb( s_red );

  return reduction;
} // klp_second_phase::reduce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the set of solutions.
 * \param p Combined problem to solve.
 * \param region The region where the efficient solutions are.
 * \param sols (in/out) The calculated solutions, in the triangle.
 * \param out_sols (in/out) The calculated solutions, outside the triangle.
 */
void klp_second_phase::solve
( const combined_problem& p, combined_triangle_front& region,
  std::list<combined_solution>& sols,
  std::list<combined_profit>& out_sols ) const
{
  solver_type solver( p, region, sols, out_sols );
} // klp_second_phase::solve()
