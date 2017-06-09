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
 * \file mt_linear_relaxation.tpp
 * \brief Implementation of the mt_linear_relaxation class.
 * \author Julien Jorge
 */
#include "bikp/relaxation/mt_linear_relaxation.hpp"

#include <iomanip>
/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation once we have the split item.
 * \param p The problem to relax.
 * \param split_item The first item that doesn't fit.
 * \param current_profit The profit on the previously added items.
 * \param capacity The remaining capacity.
 */
template<typename Problem>
typename mt_linear_relaxation<Problem>::result_type
mt_linear_relaxation<Problem>::calculate
( const problem_type& p, unsigned int split_item,
  const profit_type current_profit, const weight_type capacity ) const
{
  assert( split_item <= p.size() );


  result_type result;

  result.objective_value = current_profit;
  result.optimal = split_item == p.size();

  if ( !result.optimal && (p.size() > 1) )
    {
      assert( p.weight(split_item) > capacity );

      if ( split_item == 0 )
	{
	  const profit_type next_profit = p.profit(split_item + 1);
	  const weight_type next_weight = p.weight(split_item + 1);
	  
	  result.objective_value += relax(capacity, next_profit, next_weight);
	}
      else if ( split_item+1 == p.size() )
	{
	  const profit_type prev_profit = p.profit(split_item - 1);
	  const weight_type prev_weight = p.weight(split_item - 1);
	  const profit_type profit = p.profit(split_item);
	  const weight_type weight = p.weight(split_item);
	  
	  if ( greater_as_double(weight - capacity, profit, prev_profit,
				 prev_weight) )
	    result.objective_value += relax_as_double(weight - capacity,
						      profit, prev_profit,
						      prev_weight);
	}
      else
	{
	  const profit_type next_profit = p.profit(split_item + 1);
	  const weight_type next_weight = p.weight(split_item + 1);
	  const profit_type prev_profit = p.profit(split_item - 1);
	  const weight_type prev_weight = p.weight(split_item - 1);
	  const profit_type profit = p.profit(split_item);
	  const weight_type weight = p.weight(split_item);
	  
	  profit_type ub_0 = relax(capacity, next_profit, next_weight);

	  if ( greater_as_double(weight - capacity, profit, prev_profit,
				 prev_weight) )
	    {
	      profit_type ub_1 = relax_as_double(weight - capacity,
						 profit, prev_profit,
						 prev_weight);

	      if ( ub_1 > ub_0 )
		result.objective_value += ub_1;
	      else
		result.objective_value += ub_0;
	    }
	  else
	    result.objective_value += ub_0;
	}

      assert( result.objective_value <=
	      current_profit + relax(capacity, p.profit(split_item),
				     p.weight(split_item)) );
    }

  return result;
} // mt_linear_relaxation::calculate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the value of the bound. 
 * \param p The problem we want to bound.
 * \pre The variables of p are sorted by decreasing efficiency.
 */
template<typename Problem>
typename mt_linear_relaxation<Problem>::result_type
mt_linear_relaxation<Problem>::calculate( const problem_type& p ) const
{
  profit_type zero_profit;
  zero_profit *= 0;

  return calculate_from(p, 0, zero_profit, p.capacity() );
} // mt_linear_relaxation::calculate()

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
typename mt_linear_relaxation<Problem>::result_type
mt_linear_relaxation<Problem>::calculate_from
( const problem_type& p, unsigned int item, profit_type current_profit,
  weight_type capacity ) const
{
  bool ok = true;

  while ( (item != p.size()) && ok )
    if ( p.weight( item ) <= capacity )
      {
	capacity -= p.weight( item );
	current_profit += (profit_type)p.profit( item );
	++item;
      }
    else
      ok = false;

  return calculate( p, item, current_profit, capacity );
} // mt_linear_relaxation::calculate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide (\a p * \a c) by \a w and try to avoid integer overflow.
 * \param c Remaining capacity.
 * \param p Profit of the item.
 * \param w Weight of the item.
 */
template<typename Problem>
typename mt_linear_relaxation<Problem>::profit_type
mt_linear_relaxation<Problem>::relax( const weight_type c, const profit_type p,
                                      const weight_type w ) const
{
  return c * p / w;
  //profit_type r = p % w;
  //return c * (p / w) + (c * r) / w;
} // mt_linear_relaxation::relax()

