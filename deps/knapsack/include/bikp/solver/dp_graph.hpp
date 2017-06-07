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
 * \file dp_graph.hpp
 * \brief A class that creates the dynamic programming table/graph of a knapsack
 *        problem.
 * \author Julien Jorge
 */
#ifndef __BIKP_DP_GRAPH_HPP__
#define __BIKP_DP_GRAPH_HPP__

#include <queue>
#include <vector>
#include <map>

#include "bikp/tool/types.hpp"

/**
 * \brief A class that creates the dynamic programming table/graph of a knapsack
 *        problem.
 * \author Julien Jorge
 */
template<class Framework>
class dp_graph
{
public:
  /** \brief The type of the problem to solve. */
  typedef typename Framework::problem_type problem_type;

  /** \brief The type of the profit of the variables. */
  typedef typename problem_type::profit_type profit_type;

  /** \brief The type of the weight on the variables. */
  typedef typename problem_type::weight_type weight_type;

  /** \brief Basic operations on profits. */
  typedef typename problem_type::profit_traits_type profit_traits_type;

  /** \brief The type of the linear relaxation. */
  typedef typename Framework::relaxation_type upper_bound_type;

  /** \brief The type of the region in which the solutions must be. */
  typedef typename Framework::region_type region_type;

  /** \brief The lower bound value. */
  typedef profit_type bound_type;

  /** \brief The type of the labels on the vertices. */
  typedef typename Framework::vertex_type vertex_type;

  /** \brief The type of a layer in the graph. */
  typedef std::vector<vertex_type*> layer_type;

public:
  dp_graph( const problem_type& p, const region_type& region );
  ~dp_graph();

  const layer_type& get_layer( unsigned int i ) const;

  void output_dot( const problem_type& p, const std::string& filename ) const;

private:
  void output_dot( const problem_type& p, std::ofstream& f,
		   const vertex_type& v ) const;
  void output_dot_edge( const problem_type& p, std::ofstream& f,
			const vertex_type& s, const vertex_type& t ) const;

  void build_graph( const problem_type& p, const region_type& region );

  void insert_vertex_skip( vertex_type* skip,
			   std::queue<vertex_type*>& pending );
  void insert_vertex_keep( vertex_type* keep,
			   std::queue<vertex_type*>& pending );

  void push_in_layer( vertex_type* v );

  bool interesting( const upper_bound_type& ub, const problem_type& p,
		    const vertex_type& v, const region_type& region ) const;
  
  void output_stats( const problem_type& p,
		     const bound_type& lower_bound ) const;

  real_type paths_count() const;
  real_type paths_count
  ( const vertex_type& v,
    std::vector< std::map<weight_type, real_type> >& count_table ) const;

private:
  /** \brief The layers in the graph. */
  std::vector<layer_type> m_layers;

}; // class db_graph

#include "bikp/solver/impl/dp_graph.tpp"

#endif // __BIKP_DP_GRAPH_HPP__
