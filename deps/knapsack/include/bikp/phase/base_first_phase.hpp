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
 * \file base_first_phase.hpp
 * \brief Base class for first phase algorithms.
 * \author Julien Jorge
 */
#ifndef __BIKP_BASE_FIRST_PHASE_HPP__
#define __BIKP_BASE_FIRST_PHASE_HPP__

#include <iostream>

#include "bikp/problem/bi_problem.hpp"
#include "bikp/problem/combined_problem.hpp"
#include "bikp/concept/optimal_set.hpp"
#include "bikp/tool/logger.hpp"

/**
 * \brief First phase : calculate suported solutions.
 * \author Julien Jorge
 */
class base_first_phase
{
public:
  /** \brief The type of the set containing the calculated solutions. */
  typedef optimal_set<bi_solution> optimal_set_type;

  /** \brief The type of the set containing the calculated solutions for
      combined problems. */
  typedef optimal_set<combined_solution> combined_optimal_set_type;

  /** \brief The type of the set containing the calculated solutions. */
  typedef optimal_set<mono_solution> mono_optimal_set_type;

public:
  base_first_phase( const std::string& name );
  virtual ~base_first_phase() {}

  void run(const bi_problem& p);

  void save( std::ostream& extrem_os, std::ostream& not_extrem_os ) const;
  void clear();

  const optimal_set_type& get_extreme_solutions() const;
  const optimal_set_type& get_not_extreme_solutions() const;

protected:
  virtual void
  solve( const mono_problem& p, mono_solution& sol, real_type lb ) const = 0;

  virtual void solve( const combined_problem& p, combined_solution& sol,
		      const combined_profit& lb ) const = 0;

  virtual void solve
  ( const combined_problem& p, combined_optimal_set_type& sols,
    const combined_profit& lb ) const = 0;

private:
  void calculate_pareto_optimal
  ( const bi_problem& p, bi_solution& x1, bi_solution& x2 ) const;

  void calculate_solutions
  ( const bi_problem& p, const bi_solution& x1, const bi_solution& x2 );

  void calculate_triangle
  ( const bi_problem& p, const bi_solution& x1, const bi_solution& x2,
    optimal_set_type& new_solutions ) const;

  void calculate_lambda_unique( const bi_problem& p, const bi_profit& z1,
				const bi_profit& z2, bi_solution& sol ) const;

  void calculate_lambda_multiple
  ( const bi_problem& p, const bi_solution& x1, const bi_solution& x2,
    optimal_set_type& new_solutions ) const;
  
  void find_mono_optimal( const bi_problem& p, unsigned int obj,
			  bi_solution& sol, const bi_solution& low ) const;

  void find_unique
  ( const combined_problem& mono_p, combined_solution& sol ) const;

  void find_multiple( const combined_problem& mono_p,
		      combined_optimal_set_type& new_solutions ) const;

  bool update_solutions(const bi_solution& x1, const bi_solution& x2,
			const optimal_set_type& new_solutions);

private:
  /** \brief A logger, to write informations about the process status. */
  tool::logger m_logger;

  /** \brief Extrem supported solutions. */
  optimal_set_type m_extreme_solutions;

  /** \brief Not extrem supported solutions. */
  optimal_set_type m_not_extreme_solutions;

}; // class base_first_phase

#endif // __BIKP_BASE_FIRST_PHASE_HPP__
