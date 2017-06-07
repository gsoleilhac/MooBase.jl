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
 * \file solution_traits.tpp
 * \brief Implementation of the solution_traits class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two solution in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if and only if \a a is lesser than \a b.
 */
template<class Solution>
bool solution_traits<Solution>::less::operator()( const solution_type& a,
						  const solution_type& b ) const
{
  typename profit_traits_type::less comp;

  return comp(a.objective_value(), b.objective_value());
} // solution_traits::less::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution dominates an other.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if \a a dominates \a b.
 */
template<class Solution>
bool solution_traits<Solution>::better( const solution_type& a,
					const solution_type& b )
{
  return profit_traits_type::better( a.objective_value(), b.objective_value() );
} // solution_traits::better_than()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two solutions are of same quality.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Solution>
bool solution_traits<Solution>::equal( const solution_type& a,
				       const solution_type& b )
{
  return profit_traits_type::equal( a.objective_value(), b.objective_value() );
} // solution_traits::equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution dominates an other or if the two solutions are of
 *        same quality.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Solution>
bool solution_traits<Solution>::better_equal( const solution_type& a,
					      const solution_type& b )
{
  return better(a, b) || equal(a, b);
} // solution_traits::equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two solutions are exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Solution>
bool solution_traits<Solution>::same( const solution_type& a,
				      const solution_type& b )
{
  return a == b;
} // solution_traits::same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution dominates an other or if the two solutions are
 *        exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Solution>
bool solution_traits<Solution>::better_same( const solution_type& a,
					     const solution_type& b )
{
  return better(a, b) || same(a, b);
} // solution_traits::same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two solutions in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return less(a, b).
 */
template<class Solution>
bool solution_traits<Solution>::lex_less( const solution_type& a,
					  const solution_type& b )
{
  less comp;

  return comp(a, b);
} // solution_traits::lex_less()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the objective value of a solution.
 * \param a The solution from which we want the value.
 */
template<class Solution>
typename solution_traits<Solution>::profit_type
solution_traits<Solution>::to_profit( const solution_type& a )
{
  return a.objective_value();
} // solution_traits::to_profit()

