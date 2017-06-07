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
 * \file linear_relaxation.tpp
 * \brief Implementation of the linear_relaxation class.
 * \author Julien Jorge
 */
#include "bikp/relaxation/linear_relaxation.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation once we have the split item.
 * \param p The problem to relax.
 * \param split_item The first item that doesn't fit.
 * \param current_profit The profit on the previously added items.
 * \param capacity The remaining capacity.
 */
template<typename Problem>
typename linear_relaxation<Problem>::result_type
linear_relaxation<Problem>::calculate
( const problem_type& p, unsigned int split_item,
  const profit_type& current_profit, const weight_type& capacity ) const
{
  result_type result;

  result.objective_value = current_profit;
  result.optimal = split_item == p.size();

  if ( !result.optimal )
    result.objective_value += relax( capacity, p.profit( split_item ),
				     p.weight( split_item ) );

  return result;
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the value of the bound. 
 * \param p The problem we want to bound.
 * \pre The variables of p are sorted by decreasing efficiency.
 */
template<typename Problem>
typename linear_relaxation<Problem>::result_type
linear_relaxation<Problem>::calculate( const problem_type& p ) const
{
  profit_type zero_profit;
  zero_profit *= 0;

  return calculate_from( p, 0, zero_profit, p.capacity() );
} // linear_relaxation::calculate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the value of the bound, from a given item.
 * \param p The problem we want to bound.
 * \param item The item from which we start.
 * \param current_profit The profit on the previously added items.
 * \param capacity The remaining capacity.
 * \pre The variables of p are sorted by decreasing efficiency.
 */
template<typename Problem>
typename linear_relaxation<Problem>::result_type
linear_relaxation<Problem>::calculate_from
( const problem_type& p, unsigned int item, profit_type current_profit,
  weight_type capacity ) const
{
  bool ok = true;

  while ( (item != p.size()) && ok )
    if ( p.weight( item ) <= capacity )
      {
	capacity -= p.weight( item );
	current_profit += p.profit( item );
	++item;
      }
    else
      ok = false;

  return calculate(p, item, current_profit, capacity);
} // linear_relaxation::calculate_from()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide (\a p * \a c) by \a w and try to avoid integer overflow.
 * \param c Remaining capacity.
 * \param p Profit of the item.
 * \param w Weight of the item.
 */
template<typename Problem>
typename linear_relaxation<Problem>::profit_type
linear_relaxation<Problem>::relax
( const weight_type c, const profit_type p, const weight_type w ) const
{
  return c * p / w;
  //profit_type r = p % w;
  //return c * (p / w) + (c * r) / w;
} // linear_relaxation::relax()
