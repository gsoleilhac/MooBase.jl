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
 * \file mono_problem.cpp
 * \brief Implementation of the mono_problem class.
 * \author Julien Jorge.
 */
#include "bikp/problem/mono_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the problem.
 * \param cap The capacity of the knapsack.
 */
mono_problem::mono_problem( unsigned int n, weight_type cap )
  : super(n), m_profit(n), m_weight(n), m_capacity(cap), m_local_index(n)
{
  assert( cap >= 0 );

  for (unsigned int i=0; i!=this->m_correspondence.size(); ++i)
    m_local_index[ this->m_correspondence[i] ] = i;
} // mono_problem::mono_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the profit & weigth of a variable.
 * \param i The index of the variable.
 * \param p The profit of the variable.
 * \param w The weight of the variable.
 */
void mono_problem::set_variable
( unsigned int i, const profit_type& p, weight_type w )
{
  assert( i < size() );
  assert( w >= 0 );

  m_profit[i] = p;
  m_weight[i] = w;
  m_efficiency[i] = (real_type)p / (real_type)w;
} // mono_problem::set_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the problem.
 */
unsigned int mono_problem::size() const
{
  return m_profit.size();
} // mono_problem::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum capacity.
 */
mono_problem::weight_type mono_problem::capacity() const
{
  return m_capacity;
} // mono_problem::capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the local index of a variable, from its index in the parent
 *        problem.
 * \param index The index of the variable in the parent problem.
 */
unsigned int mono_problem::local_index( unsigned int index ) const
{
  assert( index < m_local_index.size() );
  assert( this->real_index( m_local_index[index] ) == index );

  return m_local_index[index];
} // mono_problem::local_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by decreasing efficiency.
 */
void mono_problem::sort_by_decreasing_efficiency()
{
  super::sort_by_decreasing_efficiency();

  for (unsigned int i=0; i!=m_correspondence.size(); ++i)
    m_local_index[ m_correspondence[i] ] = i;
} // mono_problem::sort_by_decreasing_efficiency()
