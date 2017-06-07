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
 * \file algorithm.tpp
 * \brief Various general algorithms.
 * \author Julien Jorge
 */
#include <functional>
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the first item in range [first, last) such that
 *        *(item-1) <= val < *item.
 * \param val The split value.
 * \param first First item in the range.
 * \param last First item after the last valid item in the range.
 * \param comp is a function object such that comp(a,b) return true if and only
 *        if a < b.
 */
template<typename Iterator, class BinaryPredicate>
Iterator find_first_greater( const typename Iterator::value_type& val,
			     const Iterator& first, const Iterator& last,
			     BinaryPredicate comp )
{
  Iterator l = last;
  Iterator f = first;

  while ( l - f > 1 )
    {
      Iterator middle = f + (l - f) / 2;
      
      if ( comp(val, *middle) )
        l = middle;
      else
        f = middle;
    }
  
  if ( comp(val, *f) )
    l = f;

  if ( l != last )
    {
      assert( comp(val, *l) );
    }
  if ( f != l )
    {
      assert( comp(*f, val) || ( !comp(*f, val) && ! comp(val, *f) ) );
    }

  return l;
} // find_first_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the first item in range [first, last) such that
 *        *(item-1) <= val < *item.
 * \param val The split value.
 * \param first First item in the range.
 * \param last First item after the last valid item in the range.
 */
template<typename Iterator>
Iterator find_first_greater( const typename Iterator::value_type& val,
			     const Iterator& first, const Iterator& last )
{
  return find_first_greater( val, first, last,
			     std::less<typename Iterator::value_type>() );
} // find_first_greater()
