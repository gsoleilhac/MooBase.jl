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
 * \file mono_relaxation.tpp
 * \brief Implementation of the mono_relaxation class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The mono problem.
 * \pre p is sorted by decreasing efficiency.
 */
template<typename Problem>
mono_relaxation<Problem>::mono_relaxation( const problem_type& p )
  : m_mono(p)
{

} // mono_relaxation::mono_relaxation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the relaxation is greater than a lower bound.
 * \param profit The profit that we already have.
 * \param bound The bound to reach.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
template<typename Problem>
bool mono_relaxation<Problem>::interesting
( const profit_type& profit, const profit_type& bound, unsigned int from_var,
  weight_type cap ) const
{
  return relax( profit, from_var, cap ) >= bound;
} // mono_relaxation::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation on the mono objective.
 * \param profit The profit that we already have.
 * \param from_var The index of the first var to take in account.
 * \param cap The remaining capacity of the problem.
 */
template<typename Problem>
real_type
mono_relaxation<Problem>::relax
( real_type profit, unsigned int from_var, weight_type cap ) const
{
  mono_relax lr;
  return lr.calculate_from(m_mono, from_var, profit, cap).objective_value;
} // mono_relaxation::relax()

