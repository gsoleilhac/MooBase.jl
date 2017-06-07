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
 * \file algorithm.hpp
 * \brief Various general algorithms.
 * \author Julien Jorge
 */
#ifndef __BIKP_TOOL_ALGORITHM_HPP__
#define __BIKP_TOOL_ALGORITHM_HPP__

template<typename Iterator, class BinaryPredicate>
Iterator find_first_greater( const typename Iterator::value_type& val,
			     const Iterator& first, const Iterator& last,
			     BinaryPredicate comp );

template<typename Iterator>
Iterator find_first_greater( const typename Iterator::value_type& val,
			     const Iterator& first, const Iterator& last );

//real_type gcd( real_type a, real_type b );

#include "bikp/tool/impl/algorithm.tpp"

#endif // __BIKP_TOOL_ALGORITHM_HPP__
