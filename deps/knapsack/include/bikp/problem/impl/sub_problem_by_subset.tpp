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
 * \file sub_problem_by_subset.tpp
 * \brief Implementation of the sub_problem_by_subset class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The parent problem.
 * \param vars The indices of the variables out of the problem.
 * \param cap Maximum capacity.
 * \param keep Tell if the indices in \a vars are the variables you want to
 *        keep.
 */
template<typename Problem>
sub_problem_by_subset<Problem>::sub_problem_by_subset
( const problem_type& p, const set_of_variables_type& vars, weight_type cap,
  bool keep )
  : m_problem(p), m_capacity(cap)
{
  assert( cap >= 0 );

  if (keep)
    {
      m_correspondence.resize( vars.size() );
      copy_indices( vars );
    }
  else
    {
      m_correspondence.resize( m_problem.size() - vars.size() );
      make_indices( vars );
    }
} // sub_problem_by_subset::sub_problem_by_subset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The parent problem.
 * \param vars The indices of the variables out of the problem.
 * \param keep Tell if the indices in \a vars are the variables you want to
 *        keep.
 */
template<typename Problem>
sub_problem_by_subset<Problem>::sub_problem_by_subset
( const problem_type& p, const set_of_variables_type& vars, bool keep )
  : m_problem(p), m_capacity(0)
{
  if (keep)
    {
      m_correspondence.resize( vars.size() );
      copy_indices( vars );
    }
  else
    {
      m_correspondence.resize( m_problem.size() - vars.size() );
      make_indices( vars );
    }
} // sub_problem_by_subset::sub_problem_by_subset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the problem.
 */
template<typename Problem>
unsigned int sub_problem_by_subset<Problem>::size() const
{
  return m_correspondence.size();
} // sub_problem_by_subset::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum capacity.
 */
template<typename Problem>
typename sub_problem_by_subset<Problem>::weight_type
sub_problem_by_subset<Problem>::capacity() const
{
  return m_capacity;
} // sub_problem_by_subset::capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum capacity.
 * \param c The new capacity.
 */
template<typename Problem>
void sub_problem_by_subset<Problem>::set_capacity( weight_type c )
{
  m_capacity = c;
} // sub_problem_by_subset::setcapacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the real index of a variable.
 * \param index Index of the variable in the current problem.
 */
template<typename Problem>
unsigned int
sub_problem_by_subset<Problem>::real_index( unsigned int index ) const
{
  assert( index < size() );

  return m_correspondence[index];
} // sub_problem_by_subset::real_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the full problem.
 */
template<typename Problem>
const typename sub_problem_by_subset<Problem>::problem_type&
sub_problem_by_subset<Problem>::get_problem() const
{
  return m_problem;
} // sub_problem_by_subset::get_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the table of correspondence of indices between the current
 *        problem and the real problem.
 * \param unwanted Indices of the removed variables.
 */
template<typename Problem>
void
sub_problem_by_subset<Problem>::make_indices
( const set_of_variables_type& unwanted )
{
  typename set_of_variables_type::const_iterator it = unwanted.begin();
  unsigned int index = 0;

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
} // sub_problem_by_subset::make_indices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the table of correspondence of indices between the current
 *        problem and the real problem.
 * \param wanted Indices of the kept variables.
 */
template<typename Problem>
void
sub_problem_by_subset<Problem>::copy_indices
( const set_of_variables_type& wanted )
{
  std::copy( wanted.begin(), wanted.end(), m_correspondence.begin() );
} // sub_problem_by_subset::copy_indices()

