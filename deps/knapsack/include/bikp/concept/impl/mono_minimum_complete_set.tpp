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
 * \file mono_minimum_complete_set.tpp
 * \brief Implementation of the mono_minimum_complete_set class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Solution, class SolutionTraits>
mono_minimum_complete_set<Solution, SolutionTraits>::mono_minimum_complete_set()
  : m_solution(NULL)
{

} // mono_minimum_complete_set::mono_minimum_complete_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The set to copy from.
 */
template<class Solution, class SolutionTraits>
mono_minimum_complete_set<Solution, SolutionTraits>::mono_minimum_complete_set
( const self_type& that )
{
  if (that.m_solution != NULL)
    m_solution = new value_type(*that.m_solution);
  else
    m_solution = NULL;
} // mono_minimum_complete_set::mono_minimum_complete_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<class Solution, class SolutionTraits>
mono_minimum_complete_set<Solution, SolutionTraits>::~mono_minimum_complete_set
()
{
  if ( m_solution != NULL )
    delete m_solution;
} // mono_minimum_complete_set::~mono_minimum_complete_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The set to copy from.
 */
template<class Solution, class SolutionTraits>
typename mono_minimum_complete_set<Solution, SolutionTraits>::self_type&
mono_minimum_complete_set<Solution, SolutionTraits>::operator=
( const self_type& that )
{
  if (m_solution == NULL)
    {
      if (that.m_solution != NULL)
	m_solution = new value_type(*that.m_solution);
    }
  else if (that.m_solution == NULL)
    {
      delete m_solution;
      m_solution = NULL;
    }
  else // (m_solution != NULL) && (that.m_solution != NULL)
    *m_solution = *that.m_solution;

  return *this;
} // mono_minimum_complete_set::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set.
 * \param sol The solution to add.
 */
template<class Solution, class SolutionTraits>
bool mono_minimum_complete_set<Solution, SolutionTraits>::insert
( const value_type& sol )
{
  bool result = true;

  if ( m_solution == NULL )
    m_solution = new value_type(sol);
  else if ( solution_traits_type::better(sol, *m_solution) )
    *m_solution = sol;
  else
    result = false;

  return result;
} // mono_minimum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the solutions of an other set in the current set.
 * \param that The set from which we take the solutions.
 */
/*template<class Solution, class SolutionTraits>
void mono_minimum_complete_set<Solution, SolutionTraits>::insert
( const self_type& that )
{
  if ( !that.empty() )
    insert( *that.m_solution );
    }*/ // mono_minimum_complete_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution is dominated by a solution from the current set.
 * \param p The solution to test.
 */
template<class Solution, class SolutionTraits>
bool mono_minimum_complete_set<Solution, SolutionTraits>::is_dominated
( const value_type& p ) const
{
  if ( empty() )
    return false;
  else
    return solution_traits_type::better_equal( *m_solution, p );
} // mono_minimum_complete_set::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of solutions in the set.
 */
template<class Solution, class SolutionTraits>
real_type mono_minimum_complete_set<Solution, SolutionTraits>::size() const
{
  if ( empty() )
    return 0;
  else
    return 1;
} // mono_minimum_complete_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no solutions in the set.
 */
template<class Solution, class SolutionTraits>
bool mono_minimum_complete_set<Solution, SolutionTraits>::empty() const
{
  return m_solution == NULL;
} // mono_minimum_complete_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first solution of the set.
 */
template<class Solution, class SolutionTraits>
typename mono_minimum_complete_set<Solution, SolutionTraits>::const_iterator
mono_minimum_complete_set<Solution, SolutionTraits>::begin() const
{
  if ( empty() )
    return NULL;
  else
    return m_solution;
} // mono_minimum_complete_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last solution of the set.
 */
template<class Solution, class SolutionTraits>
typename mono_minimum_complete_set<Solution, SolutionTraits>::const_iterator
mono_minimum_complete_set<Solution, SolutionTraits>::end() const
{
  if ( empty() )
    return NULL;
  else
    return m_solution + 1;
} // mono_minimum_complete_set::end()
