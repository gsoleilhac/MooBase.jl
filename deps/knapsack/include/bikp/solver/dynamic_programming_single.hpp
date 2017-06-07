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
 * \file dynamic_programming_single.hpp
 * \brief Solve a knapsack problem using the dynamic programming approach.
 * \author Julien Jorge.
 */
#ifndef __BIKP_DYNAMIC_PROGRAMMING_SINGLE_HPP__
#define __BIKP_DYNAMIC_PROGRAMMING_SINGLE_HPP__

#include "bikp/solver/dp_graph.hpp"
#include "bikp/solver/vertex.hpp"
#include "bikp/region/half_line.hpp"
#include "bikp/relaxation/mono_relaxation.hpp"

/**
 * \brief Solve a knapsack problem using the dynamic programming approach.
 *
 * \b Template \b arguments :
 *  - \a Problem The type of the problem that we are solving.
 *
 * \author Julien Jorge.
 */
template<typename Problem>
class dynamic_programming_single
{
public:
  /** \brief The type of the problem to solve. */
  typedef Problem problem_type;

  /** \brief The type of the profits in the problem. */
  typedef typename problem_type::profit_type profit_type;

  /** \brief The type of the solutions for out problem. */
  typedef typename problem_type::solution_type solution_type;

  /** \brief The type of the region to explore. */
  typedef half_line<typename problem_type::profit_traits_type> region_type;

  struct framework
  {
    typedef Problem problem_type;
    typedef half_line<typename problem_type::profit_traits_type> region_type;
    typedef mono_relaxation<problem_type> relaxation_type;
    typedef vertex<problem_type> vertex_type;
  }; // struct framework

protected:
  /** \brief The type of the graph representation. */
  typedef dp_graph<framework> graph_type;

  /** \brief The type of the vertices in the graph. */
  typedef typename graph_type::vertex_type vertex_type;

public:
  dynamic_programming_single
  ( const problem_type& p, const region_type& region );

  void run( solution_type& sol ) const;

private:
  void build_one_solution( solution_type& sol, unsigned int index ) const;

protected:
  /** \brief The initial problem. */
  const problem_type& m_problem;

  /** \brief The region that we explore. */
  const region_type m_region;

  /** \brief The graph in which we want to find the longest paths */
  const graph_type m_graph;

}; // class dynamic_programming_single

#include "bikp/solver/impl/dynamic_programming_single.tpp"

#endif // __BIKP_DYNAMIC_PROGRAMMING_SINGLE_HPP__
