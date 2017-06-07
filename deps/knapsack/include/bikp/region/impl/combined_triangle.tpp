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
 * \file combined_triangle.tpp
 * \brief Implementation of the template methods of the combined_triangle class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the region according to a set of points.
 *
 * The minimum combined objective of the solutions in the region is set equal to
 * or greater than the minimum combined objective value of the potential
 * solutions in the triangle, according to the set.
 *
 * \param first Iterator on the first point in the set.
 * \param last Iterator past the last point in the set.
 */
template<class Iterator>
void combined_triangle::tighten( Iterator first, Iterator last )
{
  if ( first != last )
    {
      if (m_keep_equivalence)
	tighten_keep( first, last );
      else
	tighten_dont_keep( first, last );
    }
} // combined_triangle::tighten()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the region according to a set of points in the case where we
 *        keep equivalent solutions.
 *
 * The minimum combined objective of the solutions in the region is set equal to
 * or greater than the minimum combined objective value of the potential
 * solutions in the triangle, according to the set.
 *
 * \param first Iterator on the first point in the set.
 * \param last Iterator past the last point in the set.
 */
template<class Iterator>
void combined_triangle::tighten_keep( Iterator first, Iterator last )
{
  Iterator second(first);
  real_type m, val;
  
  ++second;
  
  // top left
  val = m_lambda_1 * m_top_left.profit_1 + m_lambda_2 * m_top_left.profit_2;

  // the nadir on his right + (1,1)
  m = m_lambda_1 * (m_top_left.profit_1 + 1)
    + m_lambda_2 * (((profit_type)*first).profit_2 + 1);

  if ( m < val )
    val = m;

  for ( ; second != last; ++first, ++second)
    {
      // the point
      m = m_lambda_1 * ((profit_type)*first).profit_1
	+ m_lambda_2 * ((profit_type)*first).profit_2;
      
      if ( m < val )
	val = m;

      // the nadir on his right + (1,1)
      m = m_lambda_1 * (((profit_type)*first).profit_1 + 1)
	+ m_lambda_2 * (((profit_type)*second).profit_2 + 1);

      if ( m < val )
	val = m;
    }

  // the last point
  m = m_lambda_1 * ((profit_type)*first).profit_1
    + m_lambda_2 * ((profit_type)*first).profit_2;
      
  if ( m < val )
    val = m;

  // the nadir on the right of the last point + (1,1)
  m = m_lambda_1 * (((profit_type)*first).profit_1 + 1)
    + m_lambda_2 * (m_bottom_right.profit_2 + 1);

  if ( m < val )
    val = m;

  // bottom right point
  m = m_lambda_1 * m_bottom_right.profit_1
    + m_lambda_2 * m_bottom_right.profit_2;
  
  if ( m < val )
    val = m;
  
  if (val > m_combined_min)
    m_combined_min = val;
} // combined_triangle::tighten_keep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the region according to a set of points in the case where we
 *        do not keep equivalent solutions.
 *
 * The minimum combined objective of the solutions in the region is set equal to
 * or greater than the minimum combined objective value of the potential
 * solutions in the triangle, according to the set.
 *
 * \param first Iterator on the first point in the set.
 * \param last Iterator past the last point in the set.
 */
template<class Iterator>
void combined_triangle::tighten_dont_keep( Iterator first, Iterator last )
{
  Iterator second(first);
  real_type m, val;
  
  ++second;
  
  val = m_lambda_1 * (m_top_left.profit_1 + 1)
    + m_lambda_2 * ( ((profit_type)*first).profit_2 + 1);
  
  for ( ; second != last; ++first, ++second)
    {
      m = m_lambda_1 * ( ((profit_type)*first).profit_1 + 1) +
	m_lambda_2 * ( ((profit_type)*second).profit_2 + 1);
      
      if ( m < val )
	val = m;
    }

  m = m_lambda_1 * ( ((profit_type)*first).profit_1 + 1) +
    m_lambda_2 * (m_bottom_right.profit_2 + 1);
  
  if ( m < val )
    val = m;
  
  if (val > m_combined_min)
    m_combined_min = val;
} // combined_triangle::tighten_dont_keep()
