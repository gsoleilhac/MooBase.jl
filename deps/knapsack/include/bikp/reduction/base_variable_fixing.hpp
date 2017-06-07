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
 * \file base_variable_fixing.hpp
 * \brief This class group common fields of variable fixing classes.
 * \author Julien Jorge.
 */
#ifndef __BIKP_BASE_VARIABLE_FIXING_HPP__
#define __BIKP_BASE_VARIABLE_FIXING_HPP__

#include <set>
#include <vector>
#include "bikp/tool/logger.hpp"

/**
 * \brief This class group common fields of variable fixing classes.
 *
 * \b Template \b arguments :
 * - \a Problem : The type of the problem to reduce.
 *
 * \author Julien Jorge.
 */
template<typename Problem>
class base_variable_fixing
{
public:
  /** \brief The type of the problem to reduce. */
  typedef Problem problem_type;

  /** \brief The type of the profits. */
  typedef typename Problem::profit_type profit_type;

  /** \brief The type of the weight. */
  typedef typename Problem::weight_type weight_type;

  /** \brief Basic operations on solutions profits (objective value). */
  typedef typename Problem::profit_traits_type profit_traits_type;

  /** \brief The type of the solutions of the problem. */
  typedef typename problem_type::solution_type solution_type;

public:
  /** \brief Container for the indices of the fixed variables. */
  typedef std::set<unsigned int> fixed_set_type;

private:
  /** \brief The type of the current class. */
  typedef base_variable_fixing<problem_type> self_type;

public:
  const problem_type& get_sub_problem() const;
  const profit_type& guaranteed_profit() const;

  void
  restore_solution( const solution_type& sub_sol, solution_type& sol ) const;
  void
  create_solution( const solution_type& sol, solution_type& sub_sol ) const;

  template<typename P>
  void absorb( const base_variable_fixing<P>& sub_fixing );

  const fixed_set_type& set_variables() const;
  const fixed_set_type& unset_variables() const;

  base_variable_fixing( const problem_type& p );
  ~base_variable_fixing();

protected:
  void create_sub_problem();

private:
  weight_type merge( fixed_set_type& fixed );
  void make_indices( const fixed_set_type& unwanted );

  /** \brief Copy is forbidden. */
  base_variable_fixing( const self_type& that ) {}

protected:
  /** \brief The problem we want to reduce. */
  const problem_type& m_problem;

  /** \brief Variables assured to be in the solutions. */
  fixed_set_type m_set_variables;

  /** \brief Variables assured to be out of the solutions. */
  fixed_set_type m_unset_variables;

  /** \brief Log some informations. */
  tool::logger m_logger;

private:
  /** \brief The reduced problem. */
  problem_type* m_sub_problem;

  /** \brief The sum of the profit of the variables fixed in the solution. */
  profit_type m_guaranteed_profit;

  /** \brief Index of the variables kept in the problem. */
  std::vector<std::size_t> m_correspondence;

}; // class base_variable_fixing

#include "bikp/reduction/impl/base_variable_fixing.tpp"

#endif // __BIKP_BASE_VARIABLE_FIXING_HPP__
