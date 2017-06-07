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
 * \file dynamic_programming_multiple.hpp
 * \brief Solve a knapsack problem using the dynamic programming approach.
 * \author Julien Jorge.
 */
#ifndef __KP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__
#define __KP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__

#include <list>

#include "kp/bounded_region.hpp"
#include <kp/dp_graph.hpp>

namespace kp
{
  /**
   * \brief Solve a knapsack problem using the dynamic programming approach.
   * \author Julien Jorge.
   */
  template<typename View>
  class dynamic_programming_multiple
  {
  public:
    /** \brief The type of the vue on the problem. */
    typedef View view_type;

  private:
    /** \brief The type of the graph representation. */
    typedef dp_graph<view_type> graph_type;

  public:
    dynamic_programming_multiple
    ( const view_type& p, const bounded_region& region );

    void run( std::list<knapsack_solution>& sols ) const;

  private:
    void build_all_solutions
    ( std::list<knapsack_solution>& sols, unsigned int index ) const;

    void build_all_solutions_from
    ( std::list<knapsack_solution>& sols, knapsack_solution& sol,
      const vertex& v ) const;

  private:
    /** \brief The initial problem. */
    const view_type& m_problem;

    /** \brief The region that we explore. */
    const bounded_region& m_region;

    /** \brief The graph in which we want to find the longest paths */
    const graph_type m_graph;

  }; // class dynamic_programming_multiple
} // namespace kp

#include <kp/impl/dynamic_programming_multiple.tpp>

#endif // __KP_DYNAMIC_PROGRAMMING_MULTIPLE_HPP__
