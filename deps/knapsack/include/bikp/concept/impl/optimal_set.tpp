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
 * \file optimal_set.tpp
 * \brief Implementation of the optimal_set class.
 * \author Julien Jorge
 */
#include <cassert>

/*---------------------------------------------------------------------------*/
/**
 * \brief Default Constructor.
 * \param multi_set Tell if we must keep solutions of same quality.
 */
template<class Solution, class SolutionTraits>
optimal_set<Solution, SolutionTraits>::optimal_set( bool multi_set )
  : m_multi_set(multi_set)
{
  // nothing to do.
} // optimal_set::optimal_set()

/*---------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The set to copy from.
 */
template<class Solution, class SolutionTraits>
optimal_set<Solution, SolutionTraits>::optimal_set( const self_type& that )
  : m_multi_set(that.m_multi_set)
{
  typename set_type::const_reverse_iterator it;

  for (it=that.m_solutions.rbegin(); it!=that.m_solutions.rend(); ++it)
    m_solutions.push_front(*it);
} // optimal_set::optimal_set()

/*---------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<class Solution, class SolutionTraits>
optimal_set<Solution, SolutionTraits>::~optimal_set()
{
  clear();
} // optimal_set::~optimal_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The item to copy from.
 */
template<class Solution, class SolutionTraits>
typename optimal_set<Solution, SolutionTraits>::self_type&
optimal_set<Solution, SolutionTraits>::operator=( const self_type& that )
{
  m_solutions = that.m_solutions;
  return *this;
} // optimal_set::operator=()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set.
 *
 * The solution is added if and only if she's not dominated by another solution
 * in the set. Then, dominated solutions are removed from the set.
 *
 * \param sol The solution to insert.
 */
template<class Solution, class SolutionTraits>
bool optimal_set<Solution, SolutionTraits>::insert( const solution_type& sol )
{
  typedef std::list< typename set_type::iterator > del_list;

  del_list remove_us;
  typename del_list::iterator remove_it;
  typename set_type::iterator it;
  // Do not add the solution if she's dominated or already in the buffer.
  bool dont_add = false;
  bool stop = false;
  typename solution_traits_type::less comp;

  if ( m_multi_set )
    for ( it=m_solutions.begin();
	  (it!=m_solutions.end()) && !dont_add && !stop;
	  ++it )
      {
	if ( solution_traits_type::same(sol, *it) )
	  dont_add = true;
	else if ( solution_traits_type::better(sol, *it) )
	  remove_us.push_front( it );
	else if ( solution_traits_type::better(*it, sol) )
	  dont_add = true;
	else if ( comp(sol, *it) )
	  stop = true;
      }
  else
    for ( it=m_solutions.begin();
	  (it!=m_solutions.end()) && !dont_add && !stop;
	  ++it )
    if ( solution_traits_type::equal(sol, *it) )
      dont_add = true;
    else if ( solution_traits_type::better(sol, *it) )
      remove_us.push_front( it );
    else if ( solution_traits_type::better(*it, sol) )
      dont_add = true;
    else if ( comp(sol, *it) )
      stop = true;

  if (!dont_add)
    {
      for (remove_it=remove_us.begin(); remove_it!=remove_us.end(); ++remove_it)
	m_solutions.erase( *remove_it );

      if (stop)
	--it;
      m_solutions.insert( it, sol );
    }
  else
    { assert(remove_us.empty()); }

  return !dont_add;
} // optimal_set::insert()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add the solutions of an other set.
 * \param that The set to add.
 */
template<class Solution, class SolutionTraits>
void optimal_set<Solution, SolutionTraits>::insert( const self_type& that )
{
  insert( that.begin(), that.end() );
} // optimal_set::insert()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add a range of solutions.
 * \param first Iterator on the first solution to add.
 * \param last Iterator just past the last solution to add.
 */
template<class Solution, class SolutionTraits>
template<typename Iterator>
void optimal_set<Solution, SolutionTraits>::insert
( Iterator first, Iterator last )
{
  for( ; first!=last; ++first )
    insert( *first );
} // optimal_set::insert()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is equal or is dominated by at least one solution
 *        in the set.
 * \param sol The solution to test.
 */
template<class Solution, class SolutionTraits>
bool optimal_set<Solution, SolutionTraits>::is_dominated
( const solution_type& sol ) const
{
  enum stop_condition{ stop_dominated, stop_better, stop_end, stop_none };
  
  stop_condition result = stop_none;
  
  typename set_type::const_iterator it;
  typename solution_traits_type::less comp;

  for ( it=m_solutions.begin();
	(it!=m_solutions.end()) && (result==stop_none);
	++it)
    if ( (!m_multi_set && solution_traits_type::better_equal( *it, sol )) ||
	 (m_multi_set && solution_traits_type::better_same( *it, sol )) )
      result = stop_dominated;
    else if ( solution_traits_type::better(sol, *it) )
      result = stop_better;
    else if ( comp(sol, *it) )
      result = stop_end;

  return result == stop_dominated;
} // optimal_set::is_dominated()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get The size of the set.
 */
template<class Solution, class SolutionTraits>
real_type optimal_set<Solution, SolutionTraits>::size() const
{
  return m_solutions.size();
} // optimal_set::size()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if the set is empty.
 */
template<class Solution, class SolutionTraits>
bool optimal_set<Solution, SolutionTraits>::empty() const
{
  return m_solutions.empty();
} // optimal_set::empty()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first solution.
 */
template<class Solution, class SolutionTraits>
typename optimal_set<Solution, SolutionTraits>::const_iterator
optimal_set<Solution, SolutionTraits>::begin() const
{
  return m_solutions.begin();
} // optimal_set::begin()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last solution.
 */
template<class Solution, class SolutionTraits>
typename optimal_set<Solution, SolutionTraits>::const_iterator
optimal_set<Solution, SolutionTraits>::end() const
{
  return m_solutions.end();
} // optimal_set::end()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if this set keep solutions with the same quality.
 */
template<class Solution, class SolutionTraits>
bool optimal_set<Solution, SolutionTraits>::is_multi_set() const
{
  return m_multi_set;
} // optimal_set::is_multi_set()

/*---------------------------------------------------------------------------*/
/**
 * \brief Remove all solutions.
 */
template<class Solution, class SolutionTraits>
void optimal_set<Solution, SolutionTraits>::clear()
{
  m_solutions.clear();
} // optimal_set::clear()
