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
 * \file extern_variable_fixing.hpp
 * \brief Variable reduction applyable to a combined_problem, that uses upper
 *        bound sets as bounds.
 * \author Julien Jorge
 */
#ifndef __BIKP_EXTERN_VARIABLE_FIXING_HPP__
#define __BIKP_EXTERN_VARIABLE_FIXING_HPP__

#include "bikp/reduction/base_variable_fixing.hpp"

/**
 * \brief Variable reduction applyable to a combined_problem, that uses upper
 * \author Julien Jorge
 */
template<class Problem>
class extern_variable_fixing
  : public base_variable_fixing<Problem>
{
public:
  typedef Problem problem_type;
  
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<Problem> super;

public:
  extern_variable_fixing
  ( const problem_type& p, const std::string& file_name );

}; // class extern_variable_fixing

#include "bikp/reduction/impl/extern_variable_fixing.tpp"

#endif // __BIKP_EXTERN_VARIABLE_FIXING_HPP__
