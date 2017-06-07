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
 * \file combined_variable_fixing.hpp
 * \brief Variable reduction applyable to a combined_problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_VARIABLE_FIXING_HPP__
#define __BIKP_COMBINED_VARIABLE_FIXING_HPP__

#include "bikp/reduction/simple_variable_fixing.hpp"
#include "bikp/problem/mono_problem.hpp"
#include "bikp/problem/combined_problem.hpp"
#include "bikp/concept/optimal_set.hpp"

/**
 * \brief Variable reduction applyable to a combined_problem.
 * \author Julien Jorge
 */
class combined_variable_fixing
  : public base_variable_fixing<combined_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<problem_type> super;

  /** \brief A set of optimal solutions. */
  typedef optimal_set<combined_solution> optimal_solution_set;

  /** \brief Container for the indices of the fixed variables. */
  typedef super::fixed_set_type fixed_set_type; 

private:
  /** \brief The type of an iterator for inserting items in sets with
      std::set_union(). */
  typedef std::insert_iterator<fixed_set_type> insert_iterator;
  
  /** \brief The type of the reduction applied to the objectives,
      independently. */
  typedef simple_variable_fixing reduction_type;

  /**
   * \brief This class help us to get the real index of a variable in the
   *        std::transform() algorithm.
   */
   class real_index
   {
   public:
     real_index( const sortable_mono_problem& p );
     unsigned int operator()( unsigned int index ) const;

   private:
     /** \brief The sorted mono objective problem from which we get the
	 variables. */
     const sortable_mono_problem& m_problem;
   }; // class real_index

public:
  combined_variable_fixing
  ( const combined_problem& p, const combined_solution& x1,
    const combined_solution& x2, const real_type lb_lambda );

private:
  void do_reduction
  ( const mono_problem& mono_1, const mono_problem& mono_2,
    const mono_problem& mono_lambda, const combined_solution& x1,
    const combined_solution& x2, const real_type lb_lambda );

  void create_set_set
  ( const fixed_set_type& variables_1, const fixed_set_type& variables_2,
    const fixed_set_type& variables_lambda );

  void create_unset_set
  ( const fixed_set_type& variables_1, const fixed_set_type& variables_2,
    const fixed_set_type& variables_lambda );

}; // class combined_variable_fixing

#endif // __BIKP_COMBINED_VARIABLE_FIXING_HPP__
