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
 * \file bi_problem.hpp
 * \brief Bi-objective problem.
 * \author Julien Jorge.
 */
#ifndef __BIKP_BI_PROBLEM_HPP__
#define __BIKP_BI_PROBLEM_HPP__

#include <istream>
#include <vector>

#include "bikp/problem/solution.hpp"
#include "bikp/problem/bi_profit.hpp"
#include "bikp/problem/mono_problem.hpp"
#include "bikp/concept/profit_traits.hpp"


/*----------------------------------------------------------------------------*/
/**
 * \brief Bi-objective problem.
 * \author Julien Jorge.
 */
class bi_problem
{
public:
  /** \brief The type of the profits. */
  typedef bi_profit profit_type;

  /** \brief The type of the weights. */
  typedef real_type weight_type;

  /** \brief The type of the current class. */
  typedef bi_problem self_type;

  /** \brief The type of the solutions for this kind of problem. */
  typedef solution<self_type> solution_type;

  /** \brief Basic operations on profits. */
  typedef profit_traits<profit_type> profit_traits_type;

public:
  bi_problem( std::istream& f );
  bi_problem( int nb_obj, int sz, int *obj1, int *obj2, int *weigths ,int capacity );
  bi_problem
    ( const std::vector<real_type>& c1, const std::vector<real_type>& c2,
      const std::vector<real_type>& w, real_type cap );
  bi_problem( unsigned int n, weight_type cap );

  void save( std::ostream& f ) const;

  void set_variable( unsigned int i, const profit_type& p, weight_type w );

  mono_problem* convert_to_mono( unsigned int obj ) const;

  /**
   * \brief Get the profit associated with a variable.
   * \param index Index of the variable.
   */
  inline profit_type profit( unsigned int index ) const
  {
    assert( index < m_profit.size() );
    return m_profit[index];
  } // profit()

  /**
   * \brief Get the weight associated with a variable.
   * \param index Index of the variable.
   */
  inline weight_type weight( unsigned int index ) const
  {
    assert( index < m_weight.size() );
    return m_weight[index];
  } // weight()

  unsigned int size() const;
  weight_type capacity() const;

  /**
   * \brief Get the efficiency of a variable.
   * \param obj The objective on which you want the efficiency.
   * \param index The index of the variable.
   * \pre (obj==1) || (obj==2)
   */
  inline real_type get_efficiency( unsigned int obj, unsigned int index ) const
  {
    assert( (obj==1) || (obj==2) );
    
    if (obj == 1)
      return m_efficiency_1[index];
    else
      return m_efficiency_2[index];
  }

private:
  void compute_efficiencies();

private:
  /** \brief Profits of the items. */
  std::vector<profit_type> m_profit;

  /** \brief Weight of the items. */
  std::vector<weight_type> m_weight;

  /** \brief Maximum capacity. */
  weight_type m_capacity;

  /** \brief Efficiency of the variables on the first objective. */
  std::vector<real_type> m_efficiency_1;

  /** \brief Efficiency of the variables on the second objective. */
  std::vector<real_type> m_efficiency_2;

}; // class bi_problem

/** \brief The type of the solution for this problem. */
typedef solution<bi_problem> bi_solution;

/** \brief Basic operations on profits. */
typedef profit_traits<bi_profit> bi_profit_traits;

#endif // __BIKP_BI_PROBLEM_HPP__
