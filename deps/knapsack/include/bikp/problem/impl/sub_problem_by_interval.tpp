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
 * \file sub_problem_by_interval.tpp
 * \brief Implementation of the sub_problem_by_interval class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The parent problem.
 * \param first Index of the first variable.
 * \param last Index of the last variable.
 * \param cap Maximum capacity.
 */
template<typename Problem>
sub_problem_by_interval<Problem>::sub_problem_by_interval
( const problem_type& p, unsigned int first, unsigned int last,
  weight_type cap )
  : m_problem(p), m_first(first), m_last(last), m_capacity(cap)
{
  assert( cap >= 0 );
} // sub_problem_by_interval::sub_problem_by_interval()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the profit of a variable.
 * \param index The index of the variable.
 */
template<typename Problem>
typename sub_problem_by_interval<Problem>::profit_type
sub_problem_by_interval<Problem>::profit( unsigned int index ) const
{
  assert( size() > 0 );
  assert( index < size() );

  return m_problem.profit( m_first + index );
} // sub_problem_by_interval::profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the weight of a variable.
 * \param index The index of the variable.
 */
template<typename Problem>
typename sub_problem_by_interval<Problem>::weight_type
sub_problem_by_interval<Problem>::weight( unsigned int index ) const
{
  assert( size() > 0 );
  assert( index < size() );

  return m_problem.weight( m_first + index );
} // sub_problem_by_interval::weight()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of variables.
 */
template<typename Problem>
unsigned int sub_problem_by_interval<Problem>::size() const
{
  return m_last - m_first + 1;
} // sub_problem_by_interval::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum capacity.
 */
template<typename Problem>
typename sub_problem_by_interval<Problem>::weight_type
sub_problem_by_interval<Problem>::capacity() const
{
  return m_capacity;
} // sub_problem_by_interval::capacity()

