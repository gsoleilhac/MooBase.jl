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
 * \file bound_set_relaxation.hpp
 * \brief The bound_set_relaxation class compute an upper bound set of a
 *        combined_problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_BOUND_SET_RELAXATION_HPP__
#define __BIKP_BOUND_SET_RELAXATION_HPP__

#include "bikp/concept/pareto_set.hpp"
#include "bikp/problem/bi_problem.hpp"

/**
 * \brief The bound_set_relaxation class compute an upper bound set of a
 *        combined_problem.
 * \author Julien Jorge
 */
class bound_set_relaxation
{
public:
  /** \brief The type of the bound set. */
  typedef pareto_set<bi_profit> bound_set_type;
  
public:
  bound_set_relaxation( const bi_problem& p );

  bool interesting
  ( unsigned int i, const bi_profit& fixed,
    const bound_set_type& lower_bound, real_type cap ) const;

  bound_set_type relax
  ( unsigned int i, const bi_profit& fixed, real_type cap ) const;

private:
  bi_profit make_profit( real_type p1, real_type p2 ) const;

private:
  /** \brief The problem to relax. */
  const bi_problem& m_problem;

}; // bound_set_relaxation

#endif // __BIKP_BOUND_SET_RELAXATION_HPP__
