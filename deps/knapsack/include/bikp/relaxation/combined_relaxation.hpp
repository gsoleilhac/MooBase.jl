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
 * \file combined_relaxation.hpp
 * \brief The combined_relaxation class compute an upper bound on each objective
 *        of a combined_problem using the Martello & Toth's improved linear
 *        relaxation.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_RELAXATION_HPP__
#define __BIKP_COMBINED_RELAXATION_HPP__

#include "bikp/relaxation/linear_relaxation.hpp"
#include "bikp/problem/sub_problem_by_subset.hpp"
#include "bikp/problem/combined_problem.hpp"

/**
 * \brief The combined_relaxation class compute an upper bound on each objective
 *        of a combined_problem using the Martello & Toth's improved linear
 *        relaxation.
 * \author Julien Jorge
 */
class combined_relaxation
{
public:
  /** \brief The type of the problem to relax. */
  typedef combined_problem problem_type;

  /** \brief The type of the profits in the problem. */
  typedef problem_type::profit_type profit_type;

  /** \brief The type of the weights in the problem. */
  typedef problem_type::weight_type weight_type;

private:
  /** \brief The type of the sub problem on the first objective. */
  typedef sub_problem_by_subset<mono_problem> sub_problem;

  /** \brief The type of the relaxation on the combined objective. */
  typedef linear_relaxation<mono_problem> relaxation_type;

  /** \brief The type of the relaxation on the combined objective. */
  typedef linear_relaxation<sub_problem> sub_relaxation_type;

public:
  combined_relaxation( const problem_type& p );
  ~combined_relaxation();

  bool interesting( const profit_type& profit, const profit_type& bound,
		    unsigned int from_var, weight_type cap ) const;

protected:
  real_type relax_lambda
  ( real_type profit, unsigned int from_var, weight_type cap ) const;
  real_type relax_z1
  ( real_type profit, unsigned int from_var, weight_type cap ) const;
  real_type relax_z2
  ( real_type profit, unsigned int from_var, weight_type cap ) const;

private:
  /** \brief The bi-objective problem reduced to its combined objective. */
  mono_problem* m_mono;

  /** \brief The bi-objective problem reduced to its first objective. */
  mono_problem* m_mono_1;

  /** \brief The bi-objective problem reduced to its second objective. */
  mono_problem* m_mono_2;

  /**
   * \brief Indices of the wanted variables when doing a linear relaxation of
   *        m_mono_1 from a specific variable in m_mono.
   *
   * The relaxation is done on the j >= i variables of the combined problem.
   * These variables have different indices in the mono-objectives problems
   * (these problems are sorted by decreasing efficiency but the efficiency of
   * a variable is different regarding to the objective).
   *
   * To reduce the computation time, we store, at the beginning of the process,
   * the indices of the wanted variables of each mono-objective problem
   * according to the first variable in m_mono.
   *
   * m_sub_1[i] is the problem made with the variables of m_mono_1 having their
   * indices in [i, n) in m_mono.
   */
  std::vector<sub_problem*> m_sub_1;

  /** \brief Same as m_wanted_1 but for m_mono_2. */
  std::vector<sub_problem*> m_sub_2;

}; // combined_relaxation

#endif // __BIKP_COMBINED_RELAXATION_HPP__
