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
 * \file combined_profit.cpp
 * \brief Implementation of the combined_profit class.
 * \author Julien Jorge
 */
#include "bikp/problem/combined_profit.hpp"
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
combined_profit::combined_profit()
  : lambda(0)
{

} // combined_profit::combined_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lambda_1 Coefficient for the first objective.
 * \param lambda_2 Coefficient for the second objective.
 * \param p Value in the bi objective case.
 */
combined_profit::combined_profit
( real_type lambda_1, real_type lambda_2, const bi_profit& p )
  : super(p), lambda( lambda_1 * p.profit_1 + lambda_2 * p.profit_2 )
{

} // combined_profit::combined_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lambda_1 Coefficient for the first objective.
 * \param lambda_2 Coefficient for the second objective.
 * \param profit_1 Value on the first objective.
 * \param profit_2 Value on the second objective.
 */
combined_profit::combined_profit
( real_type lambda_1, real_type lambda_2, real_type profit_1,
  real_type profit_2 )
  : super(profit_1, profit_2),
    lambda( lambda_1 * profit_1 + lambda_2 * profit_2 )
{

} // combined_profit::combined_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the profit to unsigned integer.
 * \return The value of lambda.
 */
combined_profit::operator real_type() const
{
  return lambda;
} // combined_profit::operator real_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Increment the profit on the lambda objective.
 * \post (lambda = old(lambda) + 1) && (profit_1 == profit_2 == 0)
 */
void combined_profit::inc()
{
  // MODIF
  lambda += 1;
  profit_1 = profit_2 = 0;
} // combined_profit::inc()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ideal point, according to two profit values.
 * \param that The other profit.
 */
combined_profit combined_profit::ideal( const combined_profit& that ) const
{
  combined_profit result(*this);

  if (that.lambda > result.lambda)
    result.lambda = that.lambda;

  if (that.profit_1 > result.profit_1)
    result.profit_1 = that.profit_1;

  if (that.profit_2 > result.profit_2)
    result.profit_2 = that.profit_2;

  return result;
} // combined_profit::ideal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the nadir point, according to two profit values.
 * \param that The other profit.
 */
combined_profit combined_profit::nadir( const combined_profit& that ) const
{
  combined_profit result(*this);

  if (that.lambda < result.lambda)
    result.lambda = that.lambda;

  if (that.profit_1 < result.profit_1)
    result.profit_1 = that.profit_1;

  if (that.profit_2 < result.profit_2)
    result.profit_2 = that.profit_2;

  return result;
} // combined_profit::nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add two profits.
 * \param that The other profit to add.
 */
combined_profit combined_profit::operator-( const combined_profit& that ) const
{
  assert( lambda >= that.lambda );
  assert( profit_1 >= that.profit_1 );
  assert( profit_2 >= that.profit_2 );

  combined_profit result;

  result.lambda = lambda - that.lambda;
  result.profit_1 = profit_1 - that.profit_1;
  result.profit_2 = profit_2 - that.profit_2;

  return result;
} // combined_profit::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply the profit to unsigned integer.
 * \param val The value to multiply with.
 * \pre val == 0
 */
combined_profit& combined_profit::operator*=( real_type val )
{
  assert( val == 0 );
  
  profit_1 = profit_2 = lambda = 0;

  return *this;
} // combined_profit::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a profit.
 * \param that The profit to add.
 */
combined_profit& combined_profit::operator+=( const combined_profit& that )
{
  profit_1 += that.profit_1;
  profit_2 += that.profit_2;
  lambda   += that.lambda;

  return *this;
} // combined_profit::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract a profit.
 * \param that The profit to subtract.
 */
combined_profit& combined_profit::operator-=( const combined_profit& that )
{
  profit_1 -= that.profit_1;
  profit_2 -= that.profit_2;
  lambda   -= that.lambda;

  return *this;
} // combined_profit::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unary minus.
 * \param that The profit to change the sign to.
 */
combined_profit operator-( const combined_profit& that )
{
  combined_profit result;

  result.lambda = -that.lambda;
  result.profit_1 = -that.profit_1;
  result.profit_2 = -that.profit_2;

  return result;
} // operator-( combined_profit )

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the value of the profit in a standard output stream.
 * \param os The stream in which we write.
 * \param that The profit to write.
 */
std::ostream& operator<<( std::ostream& os, const combined_profit& that )
{
  os << that.profit_1 << " " << that.profit_2 << " " << that.lambda
     << " [comb_profit]";
  return os;
} // operator<<( std::ostream, combined_profit )
