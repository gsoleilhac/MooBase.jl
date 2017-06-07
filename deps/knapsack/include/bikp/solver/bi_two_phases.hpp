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
 * \file bi_two_phases.hpp
 * \brief Base class for first phase algorithms.
 * \author Julien Jorge
 */
#ifndef __BIKP_BI_TWO_PHASES_HPP__
#define __BIKP_BI_TWO_PHASES_HPP__

#include "bikp/problem/bi_problem.hpp"
#include <list>

/**
 * \brief A two phase algorithm.
 * \author Julien Jorge
 */
class bi_two_phases
{
public:
  void run( const bi_problem& p, std::list<bi_solution>& sols ) const;

}; // class bi_two_phases

#endif // __BIKP_BI_TWO_PHASES_HPP__
