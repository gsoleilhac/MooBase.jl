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
 * \file mono_maximum_complete_set.tpp
 * \brief Implementation of the mono_maximum_complete_set class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set.
 * \param sol The solution to add.
 */
template<class Solution, class SolutionTraits>
bool mono_maximum_complete_set<Solution, SolutionTraits>::insert
( const value_type& sol )
{
  bool result = true;

  if ( !is_dominated(sol) )
    {
      if ( solution_traits_type::better(sol, m_solutions.front()) )
	m_solutions.clear();

      m_solutions.push_front(sol);
    }
  else
    result = false;

  return result;
} // mono_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the solutions of an other set in the current set.
 * \param that The set from which we take the solutions.
 */
/*template<class Solution, class SolutionTraits>
void mono_maximum_complete_set<Solution, SolutionTraits>::insert
( const self_type& that )
{
  if ( !that.empty() )
    insert( *that.m_solution );
}*/ // mono_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a range of solutions in the current set.
 * \param first Iterator on the first solution in the set.
 * \param last Iterator past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
template<class Iterator>
void mono_maximum_complete_set<Solution, SolutionTraits>::insert
( Iterator first, Iterator last )
{
  for (; first != last; ++first)
    insert(*first);
} // mono_maximum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is dominated by a solution from the current set.
 * \param p The solution to test.
 */
template<class Solution, class SolutionTraits>
bool mono_maximum_complete_set<Solution, SolutionTraits>::is_dominated
( const value_type& p ) const
{
  bool result = false;

  typename set_type::const_iterator it = m_solutions.begin();
  typename set_type::const_iterator eit = m_solutions.end();

  for ( ; !result && (it!=eit); ++it)
    result = solution_traits_type::better_same( *it, p );

  return result;
} // mono_maximum_complete_set::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of solutions in the set.
 */
template<class Solution, class SolutionTraits>
unsigned int mono_maximum_complete_set<Solution, SolutionTraits>::size() const
{
  return m_solutions.size();
} // mono_maximum_complete_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no solutions in the set.
 */
template<class Solution, class SolutionTraits>
bool mono_maximum_complete_set<Solution, SolutionTraits>::empty() const
{
  return m_solutions.empty();
} // mono_maximum_complete_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first solution of the set.
 */
template<class Solution, class SolutionTraits>
typename mono_maximum_complete_set<Solution, SolutionTraits>::const_iterator
mono_maximum_complete_set<Solution, SolutionTraits>::begin() const
{
  return m_solutions.begin();
} // mono_maximum_complete_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
typename mono_maximum_complete_set<Solution, SolutionTraits>::const_iterator
mono_maximum_complete_set<Solution, SolutionTraits>::end() const
{
  return m_solutions.end();
} // mono_maximum_complete_set::end()
