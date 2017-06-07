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
 * \file solution_traits.hpp
 * \brief Basic operations on solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_SOLUTION_TRAITS_HPP__
#define __BIKP_SOLUTION_TRAITS_HPP__

/**
 * \brief Basic operations on solutions.
 * \author Julien Jorge
 */
template<class Solution>
class solution_traits
{
public:
  /** \brief The type of the manipulated solutions. */
  typedef Solution solution_type;

  /** \brief Basic operations on the objective value. */
  typedef typename solution_type::profit_traits_type profit_traits_type;

  /** \brief The type of the objective value. */
  typedef typename solution_type::profit_type profit_type;

  /**
   * \brief Function object that compares two solutions by their objective 
   *        values (in lexicographic order).
   * \author Julien Jorge.
   */
  class less
  {
  public:
    bool operator()( const solution_type& a, const solution_type& b ) const;
  }; // class less

public:
  static bool better( const solution_type& a, const solution_type& b );
  static bool equal( const solution_type& a, const solution_type& b );
  static bool better_equal( const solution_type& a, const solution_type& b );
  static bool same( const solution_type& a, const solution_type& b );
  static bool better_same( const solution_type& a, const solution_type& b );

  static bool lex_less( const solution_type& a, const solution_type& b );

  static profit_type to_profit( const solution_type& a );

}; // class solution_traits

#include "bikp/concept/impl/solution_traits.tpp"

#endif // __BIKP_SOLUTION_TRAITS_HPP__
