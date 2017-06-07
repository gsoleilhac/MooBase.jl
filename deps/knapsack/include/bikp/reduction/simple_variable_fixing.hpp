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
 * \file simple_variable_fixing.hpp
 * \brief A class that applies a simple variable fixing, without bound update,
 *        to reduce a problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_SIMPLE_VARIABLE_FIXING_HPP__
#define __BIKP_SIMPLE_VARIABLE_FIXING_HPP__

#include "bikp/reduction/base_variable_fixing.hpp"
#include "bikp/relaxation/mt_linear_relaxation.hpp"
#include "bikp/problem/mono_problem.hpp"

/**
 * \brief A class that applies a simple variable fixing, without bound update,
 *        to reduce a problem.
 *
 * \pre Problem::profit_type is real_type
 * \pre Problem::weight_type is real_type
 *
 * \author Julien Jorge.
 */
class simple_variable_fixing
  : public base_variable_fixing<mono_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<mono_problem> super;

  /** \brief Container for the indices of the fixed variables. */
  typedef super::fixed_set_type fixed_set_type; 

private:
  /** \brief The type of the relaxation used for computing upper bounds. */
  typedef mt_linear_relaxation<mono_problem> relaxation_type;

public:
  simple_variable_fixing( const mono_problem& p, real_type bound );

private:
  void run( real_type bound );

  void find_sets
  ( const std::vector<real_type>& weight_sum,
    const std::vector<real_type>& profit_sum,
    unsigned int split_item, real_type bound );

  unsigned int
  find_index( real_type val, const std::vector<real_type>& table ) const;

  void calculate_sums( std::vector<real_type>& weight_sum,
		       std::vector<real_type>& profit_sum ) const;

  void calculate_upper_bound_zero
  ( const std::vector<real_type>& weight_sum,
    const std::vector<real_type>& profit_sum,
    real_type bound, unsigned int split_item );

  void calculate_upper_bound_one( const std::vector<real_type>& weight_sum,
				  const std::vector<real_type>& profit_sum,
				  real_type bound, unsigned int split_item );

  real_type relax( unsigned int split_item, real_type capacity,
		      real_type current_profit ) const;

  void fix_more();

}; // class simple_variable_fixing

#endif // __BIKP_SIMPLE_VARIABLE_FIXING_HPP__
