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
 * \file base_variable_fixing.tpp
 * \brief Implementation of the base_variable_fixing class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sub problem.
 */
template<typename Problem>
const typename base_variable_fixing<Problem>::problem_type&
base_variable_fixing<Problem>::get_sub_problem() const
{
  assert(m_sub_problem);

  return *m_sub_problem;
} // base_variable_fixing::get_sub_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sum of the profit of the variables fixed in the solution.
 */
template<typename Problem>
const typename base_variable_fixing<Problem>::profit_type&
base_variable_fixing<Problem>::guaranteed_profit() const
{
  return m_guaranteed_profit;
} // base_variable_fixing::get_sub_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a solution of the sub problem to a solution of the real
 *        problem.
 * \param sub_sol The solution of the sub problem.
 * \param sol (out) The resulting solution.
 */
template<typename Problem>
void base_variable_fixing<Problem>::restore_solution
( const solution_type& sub_sol, solution_type& sol ) const
{
  assert( sub_sol.is_full() );
  assert( sub_sol.is_feasible() );
  assert( sub_sol.size() + m_set_variables.size() + m_unset_variables.size()
	  == sol.size() );

  typename fixed_set_type::const_iterator it;

  sol.clear();

  for (it=m_set_variables.begin(); it!=m_set_variables.end(); ++it)
    sol.set( *it );

  for (it=m_unset_variables.begin(); it!=m_unset_variables.end(); ++it)
    sol.unset( *it );

  for (unsigned int i=0; i!=m_sub_problem->size(); ++i)
    if ( sub_sol.is_set(i) )
      sol.set( m_correspondence[i] );
    else if ( sub_sol.is_unset(i) )
      sol.unset( m_correspondence[i] );

  assert( sol.is_feasible() );
  assert( sol.is_full() );
} // base_variable_fixing::restore_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a solution of the sub problem from a solution of the initial
 *        problem.
 * \param sol The solution to convert.
 * \param sub_sol The resulting solution.
 */
template<typename Problem>
void base_variable_fixing<Problem>::create_solution
( const solution_type& sol, solution_type& sub_sol ) const
{
  assert( sol.is_full() );
  assert( sol.is_feasible() );
  assert( sub_sol.size() + m_set_variables.size() + m_unset_variables.size()
	  == sol.size() );

  sub_sol.clear();

  unsigned int index=0;

  for (unsigned int i=0; i!=sol.size(); ++i)
    if ( m_set_variables.find(i) == m_set_variables.end() )
      {
	if ( m_unset_variables.find(i) == m_unset_variables.end() )
	  {
	    if ( sol.is_set(i) )
	      sub_sol.set( index );
	    else
	      sub_sol.unset( index );
	    
	    ++index;
	  }
	else
	  {
	    assert( sol.is_unset(i) );
	  }
      }
    else
      {
	assert( sol.is_set(i) );
      }

  assert( sub_sol.is_feasible() );
  assert( sub_sol.is_full() );
} // base_variable_fixing::create_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add to the current reduction the variables set by a reduction of the
 *        sub problem.
 * \param sub_fixing The reduction of the sub problem.
 * \remark Call to this method invalidates all pointers to the sub problem.
 * \pre The order of the variables is the same in \a this and \a sub_fixing.
 */
template<typename Problem>
template<typename P>
void base_variable_fixing<Problem>::absorb
( const base_variable_fixing<P>& sub_fixing )
{
  m_set_variables.insert( sub_fixing.set_variables().begin(),
			  sub_fixing.set_variables().end() );

  m_unset_variables.insert( sub_fixing.unset_variables().begin(),
			    sub_fixing.unset_variables().end() );

  create_sub_problem();
} // base_variable_fixing::absorb()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the set of variables that will be, for sure, in the solution.
 */
template<typename Problem>
const
typename base_variable_fixing<Problem>::fixed_set_type&
base_variable_fixing<Problem>::set_variables() const
{
  return m_set_variables;
} // base_variable_fixing::set_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the set of variables that will be, for sure, out of the solution.
 */
template<typename Problem>
const
typename base_variable_fixing<Problem>::fixed_set_type&
base_variable_fixing<Problem>::unset_variables() const
{
  return m_unset_variables;
} // base_variable_fixing::unset_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 */
template<typename Problem>
base_variable_fixing<Problem>::base_variable_fixing
( const problem_type& p )
  : m_problem(p), m_logger("base_variable_fixing"), m_sub_problem(NULL)
{
  m_guaranteed_profit *= 0;
  create_sub_problem();
} // base_variable_fixing::base_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename Problem>
base_variable_fixing<Problem>::~base_variable_fixing()
{
  delete m_sub_problem;
} // base_variable_fixing::~base_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sub problem.
 */
template<typename Problem>
void base_variable_fixing<Problem>::create_sub_problem()
{
  if (m_sub_problem)
    {
      delete m_sub_problem;
      m_sub_problem = NULL;
    }

  fixed_set_type fixed;
  typename problem_type::weight_type needed_capacity = merge(fixed);

  assert( m_problem.capacity() >= needed_capacity );

  m_sub_problem = new problem_type
    ( m_problem.size() - fixed.size(), m_problem.capacity() - needed_capacity );

  make_indices( fixed );

  for (unsigned int i=0; i!=m_sub_problem->size(); ++i)
    {
      const unsigned int real_i = m_correspondence[i];

      m_sub_problem->set_variable
	( i, m_problem.profit(real_i), m_problem.weight(real_i) );
    }
} // base_variable_fixing::create_sub_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Merge the sets of fixed variables.
 * \param fixed (out) The result of the merge.
 * \return The capacity consumed by the fixed variables.
 */
template<typename Problem>
typename base_variable_fixing<Problem>::weight_type
base_variable_fixing<Problem>::merge( fixed_set_type& fixed )
{
  typename problem_type::weight_type needed_capacity;
  typename fixed_set_type::const_iterator it;

  needed_capacity *= 0;
  m_guaranteed_profit *= 0;

  for ( it = m_set_variables.begin(); it != m_set_variables.end(); ++it )
    {
      needed_capacity += m_problem.weight(*it);
      m_guaranteed_profit += m_problem.profit(*it);
    }

  fixed.insert( m_set_variables.begin(), m_set_variables.end() );
  fixed.insert( m_unset_variables.begin(), m_unset_variables.end() );

  return needed_capacity;
} // base_variable_fixing::merge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the table of correspondence of indices between the current
 *        problem and the real problem.
 * \param unwanted Indices of the removed variables.
 */
template<typename Problem>
void
base_variable_fixing<Problem>::make_indices( const fixed_set_type& unwanted )
{
  assert( m_sub_problem != NULL );

  typename fixed_set_type::const_iterator it = unwanted.begin();
  unsigned int index = 0;

  m_correspondence.resize( m_sub_problem->size() );

  for (unsigned int i=0; i!=m_correspondence.size(); ++i, ++index)
    {
      bool stop = false;

      while ( (it != unwanted.end()) && !stop )
	if ( *it == index )
	  {
	    ++index;
	    ++it;
	  }
	else
	  stop = true;
  
      m_correspondence[i] = index;
    }
} // base_variable_fixing::make_indices()
