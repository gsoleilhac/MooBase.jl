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
 * \file base_second_phase.hpp
 * \brief Use a branch and bound algorithm to find all not supported solutions.
 * \author Julien Jorge
 */
#ifndef __BIKP_BASE_SECOND_PHASE_HPP__
#define __BIKP_BASE_SECOND_PHASE_HPP__

#include <iostream>
#include <map>

#include "bikp/problem/bi_problem.hpp"
#include "bikp/tool/logger.hpp"
#include "bikp/problem/combined_problem.hpp"
#include "bikp/reduction/base_variable_fixing.hpp"
#include "bikp/region/combined_triangle_front.hpp"
#include "bikp/concept/maximum_complete_set.hpp"

/**
 * \brief Base class for second phase algorithms.
 * \author Julien Jorge
 */
class base_second_phase
{
public:
  /** \brief The type of the set containing the calculated solutions. */
  //typedef maximum_complete_set<bi_solution> optimal_set_type;

  /** \brief The base type of reduction applyed to combined_problem. */
  typedef base_variable_fixing<combined_problem> combined_reduction_type;

  /** \brief The type of the set containing the calculated solutions for
      combined problems. */
  //typedef maximum_complete_set<combined_solution> combined_optimal_set_type;

  /**
   * \brief A triangle in the search space.
   */
  class triangle
  {
  public:
    triangle();
    triangle( const bi_profit& z_left, const bi_profit& z_right );

    const bi_profit& top_left() const;
    const bi_profit& bottom_right() const;

    real_type width() const;
    real_type height() const;

  private:
    /** \brief Point of the top-left corner. */
    const bi_profit* m_z_left;

    /** \brief Point of the bottom-right corner. */
    const bi_profit* m_z_right;

  }; // class triangle

  /**
   * \brief Function object that compares two triangles in increasing order of
   *        their area.
   */
  class area_lesser
  {
  public:
    bool operator()( const triangle& t1, const triangle& t2 ) const;

  }; // class area_lesser

  /**
   * \brief The set of the triangles in the search space, and the solutions
   *        found for each triangle.
   */
  class triangle_set
  {
  private:
    /**
     * \brief A triangle in the search space with the points of the efficient
     *        solutions located in it.
     */
    class local_point_set
    {
    public:
      local_point_set();
      local_point_set( const bi_profit& p1, const bi_profit& p2 );

      void compute_lower_bound();

    public:
      /** \brief Tell if the triangle is locked (can't insert more
	  solutions). */
      bool locked;

      /** \brief The top-left corner. */
      bi_profit z1;

      /** \brief The bottom-right corner. */
      bi_profit z2;

      /** \brief The solutions found for this space. */
      pareto_set<bi_profit> point_set;

      /** \brief The current lower bound on the solutions in this triangle. */
      real_type lower_bound;

      /** \brief The combined value of the best solutions. */
      real_type upper_bound;

    }; // class local_point_set

    /** \brief A container that stores triangles. The key is the value of the
	top-left corner on the first objective. */
    typedef std::map<real_type, local_point_set*> point_set_map;

  public:
    ~triangle_set();

    void add( const triangle& t );
    const pareto_set<bi_profit>& get_set( const triangle& t );
    void lock( const triangle& t );
    bool locked( const triangle& t ) const;

    void insert( const bi_profit& s );

    template<typename Iterator>
    void insert( Iterator first, Iterator last )
    {
      for ( ; first!=last; ++first )
        insert( *first );
    }

    void tighten();
    real_type strip_width( const triangle& t ) const;
    real_type get_lower_bound( const triangle& t ) const;

    void large_cut( std::list<bi_profit>& ub ) const;

  private:
    /** \brief The triangles in the search space. The key is the value of the
	top-left corner on the first objective. */
     point_set_map m_triangles;

  }; // class triangle_set

public:
  base_second_phase( const std::string& name );
  virtual ~base_second_phase() {}

  void run( const bi_problem& p, const std::list<bi_solution>& X_SE );
  const maximum_complete_set<bi_solution>& get_solutions() const;
  void save( std::ostream& os ) const;

protected:
  virtual void pre_sort( std::vector<triangle>& triangles ) const;

  virtual combined_reduction_type* reduce
  ( const combined_problem& p, const combined_profit& z_left,
    const combined_profit& z_right, real_type lb_lambda ) const = 0;

  virtual
  void solve( const combined_problem& p, combined_triangle_front& region,
	      std::list<combined_solution>& sols,
	      std::list<combined_profit>& out_sols ) const = 0;

private:
  void reduce_and_run
  ( const bi_problem& p, const std::list<bi_solution>& X_SE );
  void run_solver( const bi_problem& p, const std::list<bi_profit>& Z_SE );

  unsigned int
  next_triangle_index( const std::vector<triangle>& triangles ) const;

  void calculate_triangle( const bi_problem& p, const triangle& t );

  void solve( const combined_problem& mono_p, const triangle& t,
	      std::list<combined_solution>& new_solutions,
	      std::list<combined_profit>& out_solutions );

  void mono_to_sub_mono( const combined_reduction_type& reduction,
			 const std::list<combined_solution>& new_solutions,
			 std::list<combined_solution>& sols,
			 real_type min_lambda ) const;

  void scale_known_solutions
  ( pareto_set<combined_profit>& ub, const combined_profit& delta,
    real_type lambda_1, real_type lambda_2, const triangle& t ) const;

private:
  /** \brief A logger, to write informations about the process status. */
  tool::logger m_logger;

  /** \brief Not supported solutions. */
  triangle_set m_solution_sets;

  /** \brief Not supported solutions found. */
  maximum_complete_set<bi_solution> m_X_NSE;

  /** \brief The front of supported and not supported solutions found up to
      now. */
  pareto_set<bi_profit> m_pareto_front;

};  // class base_second_phase

#endif // __BIKP_BASE_SECOND_PHASE_HPP__
