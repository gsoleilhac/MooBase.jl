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
 * \file sortable_mono_problem.cpp
 * \brief Implementation of the sortable_mono_problem class.
 * \author Julien Jorge
 */
#include "bikp/problem/sortable_mono_problem.hpp"
#include "bikp/sort/efficiency_comparison.hpp"
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size The number of variables in the problem.
 */
sortable_mono_problem::sortable_mono_problem( const unsigned int size )
  : m_correspondence(size), m_efficiency(size)
{
  for (unsigned int i=0; i!=m_correspondence.size(); ++i)
    m_correspondence[i] = i;
} // sortable_mono_problem::sortable_mono_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the real index of a variable.
 * \param index The index of the variable in this problem.
 */
unsigned int sortable_mono_problem::real_index( unsigned int index ) const
{
  return m_correspondence[index];
} // sortable_mono_problem::real_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by decreasing efficiency.
 */
void sortable_mono_problem::sort_by_decreasing_efficiency()
{
  for (unsigned int i=0; i!=m_correspondence.size(); ++i)
    m_correspondence[i] = i;

  efficiency_comparison comp( m_efficiency );

  std::sort(m_correspondence.begin(), m_correspondence.end(), comp);
} // sorting_tools::sort_by_decreasing_efficiency()
