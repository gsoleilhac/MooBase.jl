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
 * \file vertex.cpp
 * \brief Implementation of the vertex class.
 * \author Julien Jorge.
 */
#include <kp/vertex.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 * \param p The size of the profit vectors.
 */
kp::vertex::vertex( unsigned int p )
  : m_var_index(0), m_weight(0), m_profit( p, 0 ), m_max_profit(m_profit),
    m_parent_zero(NULL), m_parent_one(NULL), m_cardinality(0)
{

} // vertex::vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Create a vertex by setting a variable to zero.
 * \param index The index of the layer/variable.
 * \param that The parent vertex.
 */
kp::vertex::vertex( unsigned int index, const vertex& that )
  : m_var_index(index), m_weight(that.m_weight),
    m_profit(that.m_profit), m_max_profit(that.m_max_profit),
    m_parent_zero(&that), m_parent_one(NULL), m_cardinality(that.m_cardinality)
{

} // vertex::vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Create a vertex by setting a variable to one.
 * \param index The index of the layer/variable.
 * \param that The parent vertex.
 * \param v The variable set to one.
 */
kp::vertex::vertex( unsigned int index, const vertex& that, const variable& v )
  : m_var_index(index), m_weight(that.m_weight + v.weight),
    m_profit(that.m_profit + v.cost), m_max_profit(that.m_max_profit + v.cost),
    m_parent_zero(NULL), m_parent_one(&that),
    m_cardinality(that.m_cardinality + 1)
{

} // vertex::vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the accumulated weight in this vertex.
 */
const moo::real_type& kp::vertex::weight() const
{
  return m_weight;
} // vertex::weight()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the best profit in this vertex.
 */
const kp::cost_vector& kp::vertex::profit() const
{
  return m_profit;
} // vertex::profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the best profit on each objective in this vertex.
 */
const kp::cost_vector& kp::vertex::max_profit() const
{
  return m_max_profit;
} // vertex::max_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum cardinality of the solutions in this node.
 */
unsigned int kp::vertex::cardinality() const
{
  return m_cardinality;
} // vertex::cardinality()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the parent vertex, with an edge of profit equal to zero.
 * \pre in_degree() > 0
 */
const kp::vertex& kp::vertex::parent_zero() const
{
  assert( in_degree() > 0 );
  assert( m_parent_zero );

  return *m_parent_zero;
} // vertex::parent_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the parent vertex, with an edge of profit greater than zero.
 * \pre in_degree() == 2
 */
const kp::vertex& kp::vertex::parent_one() const
{
  assert( in_degree() > 0 );
  assert( m_parent_one );

  return *m_parent_one;
} // vertex::parent_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Amalgate two vertices. The profit becomes the max of the two profits
 *        and the "empty" parent becomes the only parent of the other vertex.
 * \param v The vertex to amalgate with.
 * \param better Tell if \a v has a better value than \a this.
 * \pre (in_degree() == 1) && (v.in_degree() == 1) &&
 *      (weight() == v.weight()) && (index() == v.index())
 *      ( (m_parent_one != NULL) ^ (v.m_parent_one != NULL) ) &&
 *      ( (m_parent_zero != NULL) ^ (v.m_parent_zero != NULL) ) &&
 *      ( (m_parent_one != NULL) ^ (m_parent_zero != NULL) )
 */
void kp::vertex::amalgate( const vertex& v, bool better )
{
  assert( index() == v.index() );
  assert( weight() == v.weight() );
  assert( (m_parent_one != NULL) ^ (v.m_parent_one != NULL) );
  assert( (m_parent_zero != NULL) ^ (v.m_parent_zero != NULL) );
  assert( (m_parent_one != NULL) ^ (m_parent_zero != NULL) );

  if ( m_parent_zero )
    m_parent_one = v.m_parent_one;
  else
    m_parent_zero = v.m_parent_zero;

  if ( better )
    m_profit = v.m_profit;

  for (unsigned int i=0; i!=m_max_profit.size(); ++i)
    m_max_profit[i] = std::max(m_max_profit[i], v.m_max_profit[i]);

  if ( v.m_cardinality > m_cardinality )
    m_cardinality = v.m_cardinality;

  assert( in_degree() == 2 );
} // vertex::amalgate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the vertex that it won't be used anymore during the build
 *        process of the graph.
 */
void kp::vertex::done()
{
  // nothing to do
} // vertex::done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two vertices are equals.
 * \param that The vertex to compare to.
 * \return (index() == that.index()) && (weight() == that.weight())
 */
bool kp::vertex::operator==( const vertex& that ) const
{
  return (index() == that.index()) && (weight() == that.weight());
} // vertex::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two vertices lexicographically.
 * \param that The vertex to compare to.
 */
bool kp::vertex::operator<( const vertex& that ) const
{
  if ( index() == that.index() )
    return weight() < that.weight();
  else
    return index() < that.index();
} // vertex::operator<()
