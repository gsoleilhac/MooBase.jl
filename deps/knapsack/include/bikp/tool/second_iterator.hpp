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
 * \file second_iterator.hpp
 * \brief Generic iterator of the second members of a map.
 * \author Julien Jorge
 */
#ifndef __BIKP_SECOND_ITERATOR_HPP__
#define __BIKP_SECOND_ITERATOR_HPP__

template<class MapIterator>
class const_second_iterator
{
public:
  typedef MapIterator iterator_type;

  typedef typename iterator_type::value_type::second_type value_type;
  typedef const value_type& const_reference;
  typedef const value_type* const_pointer;

  typedef const_second_iterator<iterator_type> self_type;

public:
  const_second_iterator() {}
  const_second_iterator( iterator_type it ) : m_it(it) {}

  self_type& operator++()
  {
    ++m_it;
    return *this;
  }

  self_type operator++(int)
  {
    self_type tmp(*this);
    ++m_it;
    return tmp;
  }

  self_type& operator--()
  {
    --m_it;
    return *this;
  }

  self_type operator--(int)
  {
    self_type tmp(*this);
    --m_it;
    return tmp;
  }

  const_reference operator*() { return m_it->second; }
  const_pointer operator->() { return &m_it->second; }

  bool operator==( const self_type& that ) const { return m_it == that.m_it; }
  bool operator!=( const self_type& that ) const { return m_it != that.m_it; }

private:
  /** \brief The real iterator on the container. */
  iterator_type m_it;

}; // second_iterator

#endif // __BIKP_SECOND_ITERATOR_HPP__
