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
 * \file combined_variable_fixing.cpp
 * \brief Implementation of the combined_variable_fixing class.
 * \author Julien Jorge
 */
#include "bikp/reduction/combined_variable_fixing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The sorted mono objective problem from which we get the variables.
 */
combined_variable_fixing::real_index::real_index
( const sortable_mono_problem& p )
  : m_problem(p)
{

} // combined_variable_fixing::real_index::real_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the real index of a variable.
 * \param index The index in the sorted problem.
 */
unsigned int
combined_variable_fixing::real_index::operator()( unsigned int index ) const
{
  return m_problem.real_index( index );
} // combined_variable_fixing::real_index::operator()()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param x1 Top-left solution of the triangle.
 * \param x2 Bottom-right solution of the triangle.
 * \param lb_lambda A lower bound of the combined problem.
 */
combined_variable_fixing::combined_variable_fixing
( const combined_problem& p, const combined_solution& x1,
  const combined_solution& x2, const real_type lb_lambda )
  : super(p)
{
  assert( x1.is_feasible() );
  assert( x2.is_feasible() );

  mono_problem* mono_lambda = p.convert_to_mono(0);
  mono_problem* mono_1 = p.convert_to_mono(1);
  mono_problem* mono_2 = p.convert_to_mono(2);

  mono_1->sort_by_decreasing_efficiency();
  mono_2->sort_by_decreasing_efficiency();

  do_reduction( *mono_1, *mono_2, *mono_lambda, x1, x2, lb_lambda );

  delete mono_2;
  delete mono_1;
  delete mono_lambda;
} // combined_variable_fixing::combined_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the reduction.
 * \param mono_1 The bi-objective problem reduced to its first objective.
 * \param mono_2 The bi-objective problem reduced to its second objective.
 * \param mono_lambda A linear combination of the bi-objective problem.
 * \param x1 Top-left solution of the triangle.
 * \param x2 Bottom-right solution of the triangle.
 * \param lb_lambda A lower bound on the combined objective.
 */
void combined_variable_fixing::do_reduction
( const mono_problem& mono_1, const mono_problem& mono_2,
  const mono_problem& mono_lambda, const combined_solution& x1,
  const combined_solution& x2, const real_type lb_lambda )
{
  assert( x1.is_feasible() );
  assert( x2.is_feasible() );
  assert( x1.objective_value().profit_1 < x2.objective_value().profit_1 );
  assert( x1.objective_value().profit_2 > x2.objective_value().profit_2 );

  reduction_type reduction_1(mono_1, x1.objective_value().profit_1 + 1);
  reduction_type reduction_2(mono_2, x2.objective_value().profit_2 + 1);
  reduction_type reduction_lambda(mono_lambda, lb_lambda);

  fixed_set_type variables_1, variables_2, variables_lambda;

  std::transform( reduction_1.set_variables().begin(),
		  reduction_1.set_variables().end(),
		  insert_iterator(variables_1, variables_1.begin()), 
		  real_index( mono_1 ) );

  std::transform( reduction_2.set_variables().begin(),
		  reduction_2.set_variables().end(),
		  insert_iterator(variables_2, variables_2.begin()),
		  real_index( mono_2 ) );

  std::transform( reduction_lambda.set_variables().begin(),
		  reduction_lambda.set_variables().end(),
		  insert_iterator(variables_lambda, variables_lambda.begin()),
		  real_index( mono_lambda ) );

  create_set_set( variables_1, variables_2, variables_lambda );
  
  variables_1.clear();
  variables_2.clear();
  variables_lambda.clear();

  std::transform( reduction_1.unset_variables().begin(),
		  reduction_1.unset_variables().end(),
		  insert_iterator(variables_1, variables_1.begin()), 
		  real_index( mono_1 ) );

  std::transform( reduction_2.unset_variables().begin(),
		  reduction_2.unset_variables().end(),
		  insert_iterator(variables_2, variables_2.begin()),
		  real_index( mono_2 ) );

  std::transform( reduction_lambda.unset_variables().begin(),
		  reduction_lambda.unset_variables().end(),
		  insert_iterator(variables_lambda, variables_lambda.begin()),
		  real_index( mono_lambda ) );

  create_unset_set( variables_1, variables_2, variables_lambda );
  
  this->create_sub_problem();
} // combined_variable_fixing::do_reduction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the set of variables fixed in the solution.
 * \param variables_1 Variables fixed on the first objective.
 * \param variables_2 Variables fixed on the second objective.
 * \param variables_lambda Variables fixed on the combined problem.
 */
void combined_variable_fixing::create_set_set
( const fixed_set_type& variables_1, const fixed_set_type& variables_2,
  const fixed_set_type& variables_lambda )
{
  fixed_set_type tmp_set;

  std::set_union( variables_1.begin(), variables_1.end(),
		  variables_2.begin(), variables_2.end(),
		  insert_iterator(tmp_set, tmp_set.begin()) );

  std::set_union( tmp_set.begin(), tmp_set.end(),
		  variables_lambda.begin(), variables_lambda.end(),
		  insert_iterator(this->m_set_variables,
				  this->m_set_variables.begin()) );
} // combined_variable_fixing::create_set_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the set of variables fixed out of the solution.
 * \param variables_1 Variables fixed on the first objective.
 * \param variables_2 Variables fixed on the second objective.
 * \param variables_lambda Variables fixed on the combined problem.
 */
void combined_variable_fixing::create_unset_set
( const fixed_set_type& variables_1, const fixed_set_type& variables_2,
  const fixed_set_type& variables_lambda )
{
  fixed_set_type tmp_set;

  std::set_union( variables_1.begin(), variables_1.end(),
		  variables_2.begin(), variables_2.end(),
		  insert_iterator(tmp_set, tmp_set.begin()) );

  std::set_union( tmp_set.begin(), tmp_set.end(),
		  variables_lambda.begin(), variables_lambda.end(),
		  insert_iterator(this->m_unset_variables,
				  this->m_unset_variables.begin()) );
} // combined_variable_fixing::create_unset_set()

