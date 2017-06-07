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
 * \file optimal_set.hpp
 * \brief A set of non-dominated solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_OPTIMAL_SET_HPP__
#define __BIKP_OPTIMAL_SET_HPP__

#include <list>
#include "bikp/concept/solution_traits.hpp"

/**
 * \brief A set of non-dominated solutions.
 *
 * \b Template \b arguments :
 * - \a Solution : The type of the stored solutions.
 * - \æ SolutionTraits : The type of the traits on the solutions.
 *
 * \author Julien Jorge
 */
template <class Solution, class SolutionTraits = solution_traits<Solution> >
class optimal_set
{
public:
  /** \brief The type of the stored solutions. */
  typedef Solution value_type;

  /** \brief The type of the stored solutions. */
  typedef value_type solution_type;
  
  /** \brief The type of the traits on the solutions. */
  typedef SolutionTraits solution_traits_type;

  /** \brief The container of our solutions. */
  typedef std::list<solution_type> set_type;

  /** \brief The type of the current class. */
  typedef optimal_set<solution_type, solution_traits_type> self_type;

  /** \brief Iterator on the solutions. */
  typedef typename set_type::const_iterator const_iterator;

public:
  optimal_set( bool multi_set = false );
  optimal_set( const self_type& that );
  ~optimal_set();

  self_type& operator=( const self_type& that );

  bool insert( const solution_type& sol );
  void insert( const self_type& that );
  template<typename Iterator>
  void insert( Iterator first, Iterator last );

  bool is_dominated( const solution_type& sol ) const;
  real_type size() const;
  bool empty() const;
  const_iterator begin() const;
  const_iterator end() const;

  bool is_multi_set() const;

  void clear();

private:
  /** \brief The non-dominated solutions. */
  set_type m_solutions;

  /** \brief Tell if we keep solutions with the same profit, or not. */
  const bool m_multi_set;

}; // class optimal_set

#include "bikp/concept/impl/optimal_set.tpp"

#endif // __BIKP_OPTIMAL_SET_HPP__
