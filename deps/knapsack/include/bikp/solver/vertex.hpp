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
 * \file vertex.hpp
 * \brief Minimum requirement for vertices in the dp_graph.
 * \author Julien Jorge
 */
#ifndef __BIKP_VERTEX_HPP__
#define __BIKP_VERTEX_HPP__

#include <cassert>

/**
 * \brief Minimum requirement for vertices in the dp_graph.
 * \author Julien Jorge
 */
template<typename Problem>
class vertex
{
public:
  /** \brief The type of the problem to solve. */
  typedef Problem problem_type;

  /** \brief Basic operations on profits. */
  typedef typename problem_type::profit_traits_type profit_traits_type;

  /** \brief The type of the profit of the variables. */
  typedef typename profit_traits_type::profit_type profit_type;

  /** \brief The type of the weight on the variables. */
  typedef typename problem_type::weight_type weight_type;

  /** \brief The type of the current class. */
  typedef vertex<problem_type> self_type;

public:
  vertex( const problem_type& p );
  vertex( unsigned int index, const self_type& that );
  vertex( unsigned int index, const self_type& that,
	  const weight_type& w, const profit_type& p );

  /** \brief Get the index of the layer/variable. */
  inline unsigned int index() const { return m_var_index; }

  const weight_type& weight() const;
  const profit_type& profit() const;
  const profit_type& max_profit() const;
  profit_type min_profit() const;

  /**
   * \brief Get the parent vertex, if the vertex has only one parent.
   * \pre in_degree() == 1
   */
  inline const self_type& parent() const
  {
    assert( in_degree() == 1 );
      
    if ( m_parent_one )
      return *m_parent_one;
    else
      return *m_parent_zero;
  } // parent()

  const self_type& parent_zero() const;
  const self_type& parent_one() const;

  unsigned int cardinality() const;

  /** \brief Get the number of in-edges. */
  inline unsigned int in_degree() const
  {
    unsigned int result = 0;

    if ( m_parent_zero )
      ++result;
      
    if ( m_parent_one )
      ++result;
      
    return result;
  } // in_degree()

  void amalgate( const self_type& v );
  void done();

  bool operator==( const self_type& that ) const;
  bool operator<( const self_type& that ) const;

private:
  /** \brief The index of the variable. */
  unsigned int m_var_index;
      
  /** \brief The accumulated weigth. */
  weight_type m_weight;
      
  /** \brief The best profit we can get in this node. */
  profit_type m_profit;

  /** \brief The best profit we can get on each objective in this node. */
  profit_type m_max_profit;

  /** \brief The parent node, with an edge having a profit equal to zero. */
  const self_type* m_parent_zero;
      
  /** \brief The parent node, with an edge having a profit greater than
      zero. */
  const self_type* m_parent_one;

  /** \brief The maximum cardinality of the solutions in in node. */
  unsigned int m_cardinality;

}; // class vertex

#include "bikp/solver/impl/vertex.tpp"

#endif // __BIKP_VERTEX_HPP__
