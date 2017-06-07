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
 * \file efficiency_comparison.hpp
 * \brief This class make easier the comparison of variables by their
 *        efficiency.
 * \author Julien Jorge
 */
#ifndef __BIKP_EFFICIENCY_COMPARISON_HPP__
#define __BIKP_EFFICIENCY_COMPARISON_HPP__

/**
 * \brief This class make easier the comparison of variables by their
 *        efficiency.
 * \author Julien Jorge
 */
class efficiency_comparison
{
public:
  efficiency_comparison( const std::vector<real_type>& eff )
    : m_efficiency(eff)
  { }

  /**
   * \brief Compare two items by their efficiency.
   * \param i Index of the first item.
   * \param j index of the second item.
   * \return true iff get_efficiency(i) > get_efficiency(j)
   */
  bool operator()(unsigned int i, unsigned int j)
  {
    return m_efficiency[i] - m_efficiency[j] > 0;
  }

private:
  /** \brief The efficiency of the variables that we are sorting. */
  const std::vector<real_type>& m_efficiency;

}; // class efficiency_comparison

#endif // __BIKP_EFFICIENCY_COMPARISON_HPP__
