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
 * \file mt_linear_relaxation.hpp
 * \brief The mt_linear_relaxation class compute an upper bound on a
 *        mono-objective problem using the classic Martello & Toth linear
 *        relaxation.
 * \author Julien Jorge
 */
#ifndef __BIKP_MT_LINEAR_RELAXATION_HPP__
#define __BIKP_MT_LINEAR_RELAXATION_HPP__

#include "bikp/tool/types.hpp"

/**
 * \brief The mt_linear_relaxation class compute an upper bound on a
 *        mono-objective problem using the Martello & Toth linear relaxation.
 *
 * \pre Problem::profit_type is real_type
 * \pre Problem::weight_type is real_type
 *
 * \author Julien Jorge
 */
template<typename Problem>
class mt_linear_relaxation
{
public:
  /** \brief The type of the problem to relax. */
  typedef Problem problem_type;

  /** \brief The type of the profits in the problem. */
  typedef real_type profit_type;

  /** \brief The type of the weights in the problem. */
  typedef real_type weight_type;

  /**
   * \brief The result of the relaxation.
   */
  class result_type
  {
  public:
    /** \brief The value of the bound. */
    profit_type objective_value;

    /** \brief Tell if the bound is optimal or not. */
    bool optimal;
  }; // class result_type

public:
  result_type calculate( const problem_type& p, unsigned int split_item,
			 const profit_type current_profit,
			 const weight_type capacity ) const;

  result_type calculate( const problem_type& p ) const;

  result_type calculate_from( const problem_type& p, unsigned int split_item,
			      profit_type current_profit,
			      weight_type capacity ) const;

private:
  profit_type relax( const weight_type c, const profit_type p,
		     const weight_type w ) const;

  /**
   * \brief Tell if \a p * \a w1 is strictly greater than \a c * \a p1 with
   *        a conversion to \c real_type (to avoid integer overflow).
   * \param c Remaining capacity.
   * \param p Profit of the item.
   * \param p1 Profit of the previous item.
   * \param w1 Weight of the previous item.
   */
  inline bool greater_as_double( const weight_type c,
				 const profit_type p,
				 const profit_type p1,
				 const weight_type w1 ) const
  {
    return (real_type)p * (real_type)w1 > (real_type)c * (real_type)p1;
  } // greater_as_double()

  /**
   * \brief Divide \a p * \a w1 - \a c * \a p1 by \a w1 with a conversion to
   *        \c real_type (to avoid integer overflow).
   * \param c Remaining capacity.
   * \param p Profit of the item.
   * \param p1 Profit of the previous item.
   * \param w1 Weight of the previous item.
   */
  inline profit_type relax_as_double( const weight_type c,
				      const profit_type p,
				      const profit_type p1,
				      const weight_type w1 ) const
  {
    return (profit_type)
      ( ((real_type)p * (real_type)w1 - (real_type)c * (real_type)p1) / (real_type)w1 );
  } // relax_as_double()

}; // mt_linear_relaxation

#include "bikp/relaxation/impl/mt_linear_relaxation.tpp"

#endif // __BIKP_MT_LINEAR_RELAXATION_HPP__
