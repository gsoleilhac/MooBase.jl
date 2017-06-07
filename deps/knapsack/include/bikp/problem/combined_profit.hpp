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
 * \file combined_profit.hpp
 * \brief The type of the profits in the combined_problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_PROFIT_HPP__
#define __BIKP_COMBINED_PROFIT_HPP__

#include "bikp/problem/bi_profit.hpp"

/**
 * \brief For the profits values of the items in a combined mono problem, we
 *        store the combined value and the real values of the objectives but
 *        also the profit of the variables in the bi-objective problem.
 *
 * \remark This class should, most of the time, be seen as a mono objective
 *         problem (the profit is the value of the linear combination).
 * \author Julien Jorge
 */
class combined_profit : public bi_profit
{
public:
  /** \brief The type of the parent class. */
  typedef bi_profit super;

public:
  combined_profit();
  combined_profit
  ( real_type lambda_1, real_type lambda_2, const bi_profit& p );
  combined_profit
  ( real_type lambda_1, real_type lambda_2, real_type profit_1,
    real_type profit_2 );

  operator real_type() const;

  void inc();

  combined_profit ideal( const combined_profit& that ) const;
  combined_profit nadir( const combined_profit& that ) const;

  /**
   * \brief Add two profits.
   * \param that The other profit to add.
   */
  inline combined_profit operator+( const combined_profit& that ) const
  {
    combined_profit result;
    
    result.lambda = lambda + that.lambda;
    result.profit_1 = profit_1 + that.profit_1;
    result.profit_2 = profit_2 + that.profit_2;
    
    return result;
  } // operator+()

  combined_profit operator-( const combined_profit& that ) const;

  combined_profit& operator*=( real_type val );
  combined_profit& operator+=( const combined_profit& that );
  combined_profit& operator-=( const combined_profit& that );

public:
  /** \brief The combined value of the profits. */
  real_type lambda;
  
}; // combined_profit

/*----------------------------------------------------------------------------*/
combined_profit operator-( const combined_profit& that );
std::ostream& operator<<( std::ostream& os, const combined_profit& that );

#endif // __BIKP_COMBINED_PROFIT_HPP__
