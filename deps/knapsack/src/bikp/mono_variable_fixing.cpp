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
 * \file mono_variable_fixing.cpp
 * \brief Implementation of the mono_variable_fixing class.
 * \author Julien Jorge
 */
#include "bikp/reduction/mono_variable_fixing.hpp"
#include "bikp/problem/transform.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param lb The best known solution.
 */
mono_variable_fixing::mono_variable_fixing
( const combined_problem& p, combined_solution& lb )
  : super(p)
{
  optimal_solution_set sols;

  sols.insert( lb );

  do_reduction( p, sols );

  lb = *sols.begin();
} // mono_variable_fixing::mono_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param sols (in/out) The best known solutions.
 * \param keep_equivalent Tell if the reduction must keep equivalent solutions
 *        of the mono objective problem.
 */
mono_variable_fixing::mono_variable_fixing
( const combined_problem& p, optimal_solution_set& sols, bool keep_equivalent )
  : super(p)
{
  optimal_solution_set mono_sols( keep_equivalent );
  mono_sols.insert(sols);

  do_reduction( p, mono_sols );

  sols.clear();
  sols.insert(mono_sols);
} // mono_variable_fixing::mono_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the reduction.
 * \param p The problem to reduce.
 * \param sols (in/out) The best known solutions.
 */
void mono_variable_fixing::do_reduction
( const combined_problem& p, optimal_solution_set& sols )
{
  mono_problem* mono_p = p.convert_to_mono(0);
  optimal_set<mono_solution> mono_s( sols.is_multi_set() );

  transform::create_solutions( *mono_p, sols, mono_s );

  reduction_type reduction(*mono_p, mono_s);

  absorb(reduction);

  sols.clear();
  transform::restore_solutions( *mono_p, p, mono_s, sols );

  delete mono_p;
} // mono_variable_fixing::do_reduction()
