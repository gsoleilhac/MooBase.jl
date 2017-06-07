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
 * \file variable_fixing_by_bound_set.hpp
 * \brief Variable reduction applyable to a problem, that uses upper
 *        bound sets as bounds.
 * \author Julien Jorge
 */
#ifndef __BIKP_VARIABLE_FIXING_BY_BOUND_SET_HPP__
#define __BIKP_VARIABLE_FIXING_BY_BOUND_SET_HPP__

#include "bikp/reduction/base_variable_fixing.hpp"
#include "bikp/concept/pareto_set.hpp"
#include "bikp/problem/bi_problem.hpp"

#include <string>
#include <list>

/**
 * \brief Variable reduction applyable to a problem, that uses upper
 * \author Julien Jorge
 */
class variable_fixing_by_bound_set
  : public base_variable_fixing<bi_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<bi_problem> super;

  /** \brief A set of optimal solutions. */
  typedef pareto_set<bi_profit> bound_set_type;

public:
  variable_fixing_by_bound_set
  ( const bi_problem& p, const std::list<bi_solution>& X_SE );

private:
  void make_pattern
  ( std::string& pattern, const std::list<bi_solution>& X_SE ) const;
  void build_nadir( const bound_set_type& set, bound_set_type& out ) const;
  bool is_conv_dominated
  ( const bound_set_type& ub, const bound_set_type& lb ) const;

  bi_profit make_profit( real_type p1, real_type p2 ) const;

}; // class variable_fixing_by_bound_set

#endif // __BIKP_VARIABLE_FIXING_BY_BOUND_SET_HPP__
