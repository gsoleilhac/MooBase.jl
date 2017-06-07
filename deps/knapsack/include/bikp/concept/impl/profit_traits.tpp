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
 * \file profit_traits.tpp
 * \brief Implementation of the profit_traits class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two profit in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if and only if \a a is lesser than \a b.
 */
template<class Profit>
bool profit_traits<Profit>::less::operator()( const profit_type& a,
					      const profit_type& b ) const
{
  return a.lex_less(b);
} // profit_traits::less::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Increment a profit by one.
 * \param a The profit to increment.
 */
/*template<class Profit>
void profit_traits<Profit>::inc( profit_type& a )
{
  a.inc();
}*/ // profit_traits::inc()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a profit dominates an other.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if \a a dominates \a b.
 */
template<class Profit>
bool profit_traits<Profit>::better( const profit_type& a, const profit_type& b )
{
  return a.better_than(b);
} // profit_traits::better()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a profit dominates an other or if the two profits are of same
 *        quality.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Profit>
bool profit_traits<Profit>::better_equal( const profit_type& a,
					  const profit_type& b )
{
  return better(a, b) || equal(a, b);
} // profit_traits::equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two profits are exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Profit>
bool profit_traits<Profit>::same( const profit_type& a, const profit_type& b )
{
  return a == b;
} // profit_traits::same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a profit dominates an other or if the two profits are exactly
 *        the same.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Profit>
bool profit_traits<Profit>::better_same( const profit_type& a,
					 const profit_type& b )
{
  return better(a, b) || same(a, b);
} // profit_traits::better_same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the profit.
 * \param a The profit that we want.
 */
template<class Profit>
const typename profit_traits<Profit>::profit_type&
profit_traits<Profit>::to_profit( const profit_type& a )
{
  return a;
} // profit_traits::to_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ideal point of two profits.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Profit>
typename profit_traits<Profit>::profit_type
profit_traits<Profit>::ideal( const profit_type& a, const profit_type& b )
{
  return a.ideal(b);
} // profit_traits::ideal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the nadir point of two profits.
 * \param a Left operand.
 * \param b Right operand.
 */
template<class Profit>
typename profit_traits<Profit>::profit_type
profit_traits<Profit>::nadir( const profit_type& a, const profit_type& b )
{
  return a.nadir(b);
} // profit_traits::nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two profits in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return less(a, b).
 */
template<class Profit>
bool profit_traits<Profit>::lex_less( const profit_type& a,
				      const profit_type& b )
{
  less comp;

  return comp(a, b);
} // profit_traits::lex_less()
