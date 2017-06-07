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
 * \file combined_triangle.cpp
 * \brief Implementation of the combined_triangle class.
 * \author Julien Jorge
 */
#include "bikp/region/combined_triangle.hpp"

#include <fstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param keep_equivalence Tell if we have to keep equivalent solutions.
 * \param top_left The quality of the top-left solution.
 * \param bottom_right The quality of the bottom-right solution.
 * \param combined_min The minimum interesting value on the combined_objective.
 * \param lambda_1 The coefficient on the first objective.
 * \param lambda_2 The coefficient on the second objective.
 */
combined_triangle::combined_triangle
( bool keep_equivalence, const profit_type& top_left,
  const profit_type& bottom_right, real_type combined_min,
  real_type lambda_1, real_type lambda_2 )
  : m_keep_equivalence(keep_equivalence), m_top_left(top_left),
    m_bottom_right(bottom_right), m_combined_min(combined_min),
    m_lambda_1(lambda_1), m_lambda_2(lambda_2)
{
  assert( top_left.profit_1 < bottom_right.profit_1 );
  assert( top_left.profit_2 > bottom_right.profit_2 );

  real_type val;

  if (keep_equivalence)
    {
      val = m_lambda_1 * top_left.profit_1
	+ m_lambda_2 * (bottom_right.profit_2 + 1);

      real_type m = m_lambda_1 * (top_left.profit_1 + 1)
	+ m_lambda_2 * bottom_right.profit_2;

      if (m < val)
	val = m;
    }
  else
    val = m_lambda_1 * (top_left.profit_1 + 1)
      + m_lambda_2 * (bottom_right.profit_2 + 1);

  if (val > m_combined_min)
    m_combined_min = val;
} // combined_triangle::combined_triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param keep_equivalence Tell if we have to keep equivalent solutions.
 * \param top_left The quality of the top-left solution.
 * \param bottom_right The quality of the bottom-right solution.
 * \param lambda_1 The coefficient on the first objective.
 * \param lambda_2 The coefficient on the second objective.
 */
combined_triangle::combined_triangle
( bool keep_equivalence, const profit_type& top_left,
  const profit_type& bottom_right, real_type lambda_1,
  real_type lambda_2 )
  : m_keep_equivalence(keep_equivalence), m_top_left(top_left),
    m_bottom_right(bottom_right), m_lambda_1(lambda_1), m_lambda_2(lambda_2)
{
  assert( top_left.profit_1 < bottom_right.profit_1 );
  assert( top_left.profit_2 > bottom_right.profit_2 );

  if (keep_equivalence)
    {
      m_combined_min = m_lambda_1 * top_left.profit_1
	+ m_lambda_2 * (bottom_right.profit_2 + 1);

      real_type m = m_lambda_1 * (top_left.profit_1 + 1)
	+ m_lambda_2 * bottom_right.profit_2;
      
      if (m < m_combined_min)
	m_combined_min = m;
    }
  else
    m_combined_min = m_lambda_1 * (top_left.profit_1 + 1)
      + m_lambda_2 * (bottom_right.profit_2 + 1);
} // combined_triangle::combined_triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum interesting profit value on each objective.
 */
combined_triangle::profit_type combined_triangle::min_profit() const
{
  profit_type result;
  result.profit_1 = m_top_left.profit_1;
  result.profit_2 = m_bottom_right.profit_2;
  result.lambda = m_combined_min;

  if ( !m_keep_equivalence )
    {
      // MODIF
      result.profit_1 += 1;
      result.profit_2 += 1;
    }

  return result;
} // combined_triangle::min_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region contains a quality of solution.
 * \param p The quality to check.
 */
bool combined_triangle::contains( const profit_type& p ) const
{
  bool result = (p.lambda >= m_combined_min)
    && (p.profit_1 > m_top_left.profit_1)
    && (p.profit_2 > m_bottom_right.profit_2);

  if ( !result )
    if ( m_keep_equivalence )
      result = (p == m_top_left) || (p == m_bottom_right);

  return result;
} // combined_triangle::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we can hope to reach the region from a given quality of
 *        solution.
 * \param p The quality to check.
 */
bool combined_triangle::feasible( const profit_type& p ) const
{
  if ( m_keep_equivalence )
    return (p.profit_2 <= m_top_left.profit_2)
      && (p.profit_1 <= m_bottom_right.profit_1);
  else
    return (p.profit_2 < m_top_left.profit_2)
      && (p.profit_1 < m_bottom_right.profit_1);
} // combined_triangle::feasible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The item to copy from.
 */
combined_triangle& combined_triangle::operator=( const self_type& that )
{
  assert( m_top_left == that.m_top_left );
  assert( m_bottom_right == that.m_bottom_right );
  assert( m_lambda_1 == that.m_lambda_1 );
  assert( m_lambda_2 == that.m_lambda_2 );

  m_combined_min = that.m_combined_min;
  return *this;
} // combined_triangle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a gnuplot description of the region.
 * \param os The stream in which we write.
 * \remark The output is a full gnuplot file.
 */
void combined_triangle::gnuplot( std::ostream& os ) const
{
  std::string filename = data_file_prefix() + ".data";
  std::ofstream f( filename.c_str() );

  f << m_top_left.profit_1 << " " << m_top_left.profit_2 << std::endl;
  f << m_bottom_right.profit_1 << " " << m_bottom_right.profit_2 << std::endl;
  f << m_top_left.profit_1 << " " << m_bottom_right.profit_2 << std::endl;
  f << m_top_left.profit_1 << " " << m_top_left.profit_2 << std::endl;

  const real_type x_margin =
    (real_type)((real_type)(get_right() - get_left()) * 0.1);
  const real_type y_margin =
    (real_type)((real_type)
    (m_top_left.profit_2 - m_bottom_right.profit_2) * 0.1);

  os << "set xrange [ " << get_left() - x_margin << " : "
     << get_right() + x_margin << " ]\n";

  os << "set yrange [ "
     << m_bottom_right.profit_2 - y_margin << " : "
     << m_top_left.profit_2 + y_margin << " ]\n\n";

  os << "plot \"" << filename << "\" notitle with linespoints 1, \\\n"
     << "     (" << m_combined_min << " - " << m_lambda_1 << " * x) / "
     << m_lambda_2 << " title \"lower_bound\" with lines -1";
} // combined_triangle::gnuplot()
