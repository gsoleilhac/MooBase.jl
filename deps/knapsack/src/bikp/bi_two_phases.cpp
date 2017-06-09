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
 * \file bi_two_phases.cpp
 * \brief Implementation of the bi_two_phases class.
 * \author Julien Jorge
 */
#include "bikp/solver/bi_two_phases.hpp"

#include "bikp/phase/dp_first_phase.hpp"
#include "bikp/phase/klp_second_phase.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
void
bi_two_phases::run( const bi_problem& p, std::list<bi_solution>& sols ) const
{
  typedef optimal_set<bi_problem::solution_type> optimal_set_type;

  dp_first_phase phase_1;

  phase_1.run(p);

  optimal_set_type::const_iterator it;
  optimal_set_type efficient_solutions( phase_1.get_extreme_solutions() );

  efficient_solutions.insert( phase_1.get_not_extreme_solutions() );

  if ( efficient_solutions.size() >= 2 )
    {
      std::list<bi_solution> X_SE
        ( efficient_solutions.begin(), efficient_solutions.end() );

      klp_second_phase phase_2 = klp_second_phase();

      phase_2.run(p, X_SE);

      sols.insert( sols.begin(), phase_2.get_solutions().begin(), phase_2.get_solutions().end() );
    }
  else
    sols.insert( sols.begin(), efficient_solutions.begin(), efficient_solutions.end() );
} // bi_two_phases::run()
