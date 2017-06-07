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
 * \file k_longest_paths.hpp
 * \brief An algorithm that builds the k best-solutions of a knapsack problem.
 * \author Julien Jorge.
 */
#ifndef __BIKP_K_LONGEST_PATHS_HPP__
#define __BIKP_K_LONGEST_PATHS_HPP__

#include <list>
#include <map>
#include "bikp/concept/minimum_complete_set.hpp"
#include "bikp/concept/pareto_set.hpp"
#include "bikp/problem/combined_problem.hpp"
#include "bikp/region/combined_triangle_front.hpp"
#include "bikp/relaxation/mono_relaxation.hpp"
#include "bikp/solver/dp_graph.hpp"
#include "bikp/solver/vertex.hpp"
#include "bikp/tool/types.hpp"

/**
 * \brief An algorithm that builds the k best-solutions of a knapsack problem.
 * \author Julien Jorge.
 */
class k_longest_paths
{
public:
  /** \brief The type of the problem to solve. */
  typedef combined_problem problem_type;

  typedef combined_triangle_front region_type;

  /** \brief A set of not dominated points. */
  typedef pareto_set<combined_profit> not_dominated_set;

private:
  struct framework
  {
    typedef combined_problem problem_type;
    typedef combined_triangle_front region_type;
    typedef mono_relaxation<problem_type> relaxation_type;
    typedef vertex<problem_type> vertex_type;
  }; // struct framework

  /** \brief The type of the graph representation. */
  typedef dp_graph<framework> graph_type;

  /** \brief The type of the vertices in the graph. */
  typedef graph_type::vertex_type vertex_type;

  typedef const vertex_type* const_vertex_ptr;

  /**
   * \brief A set of vertices with shared common subsets.
   */
  class vertex_set
  {
  public:
    typedef std::list<const_vertex_ptr> set_type;

  public:
    vertex_set( const vertex_set* that, const vertex_type& v );

    void get_vertices( set_type& vertices ) const;
    const vertex_type& last_vertex() const;

    void release();

  private:
    void add_link();
    void remove_link();

    ~vertex_set();

  private:
    /** \brief The number of link to this set (ie. the number of sets containing
	this set as a subset. */
    unsigned int m_links;

    /** \brief The edges in this set. This value is shared between the
	instances. */
    const vertex_set* m_parent;

    /** \brief The edge that make this set different of its parent. */
    const vertex_type& m_turning;

  }; // class vertex_set

  /**
   * \brief This class represent a path in the graph. It contains turning edges
   *        compared to the optimal path.
   *
   * Edges are sorted in decreasing order of their target vertex.
   */
  class path
  {
  public:
    path( const vertex_type& r );
    path( const path& that, const vertex_type& t, const combined_profit& p );
    ~path();

    void get_vertices( std::list<const_vertex_ptr>& vertices ) const;

    const combined_profit& profit() const;
    const vertex_type& root() const;
    const vertex_type& last_vertex() const;

    static unsigned int inst_count;
    static unsigned int inst_count_max;
    static unsigned int current;

  private:
    /** \brief The out-vertices in the path (vertices that are not on the
	optimal	path). */
    vertex_set* m_vertices;

    /** \brief The vertex from which the path starts. */
    const vertex_type& m_root;

    /** \brief The profit-value of this path. */
    const combined_profit m_profit;

  }; // class path

  /** \brief The container use for storing paths. */
  typedef std::list<path*> quality_table_value_type;

  /** \brief The type of the table containing paths for each solution
      quality. */
  typedef std::map<real_type, quality_table_value_type> quality_table_type;

public:
  k_longest_paths
  ( const combined_problem& p, combined_triangle_front& search_region,
    std::list<combined_solution>& sols, std::list<combined_profit>& out_sols );
  ~k_longest_paths();

private:
  real_type compute_lambda_min() const;
  real_type compute_lambda_max() const;

  void build_graph();

  void build_solutions();
  void build_solution( const path& p, combined_solution& sol );
  void build_optimal_solution
  ( const path& p, const_vertex_ptr &v, combined_solution& sol );

  std::pair<bool, const_vertex_ptr>
  build_optimal_solution_step
  ( const path& p, const_vertex_ptr &v, combined_solution& sol );

  void build_optimal_solution_step_add
  ( const path& p, const_vertex_ptr &v, combined_solution& sol );

  void build_next_path( const path& p );

  void add_path( const path& p, const_vertex_ptr s, const_vertex_ptr t );
  void clear_table();

  void set_solution_variable
  ( unsigned int i, combined_solution& sol, bool in ) const;

  void add_solution( const combined_solution& sol );

  bool dominated( const combined_profit& p ) const;

  void update_lower_bound();
  void update_paths_region();

private:
  /** \brief The initial problem. */
  const combined_problem& m_problem;

  /** \brief The solutions found by the algorithm, in the region. */
  std::list<combined_solution>& m_solutions;

  /** \brief Set of points corresponding to solutions found out of the search
      region. */
  not_dominated_set m_out_profits;

  /** \brief The graph in which we want to find the longest paths */
  graph_type* m_graph;

  /** \brief Table containing, for each profit sum, the node from which we must
      start to build the solution, and the turning edges to take. */
  quality_table_type m_quality_table;

  /** \brief The search region in the objective space, limits the search of new
      paths. */
  combined_triangle_front m_paths_region;

}; // class k_longest_paths

#endif // __BIKP_K_LONGEST_PATHS_HPP__
