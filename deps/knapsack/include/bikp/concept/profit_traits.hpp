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
 * \file profit_traits.hpp
 * \brief Basic operations on profits.
 * \author Julien Jorge
 */
#ifndef __BIKP_PROFIT_TRAITS_HPP__
#define __BIKP_PROFIT_TRAITS_HPP__

#include "bikp/tool/types.hpp"

/**
 * \brief Basic operations on profits.
 * \author Julien Jorge
 */
template<class Profit>
class profit_traits
{
public:
  /** \brief The type of the manipulated profits. */
  typedef Profit profit_type;

  /**
   * \brief Function object that compares two profits by their \b values (in
   *        lexicographic order).
   * \author Julien Jorge.
   */
  class less
  {
  public:
    bool operator()( const profit_type& a, const profit_type& b ) const;
  }; // class less

public:
  //static void inc( profit_type& a );

  static bool better( const profit_type& a, const profit_type& b );

  /**
   * \brief Tell if two profits are of same quality.
   * \param a Left operand.
   * \param b Right operand.
   */
  static inline bool equal( const profit_type& a, const profit_type& b )
  {
    return a.equivalent_to(b);
  } // equal()

  static bool better_equal( const profit_type& a, const profit_type& b );
  static bool same( const profit_type& a, const profit_type& b );
  static bool better_same( const profit_type& a, const profit_type& b );

  static const profit_type& to_profit( const profit_type& a );
  static profit_type ideal( const profit_type& a, const profit_type& b );
  static profit_type nadir( const profit_type& a, const profit_type& b );

  static bool lex_less( const profit_type& a, const profit_type& b );

}; // class profit_traits

/**
 * \brief Specialization of the profit_traits class for real_type profits.
 * \author Julien Jorge
 */
template<>
class profit_traits<real_type>
{
public:
  /** \brief The type of the manipulated profits. */
  typedef real_type profit_type;

  /**
   * \brief Function object that compares two profits by their \b values (in
   *        lexicographic order).
   * \author Julien Jorge.
   */
  class less
  {
  public:
    bool operator()( profit_type a, profit_type b ) const;
  }; // class less

public:
  //static void inc( profit_type& a );

  static bool better( profit_type a, profit_type b );
  static bool equal( profit_type a, profit_type b );
  static bool better_equal( profit_type a, profit_type b );
  static bool same( profit_type a, profit_type b );
  static bool better_same( profit_type a, profit_type b );

  static profit_type to_profit( profit_type a );
  static profit_type ideal( profit_type a, profit_type b );
  static profit_type nadir( profit_type a, profit_type b );

  static bool lex_less( profit_type a, profit_type b );

}; // class profit_traits

#include "bikp/concept/impl/profit_traits.tpp"

#endif // __BIKP_PROFIT_TRAITS_HPP__
