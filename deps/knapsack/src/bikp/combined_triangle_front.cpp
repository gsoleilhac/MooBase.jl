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
 * \file combined_triangle_front.cpp
 * \brief Implementation of the combined_triangle_front class.
 * \author Julien Jorge
 */
#include "bikp/region/combined_triangle_front.hpp"

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
combined_triangle_front::combined_triangle_front
( bool keep_equivalence, const profit_type& top_left,
  const profit_type& bottom_right, real_type combined_min,
  real_type lambda_1, real_type lambda_2 )
  : super( keep_equivalence, top_left, bottom_right, combined_min, lambda_1,
	   lambda_2 )
{

} // combined_triangle_front::combined_triangle_front()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param keep_equivalence Tell if we have to keep equivalent solutions.
 * \param top_left The quality of the top-left solution.
 * \param bottom_right The quality of the bottom-right solution.
 * \param lambda_1 The coefficient on the first objective.
 * \param lambda_2 The coefficient on the second objective.
 */
combined_triangle_front::combined_triangle_front
( bool keep_equivalence, const profit_type& top_left,
  const profit_type& bottom_right, real_type lambda_1,
  real_type lambda_2 )
  : super( keep_equivalence, top_left, bottom_right, lambda_1, lambda_2 )
{

} // combined_triangle_front::combined_triangle_front()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region contains a quality of solution.
 * \param p The quality to check.
 */
bool combined_triangle_front::contains( const profit_type& p ) const
{
  bool result = false;

  if ( super::contains(p) )
    {
      result = !m_front.is_dominated(p);
      
      if (m_keep_equivalence && !result)
	result = m_front.contains(p);
    }

  return result;
} // combined_triangle_front::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a point and tighten in the region.
 * \param p The point to insert.
 */
void combined_triangle_front::insert( const profit_type& p )
{
  if ( m_front.insert(p) )
    super::tighten( m_front.begin(), m_front.end() );
} // combined_triangle_front::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a gnuplot description of the region.
 * \param os The stream in which we write.
 * \remark The output is a full gnuplot file.
 */
void combined_triangle_front::gnuplot( std::ostream& os ) const
{
  super::gnuplot( os );

  if ( !m_front.empty() )
    {
      std::string filename = data_file_prefix() + "_points.data";
      std::ofstream f( filename.c_str() );

      front_type::const_iterator it;
      
      for (it=m_front.begin(); it!=m_front.end(); ++it)
	f << it->profit_1 << " " << it->profit_2 << "\n";
      
      os << ", \\\n     \"" << filename << "\" title \"front\" with points 6";
    }
} // combined_triangle_front::gnuplot()
