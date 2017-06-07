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
 * \file simple_variable_fixing.cpp
 * \brief Implementation of the simple_variable_fixing class.
 * \author Julien Jorge
 */
#include "bikp/tool/algorithm.hpp"
#include "bikp/reduction/simple_variable_fixing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param bound The lower bound. We will try to be better or equal.
 *
 * \pre The variables of \a p are sorted by decreasing efficiency.
 */
simple_variable_fixing::simple_variable_fixing
( const mono_problem& p, real_type bound )
  : super(p)
{
  assert( p.size() > 0 );
  run(bound);
} // simple_variable_fixing::simple_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the problem reduction.
 * \param bound The lower bound. We will try to be better or equal.
 */
void simple_variable_fixing::run( real_type bound )
{
  const unsigned int problem_size = this->m_problem.size();

  std::vector<real_type> weight_sum(problem_size);
  std::vector<real_type> profit_sum(problem_size);
  
  calculate_sums( weight_sum, profit_sum );
  
  const real_type capacity = this->m_problem.capacity();
  const unsigned int split_item = find_index( capacity, weight_sum );
  
  if (split_item < problem_size)
    find_sets( weight_sum, profit_sum, split_item, bound );
  else
    for (unsigned int i=0; i!=problem_size; ++i)
      this->m_set_variables.insert(i);

  this->create_sub_problem();
} // simple_variable_fixing::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the sets of fixed variables.
 * \param weight_sum Table such that weight_sum[i] is the sum of the weight of
 *        the items j, j <= i.
 * \param profit_sum Table such that profit_sum[i] is the sum of the profit of
 *        the items j, j <= i.
 * \param split_item Index of the split item.
 * \param bound The lower bound.
 */
void simple_variable_fixing::find_sets
( const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  unsigned int split_item, real_type bound )
{
  calculate_upper_bound_zero( weight_sum, profit_sum, bound, split_item );
  calculate_upper_bound_one( weight_sum, profit_sum, bound, split_item );

  fix_more();
} // simple_variable_fixing::find_sets()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the index of the first item in the table such that
 *        table[i-1] <= val < table[i].
 * \param val The value to find.
 * \param table The table to search.
 */
unsigned int simple_variable_fixing::find_index
( real_type val, const std::vector<real_type>& table ) const
{
  assert( table.size() > 0 );

  return find_first_greater( val, table.begin(), table.end() ) - table.begin();
} // simple_variable_fixing::find_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the tables of cumulates weights and profits.
 * \param weight_sum (out) The table of cumulate weights.
 * \param profit_sum (out) The table of cumulate profits.
 */
void simple_variable_fixing::calculate_sums
( std::vector<real_type>& weight_sum,
  std::vector<real_type>& profit_sum ) const
{
  weight_sum[0] = this->m_problem.weight(0);
  profit_sum[0] = this->m_problem.profit(0);

  for (unsigned int i=1; i!=this->m_problem.size(); ++i)
    {
      weight_sum[i] = weight_sum[i-1] + this->m_problem.weight(i);
      profit_sum[i] = profit_sum[i-1] + this->m_problem.profit(i);
    }
} // simple_variable_fixing::calculate_sums()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the upper bound by setting the value of each item in range
 *        [1,s] to zero and fix the variable if the upper bound is less or equal
 *        to the lower_bound.
 * \param weight_sum The table of cumulate weights.
 * \param profit_sum The table of cumulate profits.
 * \param split_item Index of the split item.
 * \param bound The lower bound.
 */
void simple_variable_fixing::calculate_upper_bound_zero
( const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  real_type bound, unsigned int split_item )
{
  const real_type capacity = this->m_problem.capacity();

  for (unsigned int i=0; i<=split_item; ++i)
    {
      const real_type item_profit = this->m_problem.profit(i); 
      const real_type item_weight = this->m_problem.weight(i);
      const unsigned int new_split =
	find_index( capacity + item_weight, weight_sum );

      const real_type remaining_capacity =
	capacity + item_weight - weight_sum[new_split-1];
      const real_type new_profit = profit_sum[new_split - 1] - item_profit;

      real_type upper_bound =
	relax(new_split, remaining_capacity, new_profit);

      if ( bound > upper_bound )
	this->m_set_variables.insert( i );
    }
} // simple_variable_fixing::calculate_upper_bound_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the upper bound by setting the value of each item in range
 *        [s,n] to one and fix the variable if the upper bound is less or equal
 *        to the lower_bound.
 * \param weight_sum The table of cumulate weights.
 * \param profit_sum The table of cumulate profits.
 * \param bound The lower bound.
 * \param split_item Index of the split item.
 */
