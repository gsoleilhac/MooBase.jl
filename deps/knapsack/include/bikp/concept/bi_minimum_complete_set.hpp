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
 * \file bi_minimum_complete_set.hpp
 * \brief A set of non-dominated bi solutions, without equivalent ones.
 * \author Julien Jorge
 */
#ifndef __BIKP_BI_MINIMUM_COMPLETE_SET_HPP__
#define __BIKP_BI_MINIMUM_COMPLETE_SET_HPP__

#include "bikp/concept/solution_traits.hpp"
#include "bikp/tool/second_iterator.hpp"
#include "bikp/tool/types.hpp"

#include <map>

/**
 * \brief A set of non-dominated bi solutions, without equivalent ones.
 *
 * \b Template \b arguments :
 * - \a Solution : The type of the stored solutions.
 * - \a SolutionTraits : The type of the traits on the solutions.
 *
 * \pre SolutionTraits::to_point( SolutionTraits ) returns a type convertible to
 *      bi_profit.
 * \author Julien Jorge
 */
template <class Solution, class SolutionTraits = solution_traits<Solution> >
class bi_minimum_complete_set
{
public:
  /** \brief The type of the stored solutions. */
  typedef Solution value_type;

  /** \brief The type of the traits on the solutions. */
  typedef SolutionTraits solution_traits_type;

  /** \brief The type of the current class. */
  typedef bi_minimum_complete_set<value_type, solution_traits_type> self_type;

  /** \brief The container in which we store the solutions. */
  typedef std::map<real_type, value_type> solution_map;

  /** \brief Iterator on the solutions. */
  typedef
  const_second_iterator<typename solution_map::const_iterator> const_iterator;

  /** \brief The type of the image of the solution in the objective space. */
  typedef typename solution_traits_type::profit_type point_type;

private:
  /** \brief Iterator in the solution_map. */
  typedef typename solution_map::iterator solution_map_iterator;

public:

  bool insert( const value_type& p );
  //void insert( const self_type& that );
  template<typename Iterator>
  void insert( Iterator first, Iterator last );

  bool is_dominated( const value_type& sol ) const;
  bool is_dominated_neq( const value_type& sol ) const;
  //bool is_conv_dominated_neq( const value_type& sol ) const;

  //bool is_dominated( const self_type& s ) const;
  bool is_dominated_neq( const self_type& s ) const;
  //bool is_conv_dominated_neq( const self_type& s ) const;

  bool contains( const value_type& sol ) const;

  real_type size() const;
  bool empty() const;
  const_iterator begin() const;
  const_iterator end() const;

  void clear();

private:
  void remove_dominated( solution_map_iterator it );

private:
  /** \brief The values of not dominated solutions. */
  solution_map m_solutions;

}; // class bi_minimum_complete_set

#include "bikp/concept/impl/bi_minimum_complete_set.tpp"

#endif // __BIKP_BI_MINIMUM_COMPLETE_SET_HPP__
