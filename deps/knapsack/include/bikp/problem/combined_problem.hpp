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
 * \file combined_problem.hpp
 * \brief This mono-objective problem is made by a linear combinaition of the
 *        objectives of a bi-objective problem but also the profit of the
 *        variables in the bi-objective problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_PROBLEM_HPP__
#define __BIKP_COMBINED_PROBLEM_HPP__

#include "bikp/problem/mono_problem.hpp"
#include "bikp/problem/combined_profit.hpp"
#include "bikp/problem/bi_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief This mono-objective problem is made by a linear combination of the
 *        objectives of a bi-objective problem. Contrary to
 *        combined_mono_problem, we store also the profit of the variables in
 *        the bi-objective problem.
 * \author Julien Jorge
 */
class combined_problem
  : public sortable_mono_problem
{
public:
  /** \brief The type of the full problem. */
  typedef bi_problem problem_type;

  /** \brief The type of the profits. */
  typedef combined_profit profit_type;

  /** \brief The type of the weights. */
  typedef real_type weight_type;

  /** \brief The type of the current class. */
  typedef combined_problem self_type;

  /** \brief The type of the solution for this problem. */
  typedef solution<self_type> solution_type;

  /** \brief The type of the parent class. */
  typedef sortable_mono_problem super;

  /** \brief Basic operations on profits. */
  typedef profit_traits<profit_type> profit_traits_type;

private:
  /** \brief The type of the bi-objective problem reduced to its first
      objective. */
  //typedef mono_problem<self_type, 1> mono_1_problem_type;

  /** \brief The type of the bi-objective problem reduced to its second
      objective. */
  //typedef mono_problem<self_type, 2> mono_2_problem_type;

public:
  combined_problem( const bi_problem& p, const real_type lambda_1,
		    const real_type lambda_2 );
  combined_problem( unsigned int n, weight_type cap );

  void set_variable( unsigned int i, const profit_type& p, weight_type w );

  mono_problem* convert_to_mono( unsigned int obj ) const;

  /**
   * \brief Get the profit of a variable.
   * \param index The index of the variable from which we want the profit.
   */
  inline profit_type profit( unsigned int index ) const
    {
      return m_profit[ m_correspondence[index] ];
    } // profit()

  /**
   * \brief Get the weight of a variable.
   * \param index The index of the variable from which we want the weight.
   */
  inline weight_type weight( unsigned int index ) const
    {
      return m_weight[ this->m_correspondence[index] ];
    } // weight()

  unsigned int size() const;
  weight_type capacity() const;

  real_type get_lambda_1() const { return m_lambda_1; }
  real_type get_lambda_2() const { return m_lambda_2; }

  inline real_type get_efficiency( unsigned int obj, unsigned int index ) const
  {
    assert( obj == 0 );
    return m_efficiency[ this->m_correspondence[index] ];
  }

private:
  /** \brief Profits of the items. */
  std::vector<profit_type> m_profit;

  /** \brief Weight of the items. */
  std::vector<weight_type> m_weight;

  /** \brief Maximum capacity. */
  const weight_type m_capacity;

  real_type m_lambda_1;
  
  real_type m_lambda_2;
  
}; // combined_problem

/** \brief The type of the solution for this problem. */
typedef solution<combined_problem> combined_solution;

/** \brief Basic operations on profits. */
typedef profit_traits<combined_profit> combined_profit_traits;

#endif // __BIKP_COMBINED_PROBLEM_HPP__
