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
 * \file mono_minimum_complete_set.hpp
 * \brief A set of non-dominated mono solutions, without equivalent ones.
 * \author Julien Jorge
 */
#ifndef __BIKP_MONO_MINIMUM_COMPLETE_SET_HPP__
#define __BIKP_MONO_MINIMUM_COMPLETE_SET_HPP__

#include "bikp/concept/solution_traits.hpp"

/**
 * \brief A set of non-dominated mono solutions, without equivalent ones.
 *
 * \b Template \b arguments :
 * - \a Solution : The type of the stored solutions.
 * - \a SolutionTraits : The type of the traits on the solutions.
 *
 * \pre SolutionTraits::to_point( SolutionTraits ) returns a type convertible to
 *      real_type.
 * \author Julien Jorge
 */
template <class Solution, class SolutionTraits = solution_traits<Solution> >
class mono_minimum_complete_set
{
public:
  /** \brief The type of the stored solutions. */
  typedef Solution value_type;

  /** \brief The type of the current class. */
  typedef mono_minimum_complete_set<value_type> self_type;

  /** \brief Iterator on the solutions. */
  typedef const value_type* const_iterator;

  /** \brief The type of the traits on the solutions. */
  typedef SolutionTraits solution_traits_type;

public:
  mono_minimum_complete_set();
  mono_minimum_complete_set( const self_type& that );
  ~mono_minimum_complete_set();

  self_type& operator=( const self_type& that );

  bool insert( const value_type& p );
  /*void insert( const self_type& that );
  template<typename Iterator>
  void insert( Iterator first, Iterator last );*/

  bool is_dominated( const value_type& p ) const;
  real_type size() const;
  bool empty() const;
  const_iterator begin() const;
  const_iterator end() const;

private:
  /** \brief The value of not dominated solution, if any. */
  value_type* m_solution;

}; // class mono_minimum_complete_set

#include "bikp/concept/impl/mono_minimum_complete_set.tpp"

#endif // __BIKP_MONO_MINIMUM_COMPLETE_SET_HPP__
