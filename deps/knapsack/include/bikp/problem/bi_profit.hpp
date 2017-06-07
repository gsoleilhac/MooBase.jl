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
 * \file bi_profit.hpp
 * \brief Profit of the variables in a bi-objective problem.
 * \author Julien Jorge.
 */
#ifndef __BIKP_BI_PROFIT_HPP__
#define __BIKP_BI_PROFIT_HPP__

#include <ostream>
#include "bikp/tool/types.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief The type of the profit in a bi-objective problem.
 * \author Julien Jorge
 */
class bi_profit
{
public:
  bi_profit();
  bi_profit( real_type p1, real_type p2 );
  
  bool better_than( const bi_profit& that ) const;

  /**
   * \brief Tell if the current profit has the same quality than an other one.
   * \param that The other operand.
   */
  inline bool equivalent_to( const bi_profit& that ) const
  {
    return (profit_1 == that.profit_1) && (profit_2 == that.profit_2);
  } // equivalent_to()

  bi_profit ideal( const bi_profit& that ) const;
  bi_profit nadir( const bi_profit& that ) const;
  bool lex_less( const bi_profit& that ) const;

  bool operator==( const bi_profit& that ) const;
  bool operator!=( const bi_profit& that ) const;

  bi_profit operator+( const bi_profit& that ) const;
  bi_profit operator-( const bi_profit& that ) const;
  bi_profit operator*( real_type val ) const;
  bi_profit operator/( real_type val ) const;
  
  bi_profit& operator+=( const bi_profit& that );
  bi_profit& operator-=( const bi_profit& that );
  bi_profit& operator*=( real_type val );
  
public:
  /** \brief Profit on the first objective. */
  real_type profit_1;

  /** \brief Profit on the second objective. */
  real_type profit_2;

}; // class bi_profit

/*----------------------------------------------------------------------------*/
bi_profit operator*( real_type val, const bi_profit& that );
std::ostream& operator<<( std::ostream& os, const bi_profit& that );

#endif // __BIKP_BI_PROFIT_HPP__