void simple_variable_fixing::calculate_upper_bound_one
( const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  real_type bound, unsigned int split_item )
{
  const real_type capacity = this->m_problem.capacity();
  const unsigned int i_max = this->m_problem.size() - split_item;

  for (unsigned int i=0; i!=i_max; ++i)
    {
      const unsigned int index = split_item + i;
      const real_type item_profit = this->m_problem.profit(index); 
      const real_type item_weight = this->m_problem.weight(index);
      const unsigned int new_split =
	find_index( capacity - item_weight, weight_sum );

      real_type weights;
      real_type profits;
      
      if (new_split > 0)
	{
	  weights = weight_sum[new_split-1];
	  profits = profit_sum[new_split-1];
	}
      else
	{
	  weights = 0;
	  profits = 0;
	}

      const real_type remaining_capacity = capacity - item_weight - weights;
      const real_type new_profit = profits + item_profit;

      real_type upper_bound =
	relax(new_split, remaining_capacity, new_profit);

      if ( bound > upper_bound )
	this->m_unset_variables.insert( index );
    }
} // simple_variable_fixing::calculate_upper_bound_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation.
 * \param split_item Index of the split item.
 * \param capacity Remaining capacity.
 * \param current_profit Profit to add to the calculated bound.
 */
real_type simple_variable_fixing::relax
( unsigned int split_item, real_type capacity,
  real_type current_profit ) const
{
  relaxation_type rl;
  relaxation_type::result_type result;

  result = rl.calculate(this->m_problem, split_item, current_profit, capacity);

  return result.objective_value;
} // simple_variable_fixing::relax()

/*----------------------------------------------------------------------------*/
/**
 * \brief Try to fix more variables by removing the ones that doesn't fit in the
 *        remaining capacity and by fixing all remaining variables if the sum
 *        of their weight fit in the remaining capacity.
 */
void simple_variable_fixing::fix_more()
{
  real_type needed_capacity = 0;
  fixed_set_type::const_iterator it;

  for (it=this->m_set_variables.begin(); it!=this->m_set_variables.end(); ++it)
    needed_capacity += this->m_problem.weight( *it );  

  // infeasibility ==> lower bound is optimal
  if ( needed_capacity > this->m_problem.capacity() )
    {
      this->m_set_variables.clear();
      
      for (unsigned int i=0; i!=this->m_problem.size(); ++i)
	this->m_unset_variables.insert(i);
    }
  else
    {
      real_type free_weight_sum = 0;
      std::vector<std::size_t> free_items;
      unsigned int more(0);
      const real_type remaining_capacity =
	this->m_problem.capacity() - needed_capacity;

      free_items.reserve( this->m_problem.size() - 
			  std::max( this->m_set_variables.size(),
				    this->m_unset_variables.size() ) );

      for (unsigned int i=0; i!=this->m_problem.size(); ++i)
	if ((this->m_set_variables.find(i) == this->m_set_variables.end()) &&
	    (this->m_unset_variables.find(i) == this->m_unset_variables.end()))
	  {
	    if ( this->m_problem.weight(i) > remaining_capacity )
	      {
		++more;
		this->m_unset_variables.insert(i);
	      }
	    else
	      {
		free_items.push_back(i);
		free_weight_sum += this->m_problem.weight(i);
	      }
	  }
      
      if ( free_weight_sum <= remaining_capacity )
	{
	  more += free_items.size();
	  
	  for (unsigned int i=0; i!=free_items.size(); ++i)
	    this->m_set_variables.insert(i);
	}
    }
} // simple_variable_fixing::fix_more()
