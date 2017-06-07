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
 * \file combined_maximum_complete_set.tpp
 * \brief Implementation of the combined_maximum_complete_set class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set.
 * \param sol The solution to add.
 *
 * The solutions are stored in the map, according to the their values on the
 * first objective. So when we add a solution \a sol, we just need to find in
 * the map the solution on his right. If the found solution is better on both
 * objectives then \a sol is dominated.
 *
 * Otherwise, we add \a sol and we remove the solutions on his left that become
 * dominated.
 */
template<class Solution, class SolutionTraits>
bool combined_maximum_complete_set<Solution, SolutionTraits>::insert
( const value_type& sol )
{
  bool result = true;

  point_type p = solution_traits_type::to_profit(sol);
  solution_list sol_list;
  sol_list.push_front(sol);

  if ( empty() )
    m_solutions[ p.profit_1 ] = sol_list;
  else
    {
      solution_map_iterator it = m_solutions.lower_bound( p.profit_1 );
      solution_map_iterator eit = m_solutions.end();

      // no solutions on its right
      if ( it == eit )
	remove_dominated
	  ( m_solutions.insert(eit, std::make_pair(p.profit_1, sol_list)) );
      else if ( !solution_traits_type::better(it->second.front(), sol) )
	{
	  point_type s = solution_traits_type::to_profit(it->second.front());

	  if ( solution_traits_type::equal(it->second.front(), sol) )
	    {
	      if ( !is_present( sol, it->second.begin(), it->second.end() ) )
		it->second.push_front(sol);
	      else
		result = false;
	    }
	  else if ( s.profit_1 == p.profit_1 )
	    {
	      it->second = sol_list;
	      remove_dominated(it);
	    }
	  else
	    remove_dominated
	      ( m_solutions.insert(it, std::make_pair(p.profit_1, sol_list)) );
	}
      else
	result = false;
    }

  return result;
} // combined_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the solutions of an other set in the current set.
 * \param that The set from which we take the solutions.
 */
/*template<class Solution, class SolutionTraits>
void combined_maximum_complete_set<Solution, SolutionTraits>::insert( const self_type& that )
{
  solution_map::const_iterator it = that.begin();
  solution_map::const_iterator eit = that.end();

  for (; it != eit; ++it)
    insert( it->second );
}*/ // combined_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a range of solutions in the current set.
 * \param first Iterator on the first solution in the set.
 * \param last Iterator past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
template<class Iterator>
void combined_maximum_complete_set<Solution, SolutionTraits>::insert
( Iterator first, Iterator last )
{
  for (; first != last; ++first)
    insert(*first);
} // combined_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is dominated by a solution from the current set.
 * \param sol The solution to test.
 */
template<class Solution, class SolutionTraits>
bool combined_maximum_complete_set<Solution, SolutionTraits>::is_dominated
( const value_type& sol ) const
{
  bool result = false;

  if ( !empty() )
    {
      point_type p = solution_traits_type::to_profit(sol);
      typename solution_map::const_iterator it
	( m_solutions.lower_bound(p.profit_1) );

      if ( it != m_solutions.end() )
	{
	  typename solution_list::const_iterator it_s = it->second.begin();

	  if ( solution_traits_type::better(*it_s, sol) )
	    result = true;
	  else if ( solution_traits_type::equal(*it_s, sol) )
	    result = is_present(sol, it_s, it->second.end());
	}
    }

  return result;
} // combined_maximum_complete_set::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of solutions in the set.
 */
template<class Solution, class SolutionTraits>
real_type
combined_maximum_complete_set<Solution, SolutionTraits>::size() const
{
  real_type result = 0;

  typename solution_map::const_iterator it = m_solutions.begin();
  typename solution_map::const_iterator eit = m_solutions.end();

  for ( ; it!=eit; ++it )
    result += it->second.size();

  return result;
} // combined_maximum_complete_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no solutions in the set.
 */
template<class Solution, class SolutionTraits>
bool combined_maximum_complete_set<Solution, SolutionTraits>::empty() const
{
  return m_solutions.empty();
} // combined_maximum_complete_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first solution of the set.
 */
template<class Solution, class SolutionTraits>
typename combined_maximum_complete_set<Solution, SolutionTraits>::const_iterator
combined_maximum_complete_set<Solution, SolutionTraits>::begin() const
{
  return const_iterator( m_solutions.begin() );
} // combined_maximum_complete_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
typename combined_maximum_complete_set<Solution, SolutionTraits>::const_iterator
combined_maximum_complete_set<Solution, SolutionTraits>::end() const
{
  return const_iterator( m_solutions.end() );
} // combined_maximum_complete_set::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the solutions from the set.
 */
template<class Solution, class SolutionTraits>
void combined_maximum_complete_set<Solution, SolutionTraits>::clear()
{
  m_solutions.clear();
} // combined_maximum_complete_set::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the solutions dominated by a given solution.
 * \param it Iterator on the dominator.
 */
template<class Solution, class SolutionTraits>
void combined_maximum_complete_set<Solution, SolutionTraits>::remove_dominated
( solution_map_iterator it )
{
  const value_type ref(it->second.front());
  const solution_map_iterator beg( m_solutions.begin() );
  std::list<solution_map_iterator> remove_us;

  bool stop = false;

  // find the dominated solutions
  while ( (it != beg) && !stop )
    {
      --it;

      if ( solution_traits_type::better(ref, it->second.front()) )
	remove_us.push_front(it);
      else
	stop = true;
    }

  // effectively remove the dominated solutions
  typename std::list<solution_map_iterator>::const_iterator rit;
  typename std::list<solution_map_iterator>::const_iterator erit
    ( remove_us.end() );

  for (rit=remove_us.begin(); rit!=erit; ++rit)
    m_solutions.erase(*rit);
} // combined_maximum_complete_set::remove_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is already present in the set.
 * \param sol The solution we are searching for.
 * \param first Iterator on the first solution of the set.
 * \param last Iterator just past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
bool combined_maximum_complete_set<Solution, SolutionTraits>::is_present
( const value_type& sol, const_solution_list_iterator first,
  const_solution_list_iterator last ) const
{
  bool result = false;

  for (; !result && (first != last); ++first)
    result = solution_traits_type::same(*first, sol);

  return result;
} // combined_maximum_complete_set::is_present()
