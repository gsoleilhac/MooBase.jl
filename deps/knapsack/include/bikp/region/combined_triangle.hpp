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
 * \file combined_triangle.hpp
 * \brief This class is a triangular region in the objective space.
 * \author Julien Jorge
 */
#ifndef __BIKP_COMBINED_TRIANGLE_HPP__
#define __BIKP_COMBINED_TRIANGLE_HPP__

#include "bikp/problem/combined_problem.hpp"

#include <sstream>

/**
 * \brief This class is a triangular region in the objective space.
 *
 * The region is delimited by two consecutive supported solutions plus the
 * minimum value of the combined objective of the potentials non-supported
 * solutions in this triangle.
 *
 * \author Julien Jorge
 */
class combined_triangle
{
public:
  /** \brief Basic operations on profits. */
  typedef combined_problem::profit_traits_type profit_traits_type;

  /** \brief The type of the profits in our search region. */
  typedef profit_traits_type::profit_type profit_type;

  /** \brief The type of the current class. */
  typedef combined_triangle self_type;

public:
  combined_triangle( bool keep_equivalence, const profit_type& top_left,
		     const profit_type& bottom_right,
		     real_type combined_min,
		     real_type lambda_1, real_type lambda_2 );
  combined_triangle( bool keep_equivalence, const profit_type& top_left,
		     const profit_type& bottom_right,
		     real_type lambda_1, real_type lambda_2 );

  profit_type min_profit() const;
  bool contains( const profit_type& p ) const;
  bool feasible( const profit_type& p ) const;

  self_type& operator=( const self_type& that );

  void gnuplot( std::ostream& os ) const;

  std::string data_file_prefix() const
  {
    std::ostringstream oss;
    oss << "triangle_x" << get_left() << "_x" << get_right();
    return oss.str();
  }

  template<class Iterator>
  void tighten( Iterator first, Iterator last );

  profit_type get_top_left() const { return m_top_left; }
  profit_type get_bottom_right() const { return m_bottom_right; }
  real_type get_top() const { return m_top_left.profit_2; }
  real_type get_bottom() const { return m_bottom_right.profit_2; }
  real_type get_left() const { return m_top_left.profit_1; }
  real_type get_right() const { return m_bottom_right.profit_1; }
  real_type get_lambda_1() const { return m_lambda_1; }
  real_type get_lambda_2() const { return m_lambda_2; }

private:
  template<class Iterator>
  void tighten_keep( Iterator first, Iterator last );

  template<class Iterator>
  void tighten_dont_keep( Iterator first, Iterator last );

protected:
  /** \brief Tell if we have to keep the possibility to have equivalent
      solutions. It concerns only the bound on lambda. */
  const bool m_keep_equivalence;

private:
  /** \brief The objective value of the solution in the top-left corner. */
  const profit_type m_top_left;

  /** \brief The objective value of the solution in the bottom-right corner. */
  const profit_type m_bottom_right;

  /** \brief The minimum value of the combined objectives. */
  real_type m_combined_min;

  /** \brief Coefficient on the first objective. */
  const real_type m_lambda_1;

  /** \brief Coefficient on the second objective. */
  const real_type m_lambda_2;

}; // class combined_triangle

#include "bikp/region/impl/combined_triangle.tpp"

#endif // __BIKP_COMBINED_TRIANGLE_HPP__
