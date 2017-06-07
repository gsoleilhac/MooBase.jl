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
 * \file combined_problem.cpp
 * \brief Implementation of the combined_problem class.
 * \author Julien Jorge
 */
#include <functional>
#include "bikp/problem/combined_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The bi-objective problem from which we combine the objectives.
 * \param lambda_1 Coefficient for the first objective.
 * \param lambda_2 Coefficient for the second objective.
 */
combined_problem::combined_problem
( const bi_problem& p, const real_type lambda_1,
  const real_type lambda_2 )
  : super(p.size()), m_profit(p.size()), m_weight(p.size()),
    m_capacity(p.capacity()), m_lambda_1(lambda_1), m_lambda_2(lambda_2)
{
  assert( m_capacity >= 0 );

  for (unsigned int i=0; i!=size(); ++i)
    {
      m_weight[i] = p.weight(i);
      m_profit[i] = combined_profit( lambda_1, lambda_2, p.profit(i) );
      m_efficiency[i] = (real_type)profit(i).lambda / (real_type)weight(i);
    }
} // combined_problem::combined_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the problem.
 * \param cap The capacity of the knapsack.
 */
combined_problem::combined_problem( unsigned int n, const weight_type cap )
  : super(n), m_profit(n), m_weight(n), m_capacity(cap)
{
  assert( cap >= 0 );
} // combined_problem::combined_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the profit & weigth of a variable.
 * \param i The index of the variable.
 * \param p The profit of the variable.
 * \param w The weight of the variable.
 */
void combined_problem::set_variable
( unsigned int i, const profit_type& p, weight_type w )
{
  assert( i < size() );
  assert( w >= 0 );

  m_profit[i] = p;
  m_weight[i] = w;
  m_efficiency[i] = (real_type)p.lambda / (real_type)w;
} // combined_problem::set_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the problem into a mono objective problem.
 * \param obj The objective to keep.
 */
mono_problem* combined_problem::convert_to_mono( unsigned int obj ) const
{
  mono_problem* p = new mono_problem( size(), capacity() );

  switch(obj)
    {
    case 0:
      for (unsigned int i=0; i!=size(); ++i)
	p->set_variable( i, profit(i).lambda, weight(i) );
      break;
    case 1:
      for (unsigned int i=0; i!=size(); ++i)
	p->set_variable( i, profit(i).profit_1, weight(i) );
      break;
    case 2:
      for (unsigned int i=0; i!=size(); ++i)
	p->set_variable( i, profit(i).profit_2, weight(i) );
    }

  return p;
} // combined_problem::convert_to_mono()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the problem.
 */
unsigned int combined_problem::size() const
{
  return m_profit.size();
} // combined_problem::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum capacity.
 */
combined_problem::weight_type combined_problem::capacity() const
{
  return m_capacity;
} // combined_problem::capacity()
