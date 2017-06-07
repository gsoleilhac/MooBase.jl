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
 * \file combined_triangle_front.tpp
 * \brief Implementation of the template methods of the combined_triangle_front
 *        class.
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
 * All points are added to the front.
 *
 * \param first Iterator on the first point in the set.
 * \param last Iterator past the last point in the set.
 */
template<class Iterator>
void combined_triangle_front::tighten( Iterator first, Iterator last )
{
  m_front.insert(first, last);
  super::tighten( m_front.begin(), m_front.end() );
} // combined_triangle_front::tighten()
