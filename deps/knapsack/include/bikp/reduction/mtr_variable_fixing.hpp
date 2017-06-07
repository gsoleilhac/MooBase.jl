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
 * \file mtr_variable_fixing.hpp
 * \brief This class tries to reduce the size of a problem by using the
 *        reduction algorithm from Martello and Toth.
 * \author Julien Jorge.
 */
#ifndef __BIKP_MTR_VARIABLE_FIXING_HPP__
#define __BIKP_MTR_VARIABLE_FIXING_HPP__

#include "bikp/reduction/base_variable_fixing.hpp"
#include "bikp/relaxation/mt_linear_relaxation.hpp"
#include "bikp/concept/profit_traits.hpp"
#include "bikp/concept/optimal_set.hpp"
#include "bikp/problem/mono_problem.hpp"

/**
 * \brief This class tries to reduce the size of a problem by using the
 *        reduction algorithm from Martello and Toth.
 * \author Julien Jorge.
 */
class mtr_variable_fixing
  : public base_variable_fixing<mono_problem>
{
public:
  /** \brief The type of the parent class. */
  typedef base_variable_fixing<mono_problem> super;

  /** \brief A set of optimal solutions. */
  typedef optimal_set<mono_solution> optimal_solution_set;

  /** \brief Container for the indices of the fixed variables. */
  typedef super::fixed_set_type fixed_set_type; 

private:
  /** \brief The type of the traits on the profits. */
  typedef super::profit_traits_type profit_traits_type;

  /** \brief The type of the relaxation used for computing upper bounds. */
  typedef mt_linear_relaxation<mono_problem> relaxation_type;

  /**
   * \brief A class to store how to build the solution corresponding to the best
   *        lower bound.
   */
  class lower_bound_type
  {
  public:
    lower_bound_type( const lower_bound_type& that );
    explicit lower_bound_type( unsigned int problem_size );
    explicit lower_bound_type
    ( unsigned int problem_size, const mono_solution& sol );
    ~lower_bound_type();

    void set_add( real_type profit, unsigned int split_item,
		  unsigned int index );
    void set_remove( real_type profit, unsigned int split_item,
		     unsigned int index );
    void set_profit( real_type profit );
    
    void build( mono_solution& sol ) const;

    real_type to_profit() const;

  private:
    void build_insertion( mono_solution& sol ) const;
    void build_suppression( mono_solution& sol ) const;

  private:
    /** \brief The size of the problem for which we are a solution. */
    const unsigned int m_problem_size;

    /** \brief Pointer to the solution from which we were build. */
    const mono_solution* m_solution;

    /** \brief The value of the bound. */
    real_type m_profit;

    /** \brief The first item that doesn't fit in the solution. */
    unsigned int m_split_item;

    /** \brief The item to add in the solution. */
    unsigned int m_add_item;

    /** \brief The item to remove from the solution. */
    unsigned int m_remove_item;

  }; // class lower_bound_type

  /**
   * \brief Basic operations on lower bounds.
   * \author Julien Jorge
   */
  class lower_bound_traits
  {
  public:
    /** \brief Basic operations on the objective value. */
    typedef profit_traits<real_type> profit_traits_type;

    /**
     * \brief Function object that compares two lower bounds by the objective 
     *        values of their solution (in lexicographic order).
     * \author Julien Jorge.
     */
    class less
    {
    public:
      bool operator()( const lower_bound_type& a,
		       const lower_bound_type& b ) const;
    }; // class less

  public:
    static bool better( const lower_bound_type& a, const lower_bound_type& b );
    static bool equal( const lower_bound_type& a, const lower_bound_type& b );
    static bool better_equal( const lower_bound_type& a,
			      const lower_bound_type& b );
    static bool same( const lower_bound_type& a, const lower_bound_type& b );
    static bool better_same( const lower_bound_type& a,
			     const lower_bound_type& b );
    
    static bool lex_less( const lower_bound_type& a,
			  const lower_bound_type& b );
    
  }; // lower_bound_traits
  
  /** \brief A set of optimal profits, used for storing lower bounds. */
  typedef optimal_set< lower_bound_type, lower_bound_traits > lower_bound_set;

public:
  mtr_variable_fixing( const mono_problem& p, mono_solution& sol );
  mtr_variable_fixing( const mono_problem& p, optimal_solution_set& sols );

private:
  void run( optimal_solution_set& sols );

  void find_sets( const std::vector<real_type>& weight_sum,
		  const std::vector<real_type>& profit_sum,
		  unsigned int split_item, optimal_solution_set& sols );

  unsigned int
  find_index( real_type val, const std::vector<real_type>& table ) const;

  void calculate_sums( std::vector<real_type>& weight_sum,
		       std::vector<real_type>& profit_sum ) const;

  void greedy( const std::vector<real_type>& weight_sum,
	       const std::vector<real_type>& profit_sum,
	       optimal_solution_set& sol ) const;

  void calculate_upper_bound_zero( std::vector<real_type>& upper_bound,
				   const std::vector<real_type>& weight_sum,
				   const std::vector<real_type>& profit_sum,
				   lower_bound_set& lower_bound ) const;

  void calculate_upper_bound_one( std::vector<real_type>& upper_bound,
				  const std::vector<real_type>& weight_sum,
				  const std::vector<real_type>& profit_sum,
				  lower_bound_set& lower_bound,
				  unsigned int split_item ) const;

  real_type relax( unsigned int split_item, real_type capacity,
		      real_type current_profit ) const;

  void build_lower_bound( const optimal_solution_set& sols,
			  lower_bound_set& lower_bound ) const;
  void build_solutions( const lower_bound_set& lower_bound,
			optimal_solution_set& sols ) const;
  void fix_variables( const lower_bound_set& lower_bound,
		      const std::vector<real_type>& upper_bound_zero,
		      const std::vector<real_type>& upper_bound_one,
		      unsigned int split_item );

  void fix_more();

}; // class mtr_variable_fixing

#endif // __BIKP_MTR_VARIABLE_FIXING_HPP__
