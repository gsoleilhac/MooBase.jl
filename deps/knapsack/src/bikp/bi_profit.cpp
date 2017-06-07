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
 * \file bi_profit.cpp
 * \brief Implementation of the bi_profit class.
 * \author Julien Jorge
 */
#include "bikp/problem/bi_profit.hpp"
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bi_profit::bi_profit()
  : profit_1(0), profit_2(0)
{

} // bi_profit::bi_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p1 Profit on the first objective.
 * \param p2 Profit on the second objective.
 */
bi_profit::bi_profit(real_type p1, real_type p2)
  : profit_1(p1), profit_2(p2)
{

} // bi_profit::bi_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current profit dominates (even weakly) an other one.
 * \param that The other profit.
 */
bool bi_profit::better_than( const bi_profit& that ) const
{
  return ((profit_1 > that.profit_1) && (profit_2 >= that.profit_2))
    || ((profit_1 >= that.profit_1) && (profit_2 > that.profit_2));
} // bi_profit::better_than()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ideal point, according to two profit values.
 * \param that The other profit.
 */
bi_profit bi_profit::ideal( const bi_profit& that ) const
{
  bi_profit result(*this);

  if (that.profit_1 > result.profit_1)
    result.profit_1 = that.profit_1;

  if (that.profit_2 > result.profit_2)
    result.profit_2 = that.profit_2;

  return result;
} // bi_profit::ideal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the nadir point, according to two profit values.
 * \param that The other profit.
 */
bi_profit bi_profit::nadir( const bi_profit& that ) const
{
  bi_profit result(*this);

  if (that.profit_1 < result.profit_1)
    result.profit_1 = that.profit_1;

  if (that.profit_2 < result.profit_2)
    result.profit_2 = that.profit_2;

  return result;
} // bi_profit::nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current profit is lesser than an other profit, in
 *        lexicographic order.
 * \param that The other profit.
 */
bool bi_profit::lex_less( const bi_profit& that ) const
{
  return (profit_1 < that.profit_1)
    || ( (profit_1 == that.profit_1) && (profit_2 < that.profit_2) );
} // bi_profit::lex_less()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality operator.
 * \param that The other profit to compare to.
 */
bool bi_profit::operator==( const bi_profit& that ) const
{
  return (profit_1 == that.profit_1) && (profit_2 == that.profit_2);
} // bi_profit::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disquality operator.
 * \param that The other profit to compare to.
 */
bool bi_profit::operator!=( const bi_profit& that ) const
{
  return !( *this == that );
} // bi_profit::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator '+' overload.
 * \param that The other profit to add.
 */
bi_profit bi_profit::operator+( const bi_profit& that ) const
{
  return bi_profit( profit_1 + that.profit_1, profit_2 + that.profit_2 );
} // bi_profit::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator '-' overload.
 * \param that The other profit to subtract.
 */
bi_profit bi_profit::operator-( const bi_profit& that ) const
{
  assert( better_than(that) || equivalent_to(that) );

  return bi_profit( profit_1 - that.profit_1, profit_2 - that.profit_2 );
} // bi_profit::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply the profits by an integer.
 * \param val The value to multiply with.
 */
bi_profit bi_profit::operator*( real_type val ) const
{
  return bi_profit( profit_1 * val, profit_2 * val );
} // bi_profit::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide the profits by an integer.
 * \param val The value to divide with.
 */
bi_profit bi_profit::operator/( real_type val ) const
{
  return bi_profit( profit_1 / val, profit_2 / val );
} // bi_profit::operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a profit.
 * \param that The profit to add.
 */
bi_profit& bi_profit::operator+=( const bi_profit& that )
{
  profit_1 += that.profit_1;
  profit_2 += that.profit_2;

  return *this;
} // bi_profit::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract a profit.
 * \param that The profit to subtract.
 */
bi_profit& bi_profit::operator-=( const bi_profit& that )
{
  assert( that.profit_1 <= profit_1 );
  assert( that.profit_2 <= profit_2 );

  profit_1 -= that.profit_1;
  profit_2 -= that.profit_2;

  return *this;
} // bi_profit::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply (scale).
 * \param val The value to multiply with.
 */
bi_profit& bi_profit::operator*=( real_type val )
{
  profit_1 *= val;
  profit_2 *= val;

  return *this;
} // bi_profit::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply an integer by a profit.
 * \param val The value to multiply.
 * \param that The value to multiply with.
 */
bi_profit operator*( real_type val, const bi_profit& that )
{
  return that * val;
} // operator*( real_type, bi_profit )

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the value of a profit in a standard output stream.
 * \param os The stream to write in.
 * \param that The profit to write.
 */
std::ostream& operator<<( std::ostream& os, const bi_profit& that )
{
  os << that.profit_1 << " " << that.profit_2 << " [bi_profit]" ;
  return os;
} // operator<<( std::ostream, bi_profit )
