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
#ifndef __KP_DP_GRAPH_HPP__
#define __KP_DP_GRAPH_HPP__

#include <queue>
#include <vector>

#include <kp/bounded_region.hpp>
#include <kp/vertex.hpp>

#include <bikp/problem/mono_problem.hpp>

namespace kp
{
  /**
   * \brief A class that creates the dynamic programming table/graph of a
   *        knapsack problem.
   * \author Julien Jorge
   */
  template<class View>
  class dp_graph
  {
  public:
    /** \brief The type of the vue on the problem. */
    typedef View view_type;

    /** \brief The type of a layer in the graph. */
    typedef std::vector<vertex*> layer_type;

  private:
    /** \brief A table where we store the result of the relaxation for a given
        variable index. The key is the capacity. */
    typedef std::vector<moo::real_type> relax_cache_type;

  public:
    dp_graph( const view_type& p, const bounded_region& region );
    ~dp_graph();

    const layer_type& get_layer( unsigned int i ) const;

  private:
    void build_graph( const view_type& p, const bounded_region& region );

    void insert_vertex_skip
    ( vertex* skip, std::queue<vertex*>& pending, const cost_vector& dir );
    void insert_vertex_keep( vertex* keep, std::queue<vertex*>& pending );

    void push_in_layer( vertex* v, const cost_vector& dir );

    bool interesting
    ( relax_cache_type& relax_cache, const mono_problem& p, const vertex& v,
      const bounded_region& region ) const;
  
  private:
    /** \brief The layers in the graph. */
    std::vector<layer_type> m_layers;

  }; // class db_graph
} // namespac kp

#include "kp/impl/dp_graph.tpp"

#endif // __KP_DP_GRAPH_HPP__
