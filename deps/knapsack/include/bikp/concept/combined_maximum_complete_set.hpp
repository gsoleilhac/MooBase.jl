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
 * \file combined_maximum_complete_set.hpp
 * \brief A set of non-dominated combined solutions, with equivalent ones.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_MAXIMUM_COMPLETE_SET_HPP__
#define __BIKP_COMBINED_MAXIMUM_COMPLETE_SET_HPP__

#include "bikp/concept/solution_traits.hpp"

#include <map>
#include <list>


/**
 * \brief A set of non-dominated combined solutions, with equivalent ones.
 *
 * \b Template \b arguments :
 * - \a Solution : The type of the stored solutions.
 * - \a SolutionTraits : The type of the traits on the solutions.
 *
 * \pre SolutionTraits::to_point( SolutionTraits ) returns a type convertible to
 *      combined_profit.
 * \author Julien Jorge
 */
template< class Solution, class SolutionTraits = solution_traits<Solution> >
class combined_maximum_complete_set
{
public:
  /** \brief The type of the stored solutions. */
  typedef Solution value_type;

  /** \brief The type of the traits on the solutions. */
  typedef SolutionTraits solution_traits_type;

  /** \brief The type of the current class. */
  typedef combined_maximum_complete_set<Solution, SolutionTraits> self_type;

  /** \brief The container in which we store the equivalent solutions. */
  typedef std::list<value_type> solution_list;

  /** \brief The container in which we store the solutions. */
  typedef std::map<real_type, solution_list> solution_map;

  /** \brief Iterator on the solutions. */
  class const_iterator
  {
  private:
    typedef typename solution_map::const_iterator map_iterator;
    typedef typename solution_list::const_iterator list_iterator;

  public:
    typedef typename list_iterator::reference reference;
    typedef typename list_iterator::pointer pointer;

  public:
    
    typedef std::input_iterator_tag iterator_category;

    const_iterator()
    { }

    const_iterator(map_iterator it)
      : m_map_it(it)
    {
      if ( m_map_it != map_iterator() )
	m_list_it = m_map_it->second.begin();
    }
    
    bool operator==( const const_iterator& that ) const
    {
      bool result = false;
      
      if ( m_map_it == that.m_map_it )
	{
	  if ( m_map_it == map_iterator() )
	    result = true;
	  else
	    result = (m_list_it == that.m_list_it);
	}
      
      return result;
    }

    bool operator!=( const const_iterator& that ) const
    {
      return !( *this == that );
    }

    reference operator*() const { return *m_list_it; }
    pointer operator->() const { return m_list_it.operator->(); }

    const_iterator& operator++()
    {
      ++m_list_it;

      if ( m_list_it == m_map_it->second.end() )
	{
	  ++m_map_it;
	  if ( m_map_it != map_iterator() )
	    m_list_it = m_map_it->second.begin();
	}

      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

  private:
    /** \brief Iterator in the map of lists. */
    map_iterator m_map_it;

    /** \brief Iterator in the list of solutions. */
    list_iterator m_list_it;

  }; // class const_iterator

  /** \brief The type of the image of the solution in the objective space. */
  typedef typename solution_traits_type::profit_type point_type;

private:
  /** \brief Iterator in the solution_map. */
  typedef typename solution_map::iterator solution_map_iterator;

  /** \brief Iterator in the solution_list. */
  typedef typename solution_list::const_iterator const_solution_list_iterator;

public:

  bool insert( const value_type& sol );
  //void insert( const self_type& that );
  template<typename Iterator>
  void insert( Iterator first, Iterator last );

  bool is_dominated( const value_type& p ) const;
  real_type size() const;
  bool empty() const;
  const_iterator begin() const;
  const_iterator end() const;
  void clear();

private:
  void remove_dominated( solution_map_iterator it );
  bool is_present( const value_type& sol, const_solution_list_iterator first,
		   const_solution_list_iterator last ) const;

private:
  /** \brief The values of not dominated solutions. */
  solution_map m_solutions;

}; // class combined_maximum_complete_set

#include "bikp/concept/impl/combined_maximum_complete_set.tpp"

#endif // __BIKP_COMBINED_MAXIMUM_COMPLETE_SET_HPP__
