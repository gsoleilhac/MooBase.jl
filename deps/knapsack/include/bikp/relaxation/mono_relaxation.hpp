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
 * \file mono_relaxation.hpp
 * \brief The mono_relaxation class compute an upper bound on the objective 
 *        of a mono objective problem using the Martello & Toth's improved
 *        linear relaxation.
 * \author Julien Jorge
 */
#ifndef __BIKP_MONO_RELAXATION_HPP__
#define __BIKP_MONO_RELAXATION_HPP__

#include "bikp/relaxation/mt_linear_relaxation.hpp"

/**
 * \brief The mono_relaxation class compute an upper bound on the objective 
 *        of a mono objective problem using the Martello & Toth's improved
 *        linear relaxation.
 *
 * \author Julien Jorge
 */
template<typename Problem>
class mono_relaxation
{
public:
  /** \brief The type of the problem to relax. */
  typedef Problem problem_type;

  /** \brief The type of the profits in the problem. */
  typedef real_type profit_type;

  /** \brief The type of the weights in the problem. */
  typedef real_type weight_type;

private:
  /** \brief The type of the problem reduced to its mono objective. */
  typedef problem_type mono_problem_type;

  /** \brief The type of the relaxation on the mono objective. */
  typedef mt_linear_relaxation<mono_problem_type> mono_relax;

public:
  mono_relaxation( const problem_type& p );

  bool interesting( const profit_type& profit, const profit_type& bound,
		    unsigned int from_var, weight_type cap ) const;

  real_type
  relax( real_type profit, unsigned int from_var, weight_type cap ) const;

private:
  /** \brief The bi-objective problem reduced to its mono objective. */
  mono_problem_type m_mono;

}; // mono_relaxation

#include "bikp/relaxation/impl/mono_relaxation.tpp"

#endif // __BIKP_MONO_RELAXATION_HPP__
