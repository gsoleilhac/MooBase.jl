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
 * \file solution.tpp
 * \brief Implementation of the solution class.
 * \author Julien Jorge
 */
#include <cassert>
#include <algorithm>
#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the objective value of a solution in a standard output stream.
 * \param os The stream to write in.
 * \param sol The solution to write.
 */
template<typename Problem>
std::ostream& operator<<( std::ostream& os, const solution<Problem>& sol )
{
  os << "(sol) " << sol.objective_value();

  return os;
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 *
 * This constructor is implemeted ony for convenience. Use with care. The
 * problem is not stored so most of the methods will sigfault.
 */
template<typename Problem>
solution<Problem>::solution()
  : m_problem(NULL)
{

} // solution::solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem for which we are a solution.
 */
template<typename Problem>
solution<Problem>::solution( const problem_type& p )
  : m_problem(&p), m_variables( p.size(), var_free )
{
  m_objective_value *= 0;
  m_weight *= 0;
} // solution::solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The solution to copy from.
 */
/*template<typename Problem>
typename solution<Problem>::self_type&
solution<Problem>::operator=( const self_type& that )
{
  assert( that.is_feasible() );
  assert( that.size() == size() );

  std::copy( that.m_variables.begin(), that.m_variables.end(),
	     m_variables.begin() );

  m_objective_value = that.m_objective_value;
  m_weight = that.m_weight;

  assert( is_feasible() );
  assert( that.is_full() ? is_full() : true );

  return *this;
}*/ // solution::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two solutions by their vectors.
 * \param that The solution to compare to.
 */
template<typename Problem>
bool solution<Problem>::operator==( const self_type& that ) const
{
  assert( m_problem != NULL );
  assert( that.m_problem != NULL );
  assert( that.size() == size() );

  bool result = (m_objective_value == that.m_objective_value)
    && (m_weight == that.m_weight);

  if (result)
    result = std::equal( that.m_variables.begin(), that.m_variables.end(),
			 m_variables.begin() );

  return result;
} // solution::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two solutions by their vectors.
 * \param that The solution to compare to.
 */
template<typename Problem>
bool solution<Problem>::operator!=( const self_type& that ) const
{
  return !( *this == that );
} // solution::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic compararison of two solutions by their value then by
 *        their weights, then by their vectors.
 * \param that The solution to compare to.
 */
template<typename Problem>
bool solution<Problem>::operator<( const self_type& that ) const
{
  assert( that.size() == size() );

  bool result;

  if ( profit_traits_type::equal(m_objective_value, that.m_objective_value) )
    {
      if ( m_weight == that.m_weight )
	result = m_variables < that.m_variables;
      else
	result = m_weight < that.m_weight;
    }
  else
    result =
      profit_traits_type::lex_less(m_objective_value, that.m_objective_value);

  return result;
} // solution::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the objective function.
 */
template<typename Problem>
const typename solution<Problem>::profit_type&
solution<Problem>::objective_value() const
{
  return m_objective_value;
} // solution::objective_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the objective function.
 */
template<typename Problem>
solution<Problem>::operator const profit_type&() const
{
  return objective_value();
} // solution::operator profit_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the weight of the solution.
 */
template<typename Problem>
const typename solution<Problem>::weight_type& solution<Problem>::weight() const
{
  return m_weight;
} // solution::weight()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the solution.
 * \param index Index of the variable to add.
 * \pre The status of item \a index is undefined.
 * \pre weight() + problem.weight(index) <= problem.capacity()
 */
template<typename Problem>
void solution<Problem>::set( unsigned int index )
{
  assert( index < m_problem->size() );
  assert( is_free(index) );
  assert( weight() + m_problem->weight(index) <= m_problem->capacity() );

  m_variables[index] = var_set;
  
  m_objective_value += m_problem->profit( index );
  m_weight += m_problem->weight( index );

  assert( is_feasible() );
} // solution::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set an item out of the solution.
 * \param index Index of the variable to set.
 * \pre The status of item \a index is undefined.
 */
template<typename Problem>
void solution<Problem>::unset( unsigned int index )
{
  assert( index < m_problem->size() );
  assert( is_free(index) );

  m_variables[index] = var_unset;

  assert( is_feasible() );
} // solution::unset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Undefine the status of a variable.
 * \param index Index of the variable to free.
 * \pre Item \a index has been set() or unset().
 */
template<typename Problem>
void solution<Problem>::free( unsigned int index )
{
  assert( index < m_problem->size() );
  assert( !is_free(index) );

  if ( m_variables[index] == var_set )
    {
      m_objective_value -= m_problem->profit( index );
      m_weight -= m_problem->weight( index );
    }

  m_variables[index] = var_free;

  assert( is_feasible() );
} // solution::free()

/*----------------------------------------------------------------------------*/
/**
 * \brief Force the value of the objective function.
 * \param v The new value.
 */
template<typename Problem>
void solution<Problem>::unsafe_set_objective_value( const profit_type& v )
{
  m_objective_value = v;
} // solution::unsafe_set_objective_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all variables from the solution.
 */
template<typename Problem>
void solution<Problem>::clear()
{
  m_objective_value *= 0;
  m_weight *=0;

  std::fill( m_variables.begin(), m_variables.end(), var_free );
} // solution::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a variable is set.
 * \param index Index of the variable to test.
 */
template<typename Problem>
bool solution<Problem>::is_set( unsigned int index ) const
{
  assert( index < m_problem->size() );
  return m_variables[index] == var_set;
} // solution::is_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a variable is unset.
 * \param index Index of the variable to test.
 */
template<typename Problem>
bool
solution<Problem>::is_unset( unsigned int index ) const
{
  assert( index < m_problem->size() );
  return m_variables[index] == var_unset;
} // solution::is_unset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a variable is .
 * \param index Index of the variable to test.
 */
template<typename Problem>
bool solution<Problem>::is_free( unsigned int index ) const
{
  assert( index < m_problem->size() );
  return m_variables[index] == var_free;
} // solution::is_free()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the solution (number of variables in the problem).
 */
template<typename Problem>
unsigned int solution<Problem>::size() const
{
  return m_problem->size();
} // solution::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the solution is feasible or not.
 *
 * \remark This method exists for debugging purposes only.
 */
template<typename Problem>
bool solution<Problem>::is_feasible() const
{
  profit_type real_objective_value;
  weight_type real_weight;

  real_objective_value *= 0;
  real_weight *= 0;

  for (unsigned int i=0; i!=m_problem->size(); ++i)
    if ( m_variables[i] == var_set )
      {
	real_weight += m_problem->weight(i);
	real_objective_value += m_problem->profit(i);
      }

  if ( real_weight != m_weight )
    std::cerr << "Weight is incorrect: " << m_weight << " != " << real_weight
	      << std::endl;

  if ( !profit_traits_type::equal(real_objective_value, m_objective_value) )
    std::cerr << "Objective value is incorrect: " << m_objective_value << " != "
	      << real_objective_value <<  std::endl;

  if ( m_weight > m_problem->capacity() )
    std::cerr << "Knapsack is over filled: " << m_weight << " > "
	      << m_problem->capacity() << std::endl;

  return ( real_weight == m_weight )
    && ( profit_traits_type::equal(real_objective_value, m_objective_value) )
    && ( m_weight <= m_problem->capacity() );
} // solution::is_feasible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all variables have been set.
 *
 * \remark This method exists for debugging purposes only.
 */
template<typename Problem>
bool solution<Problem>::is_full() const
{
  unsigned int n = 0;

  for (unsigned i=0; i!=size(); ++i)
    if ( is_free(i) )
      ++n;

#if 0
  if (n)
    std::cout << n << " free variables." << std::endl;
#endif

  return n==0;
} // solution::is_full()

