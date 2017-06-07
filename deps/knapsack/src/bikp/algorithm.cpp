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
 * \file algorithm.cpp
 * \brief Various general algorithms.
 * \author Julien Jorge
 */
#include "bikp/tool/algorithm.hpp"
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the greatest common divisor using Euclide's algorithm.
 * \param a One operand.
 * \param b The other operand.
 */
/*real_type gcd( real_type a, real_type b )
{
  if ( a < b )
    std::swap(a, b);
  
  while ( b != 0 )
    {
      real_type t = a;
      a = b;
      b = t % b;
    }
  
  return a;
}*/ // gcd()
