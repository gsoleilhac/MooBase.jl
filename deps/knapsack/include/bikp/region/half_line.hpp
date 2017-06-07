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
 * \file half_line.hpp
 * \brief The "half_line" region is an one-dimensional region containing
 *        solutions having their objective value at least equal to a specific
 *        value.
 * \author Julien Jorge
 */
#ifndef __BIKP_HALF_LINE_HPP__
#define __BIKP_HALF_LINE_HPP__

/**
 * \brief The "half_line" region is an one-dimensional region containing
 *        solutions having their objective value at least equal to a specific
 *        value.
 */
template<class ProfitTraits>
class half_line
{
public:
  /** \brief Basic operations on profits. */
  typedef ProfitTraits profit_traits_type;

  /** \brief The type of the profits in our search region. */
  typedef typename profit_traits_type::profit_type profit_type;

  /** \brief The type of the current class. */
  typedef half_line<profit_traits_type> self_type;

public:
  half_line( const profit_type min_value, bool keep_equivalent );

  profit_type min_profit() const;
  bool contains( const profit_type p ) const;
  bool feasible( const profit_type& p ) const;

  self_type& operator=( const self_type& that );

  template<class Iterator>
  void tighten( Iterator first, Iterator last );

private:
  /** \brief The minimum interesting value. */
  profit_type m_min_value;

  /** \brief Tell if the region must keep equivalent solutions. */
  const bool m_keep_equivalent;

}; // class half_line

#include "bikp/region/impl/half_line.tpp"

#endif // __BIKP_HALF_LINE_HPP__
