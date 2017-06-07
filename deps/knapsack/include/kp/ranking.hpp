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
 * \file ranking.hpp
 * \brief An algorithm that builds the k best-solutions of a knapsack problem.
 * \author Julien Jorge.
 */
#ifndef __KP_RANKING_HPP__
#define __KP_RANKING_HPP__

#include <list>
#include <map>

namespace kp
{
  /**
   * \brief An algorithm that builds the k best-solutions of a knapsack problem.
   * \author Julien Jorge.
   */
  template<typename View>
  class ranking
  {
  public:
    /** \brief The type of the vue on the problem. */
    typedef View view_type;

  private:
    /** \brief The type of the graph representation. */
    typedef dp_graph<view_type> graph_type;

    typedef const vertex* const_vertex_ptr;

    /**
     * \brief A set of vertices with shared common subsets.
     */
    class vertex_set
    {
    public:
      typedef std::list<const_vertex_ptr> set_type;

    public:
      vertex_set( const vertex_set* that, const vertex& v );

      void get_vertices( set_type& vertices ) const;
      const vertex& last_vertex() const;

      void release();

    private:
      void add_link();
      void remove_link();

      ~vertex_set();

    private:
      /** \brief The number of link to this set (ie. the number of sets
          containing this set as a subset. */
      unsigned int m_links;

      /** \brief The edges in this set. This value is shared between the
          instances. */
      const vertex_set* m_parent;

      /** \brief The edge that make this set different of its parent. */
      const vertex& m_turning;

    }; // class vertex_set

    /**
     * \brief This class represent a path in the graph. It contains turning
     *        edges compared to the optimal path.
     *
     * Edges are sorted in decreasing order of their target vertex.
     */
    class path
    {
    public:
      path( const vertex& r );
      path( const path& that, const vertex& t, const cost_vector& p );
      ~path();

      void get_vertices( std::list<const_vertex_ptr>& vertices ) const;

      const cost_vector& profit() const;
      const vertex& root() const;
      const vertex& last_vertex() const;

      static unsigned int inst_count;
      static unsigned int inst_count_max;
      static unsigned int current;

    private:
      /** \brief The out-vertices in the path (vertices that are not on the
          optimal	path). */
      vertex_set* m_vertices;

      /** \brief The vertex from which the path starts. */
      const vertex& m_root;

      /** \brief The profit-value of this path. */
      const cost_vector m_profit;

    }; // class path

    /** \brief The container use for storing paths. */
    typedef std::list<path*> quality_table_value_type;

    /** \brief The type of the table containing paths for each solution
        quality. */
    typedef
    std::map<moo::real_type, quality_table_value_type> quality_table_type;

  public:
    ranking
    ( const view_type& p, bounded_region& search_region,
      std::list<knapsack_solution>& sols, pareto_set& out_profits );
    ~ranking();

  private:
    moo::real_type compute_lambda_min() const;
    moo::real_type compute_lambda_max() const;

    void build_graph();

    void build_solutions();
    void build_solution( const path& p, knapsack_solution& sol );
    void build_optimal_solution
    ( const path& p, const_vertex_ptr &v, knapsack_solution& sol );

    std::pair<bool, const_vertex_ptr>
    build_optimal_solution_step
    ( const path& p, const_vertex_ptr &v, knapsack_solution& sol );

    void build_optimal_solution_step_add
    ( const path& p, const_vertex_ptr &v, knapsack_solution& sol );

    void build_next_path( const path& p );

    void add_path( const path& p, const_vertex_ptr s, const_vertex_ptr t );

    void clear_table();

    void set_solution_variable
    ( unsigned int i, knapsack_solution& sol, bool in ) const;

    void add_solution( const knapsack_solution& sol );

    bool dominated( const cost_vector& p ) const;

    void update_lower_bound();
    void update_paths_region();

  private:
    /** \brief The initial problem. */
    const view_type& m_problem;

    /** \brief The solutions found by the algorithm, in the region. */
    std::list<knapsack_solution>& m_solutions;

    /** \brief The graph in which we want to find the longest paths */
    graph_type* m_graph;

    /** \brief Table containing, for each profit sum, the node from which we
        must start to build the solution, and the turning edges to take. */
    quality_table_type m_quality_table;

    /** \brief The search region in the objective space, limits the search of
        new paths. */
    bounded_region m_paths_region;

    /** \brief The profits found out of the search area. */
    pareto_set& m_out_profits;

  }; // class ranking
} // namespace kp

#include <kp/impl/ranking.tpp>

#endif // __KP_RANKING_HPP__
