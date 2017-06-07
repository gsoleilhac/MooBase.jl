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
 * \file profit_traits.cpp
 * \brief Implementation of the profit_traits class specialized for real_type
 *        type.
 * \author Julien Jorge
 */
#include "bikp/concept/profit_traits.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two profit in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return \a a < \a b
 * \remark This is a specialization for real_type type.
 */
bool profit_traits<real_type>::less::operator()
  ( profit_type a, profit_type b ) const
{
  return a < b;
} // profit_traits::less::operator()() [real_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Increment a profit by one.
 * \param a The profit to increment.
 */
/*void profit_traits<real_type>::inc( profit_type& a )
{
  ++a;
}*/ // profit_traits::inc() [real_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a solution dominates an other.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if \a a dominates \a b.
 * \remark This is a specialization for real_type type.
 */
bool profit_traits<real_type>::better( profit_type a, profit_type b )
{
  return a > b;
} // profit_traits::better() [real_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two profits are of same quality.
 * \param a Left operand.
 * \param b Right operand.
 * \remark This is a specialization for real_type type.
 */
bool profit_traits<real_type>::equal( profit_type a, profit_type b )
{
  return a == b;
} // profit_traits::equal() [real_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a profit dominates an other or if the two profits are of same
 *        quality.
 * \param a Left operand.
 * \param b Right operand.
 * \remark This is a specialization for real_type type.
 */
bool profit_traits<real_type>::better_equal( profit_type a, profit_type b )
{
  return a >= b;
} // profit_traits::better_equal() [real_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two profits are exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
bool profit_traits<real_type>::same( profit_type a, profit_type b )
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
bool profit_traits<real_type>::better_same( profit_type a, profit_type b )
{
  return a >= b;
} // profit_traits::better_same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point of a profit.
 * \param a The profit that we want.
 */
profit_traits<real_type>::profit_type
profit_traits<real_type>::to_profit( profit_type a )
{
  return a;
} // profit_traits::to_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ideal point of two profits.
 * \param a Left operand.
 * \param b Right operand.
 */
profit_traits<real_type>::profit_type
profit_traits<real_type>::ideal( profit_type a, profit_type b )
{
  if (a >= b)
    return a;
  else
    return b;
} // profit_traits::ideal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the nadir point of two profits.
 * \param a Left operand.
 * \param b Right operand.
 */
profit_traits<real_type>::profit_type
profit_traits<real_type>::nadir( profit_type a, profit_type b )
{
  if (a <= b)
    return a;
  else
    return b;
} // profit_traits::nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two profits in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return \a a < \a b
 * \remark This is a specialization for real_type type.
 */
bool profit_traits<real_type>::lex_less( profit_type a, profit_type b )
{
  return a < b;
} // profit_traits::lex_less() [real_type]
