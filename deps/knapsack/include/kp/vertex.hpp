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
#ifndef __KP_VERTEX_HPP__
#define __KP_VERTEX_HPP__

#include <cassert>
#include <kp/variable.hpp>

namespace kp
{
  /**
   * \brief Minimum requirement for vertices in the dp_graph.
   * \author Julien Jorge
   */
  class vertex
  {
  public:
    explicit vertex( unsigned int p );
    vertex( unsigned int index, const vertex& that );
    vertex( unsigned int index, const vertex& that, const variable& v );

    /** \brief Get the index of the layer/variable. */
    inline unsigned int index() const { return m_var_index; }

    const moo::real_type& weight() const;
    const cost_vector& profit() const;
    const cost_vector& max_profit() const;

    /**
     * \brief Get the parent vertex, if the vertex has only one parent.
     * \pre in_degree() == 1
     */
    inline const vertex& parent() const
    {
      assert( in_degree() == 1 );
      
      if ( m_parent_one )
        return *m_parent_one;
      else
        return *m_parent_zero;
    } // parent()

    const vertex& parent_zero() const;
    const vertex& parent_one() const;

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

    void amalgate( const vertex& v, bool better );
    void done();

    bool operator==( const vertex& that ) const;
    bool operator<( const vertex& that ) const;

  private:
    /** \brief The index of the variable. */
    unsigned int m_var_index;
      
    /** \brief The accumulated weigth. */
    moo::real_type m_weight;
      
    /** \brief The best profit we can get in this node. */
    cost_vector m_profit;

    /** \brief The best profit we can get on each objective in this node. */
    cost_vector m_max_profit;

    /** \brief The parent node, with an edge having a profit equal to zero. */
    const vertex* m_parent_zero;
      
    /** \brief The parent node, with an edge having a profit greater than
        zero. */
    const vertex* m_parent_one;

    /** \brief The maximum cardinality of the solutions in in node. */
    unsigned int m_cardinality;

  }; // class vertex
} // namespace kp

#endif // __KP_VERTEX_HPP__
