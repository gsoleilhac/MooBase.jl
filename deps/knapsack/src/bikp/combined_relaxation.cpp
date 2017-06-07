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
 * \file combined_relaxation.cpp
 * \brief Implementation of the combined_relaxation class.
 * \author Julien Jorge
 */
#include "bikp/relaxation/combined_relaxation.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The combined problem.
 * \pre p is sorted by decreasing efficiency.
 */
combined_relaxation::combined_relaxation( const problem_type& p )
  : m_mono( p.convert_to_mono(0) ), m_mono_1( p.convert_to_mono(1) ),
    m_mono_2( p.convert_to_mono(2) ), m_sub_1( m_mono_1->size() + 1),
    m_sub_2( m_mono_2->size() + 1 )
{
  m_mono_1->sort_by_decreasing_efficiency();
  m_mono_2->sort_by_decreasing_efficiency();

  sub_problem::set_of_variables_type wanted;

  // sub problems on the first objective
  unsigned int i = m_sub_1.size() - 1;
  m_sub_1[i] = new sub_problem( *m_mono_1, wanted, true );

  do
    {
      --i;

      wanted.insert( m_mono_1->local_index(i) );
      m_sub_1[i] = new sub_problem( *m_mono_1, wanted, true );
    }
  while (i);

  wanted.clear();

  // sub problems on the second objective
  i = m_sub_2.size() - 1;
  m_sub_2[i] = new sub_problem( *m_mono_2, wanted, true );

  do
    {
      --i;

      wanted.insert( m_mono_2->local_index(i) );
      m_sub_2[i] = new sub_problem( *m_mono_2, wanted, true );
    }
  while (i);
} // combined_relaxation::combined_relaxation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
combined_relaxation::~combined_relaxation()
{
  delete m_mono_2;
  delete m_mono_1;
  delete m_mono;

  for (unsigned int i=0; i!=m_sub_1.size(); ++i)
    delete m_sub_1[i];

  for (unsigned int i=0; i!=m_sub_2.size(); ++i)
    delete m_sub_2[i];
} // combined_relaxation::~combined_relaxation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the relaxation is greater than a lower bound on each
 *        objective.
 * \param profit The profit that we already have.
 * \param bound The bound to reach.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
bool combined_relaxation::interesting
( const profit_type& profit, const profit_type& bound, unsigned int from_var,
  weight_type cap ) const
{
  bool result = false;

  if ( relax_lambda( profit.lambda, from_var, cap ) >= bound.lambda )
    if ( relax_z1( profit.profit_1, from_var, cap ) >= bound.profit_1 )
      if ( relax_z2( profit.profit_2, from_var, cap ) >= bound.profit_2 )
	result = true;

  return result;
} // combined_relaxation::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation on the combined objective.
 * \param profit The profit that we already have.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
real_type combined_relaxation::relax_lambda
( real_type profit, unsigned int from_var, weight_type cap ) const
{
  relaxation_type lr;
  return lr.calculate_from(*m_mono, from_var, profit, cap).objective_value;
} // combined_relaxation::relax_lambda()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation on the first objective.
 * \param profit The profit that we already have.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
real_type combined_relaxation::relax_z1
( real_type profit, unsigned int from_var, weight_type cap ) const
{
  sub_relaxation_type lr;
  m_sub_1[from_var]->set_capacity(cap);

  return lr.calculate( *m_sub_1[from_var] ).objective_value + profit;
} // combined_relaxation::relax_z1()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation on the combined objective.
 * \param profit The profit that we already have.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
real_type combined_relaxation::relax_z2
( real_type profit, unsigned int from_var, weight_type cap ) const
{
  sub_relaxation_type lr;
  m_sub_2[from_var]->set_capacity(cap);

  return lr.calculate( *m_sub_2[from_var] ).objective_value + profit;
} // combined_relaxation::relax_z2()

