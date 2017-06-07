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
 * \file combined_triangle_front.hpp
 * \brief This class is a triangular region in the objective space, for which
 *        we keep the front of solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_TRIANGLE_FRONT_HPP__
#define __BIKP_COMBINED_TRIANGLE_FRONT_HPP__

#include "bikp/region/combined_triangle.hpp"
#include "bikp/concept/pareto_set.hpp"

/**
 * \brief This class is a triangular region in the objective space.
 *
 * The region is delimited by two consecutive supported solutions plus the
 * front of the solutions inside it.
 *
 * \author Julien Jorge
 */
class combined_triangle_front : public combined_triangle
{
public:
  typedef combined_triangle super;

  /** \brief Basic operations on profits. */
  typedef super::profit_traits_type profit_traits_type;

  /** \brief The type of the profits in our search region. */
  typedef super::profit_type profit_type;

  typedef pareto_set<profit_type, profit_traits_type> front_type;

public:
  combined_triangle_front( bool keep_equivalence, const profit_type& top_left,
			   const profit_type& bottom_right,
			   real_type combined_min,
			   real_type lambda_1, real_type lambda_2 );
  combined_triangle_front( bool keep_equivalence, const profit_type& top_left,
			   const profit_type& bottom_right,
			   real_type lambda_1, real_type lambda_2 );

  void insert( const profit_type& p );
  bool contains( const profit_type& p ) const;

  void gnuplot( std::ostream& os ) const;

  template<class Iterator>
  void tighten( Iterator first, Iterator last );

private:
  /** \brief The front of solutions. */
  front_type m_front;

}; // class combined_triangle_front

#include "bikp/region/impl/combined_triangle_front.tpp"

#endif // __BIKP_COMBINED_TRIANGLE_FRONT_HPP__
