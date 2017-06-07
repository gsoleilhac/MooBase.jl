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
 * \file vertex.tpp
 * \brief Implementation of the vertex class.
 * \author Julien Jorge.
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template<typename Problem>
vertex<Problem>::vertex( const problem_type& p )
  : m_var_index(0), m_parent_zero(NULL), m_parent_one(NULL), m_cardinality(0)
{
  m_profit *= 0;
  m_max_profit = m_profit;
  m_weight *= 0;
} // vertex::vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Create a vertex by setting a variable to zero.
 * \param index The index of the layer/variable.
 * \param that The parent vertex.
 */
template<typename Problem>
vertex<Problem>::vertex( unsigned int index, const self_type& that )
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
 * \param w The weight of the variable.
 * \param p The profit of the variable.
 */
template<typename Problem>
vertex<Problem>::vertex
( unsigned int index, const self_type& that, const weight_type& w,
  const profit_type& p )
  : m_var_index(index), m_weight(that.m_weight + w),
    m_profit(that.m_profit + p), m_max_profit(that.m_max_profit + p),
    m_parent_zero(NULL), m_parent_one(&that),
    m_cardinality(that.m_cardinality + 1)
{

} // vertex::vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the accumulated weight in this vertex.
 */
template<typename Problem>
const typename vertex<Problem>::weight_type& vertex<Problem>::weight() const
{
  return m_weight;
} // vertex::weight()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the best profit in this vertex.
 */
template<typename Problem>
const typename vertex<Problem>::profit_type& vertex<Problem>::profit() const
{
  return m_profit;
} // vertex::profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the best profit on each objective in this vertex.
 */
template<typename Problem>
const typename vertex<Problem>::profit_type& vertex<Problem>::max_profit() const
{
  return m_max_profit;
} // vertex::max_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum profit on each objective in this vertex.
 */
template<typename Problem>
typename vertex<Problem>::profit_type vertex<Problem>::min_profit() const
{
  profit_type result;
  result *= 0;
  return result;
} // vertex::min_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum cardinality of the solutions in this node.
 */
template<typename Problem>
unsigned int vertex<Problem>::cardinality() const
{
  return m_cardinality;
} // vertex::cardinality()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the parent vertex, with an edge of profit equal to zero.
 * \pre in_degree() > 0
 */
template<typename Problem>
const typename vertex<Problem>::self_type& vertex<Problem>::parent_zero() const
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
template<typename Problem>
const typename vertex<Problem>::self_type& vertex<Problem>::parent_one() const
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
 * \pre (in_degree() == 1) && (v.in_degree() == 1) &&
 *      (weight() == v.weight()) && (index() == v.index())
 *      ( (m_parent_one != NULL) ^ (v.m_parent_one != NULL) ) &&
 *      ( (m_parent_zero != NULL) ^ (v.m_parent_zero != NULL) ) &&
 *      ( (m_parent_one != NULL) ^ (m_parent_zero != NULL) )
 */
template<typename Problem>
void vertex<Problem>::amalgate( const self_type& v )
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

  if ( (real_type)v.m_profit > (real_type)m_profit )
    m_profit = v.m_profit;

  m_max_profit = profit_traits_type::ideal(m_max_profit, v.m_max_profit);

  if ( v.m_cardinality > m_cardinality )
    m_cardinality = v.m_cardinality;

  assert( in_degree() == 2 );
} // vertex::amalgate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the vertex that it won't be used anymore during the build
 *        process of the graph.
 */
template<typename Problem>
void vertex<Problem>::done()
{
  // nothing to do
} // vertex::done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two vertices are equals.
 * \param that The vertex to compare to.
 * \return (index() == that.index()) && (weight() == that.weight())
 */
template<typename Problem>
bool vertex<Problem>::operator==( const self_type& that ) const
{
  return (index() == that.index()) && (weight() == that.weight());
} // vertex::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two vertices lexicographically.
 * \param that The vertex to compare to.
 */
template<typename Problem>
bool vertex<Problem>::operator<( const self_type& that ) const
{
  if ( index() == that.index() )
    return weight() < that.weight();
  else
    return index() < that.index();
} // vertex::operator<()
