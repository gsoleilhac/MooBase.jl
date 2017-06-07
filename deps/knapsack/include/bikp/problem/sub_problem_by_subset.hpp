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
 * \file sub_problem_by_subset.hpp
 * \brief A sub problem by subset is a problem reduced to a subset of its
 *        variables.
 * \author Julien Jorge
 */
#ifndef __BIKP_SUB_PROBLEM_BY_SUBSET_HPP__
#define __BIKP_SUB_PROBLEM_BY_SUBSET_HPP__

#include <set>
#include "bikp/problem/solution.hpp"

/**
 * \brief A sub problem by subset is a problem reduced to a subset of its
 *        variables.
 *
 * \b Template \b arguments :
 * - \a Problem : The type of the parent problem.
 *
 * \author Julien Jorge
 */
template<typename Problem>
class sub_problem_by_subset
{
public:
  /** \brief The type of the parent problem. */
  typedef Problem problem_type;

  /** \brief The type of the profits. */
  typedef typename problem_type::profit_type profit_type;

  /** \brief The type of the weights. */
  typedef typename problem_type::weight_type weight_type;

  /** \brief Basic operations on profits. */
  typedef typename problem_type::profit_traits_type profit_traits_type;

  /** \brief Container type for set of variables given to the constructor. */
  typedef std::set<unsigned int> set_of_variables_type;

  /** \brief The type of the current class. */
  typedef sub_problem_by_subset<problem_type> self_type;

  /** \brief The type of the solution for this problem. */
  typedef solution<self_type> solution_type;

public:
  sub_problem_by_subset
  ( const problem_type& p, const set_of_variables_type& vars, weight_type cap,
    bool keep );

  sub_problem_by_subset
  ( const problem_type& p, const set_of_variables_type& vars, bool keep );

  /**
   * \brief Get the profit of a variable.
   * \param index The index of the variable.
   */
  inline profit_type profit( unsigned int index ) const
  {
    assert( index < size() );
    return m_problem.profit( m_correspondence[index] );
  } // profit()

  /**
   * \brief Get the weight of a variable.
   * \param index The index of the variable.
   */
  inline weight_type weight( unsigned int index ) const
  {
    assert( index < size() );
    return m_problem.weight( m_correspondence[index] );
  } // weight()

  unsigned int size() const;
  weight_type capacity() const;

  void set_capacity( weight_type c );

  /**
   * \brief Get the efficiency of a variable.
   * \param obj The objective on which you want the efficiency.
   * \param index The index of the variable.
   */
  inline real_type get_efficiency( unsigned int obj, unsigned int index ) const
  {
    assert( index < size() );
    return m_problem.get_efficiency( obj, m_correspondence[index] );
  }

  unsigned int real_index( unsigned int index ) const;

  const problem_type& get_problem() const;

private:
  void make_indices( const set_of_variables_type& unwanted );
  void copy_indices( const set_of_variables_type& wanted );

private:
  /** \brief The parent problem. */
  const problem_type& m_problem;

  /** \brief Index of the variables kept in the problem. */
  std::vector<std::size_t> m_correspondence;

  /** \brief Remaining capacity. */
  weight_type m_capacity;

}; // class sub_problem_by_subset

#include "bikp/problem/impl/sub_problem_by_subset.tpp"

#endif // __BIKP_SUB_PROBLEM_BY_SUBSET_HPP__
