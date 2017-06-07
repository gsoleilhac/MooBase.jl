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
 * \file dp_graph.tpp
 * \brief Implementation of the dp_graph class.
 * \author Julien Jorge.
 */
#include <fstream>
#include <iostream>
#include <set>
#include <cassert>

#include "bikp/problem/transform.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem for which we build the graph.
 * \param region The region in which the solutions are.
 */
template<class Framework>
dp_graph<Framework>::dp_graph
( const problem_type& p, const region_type& region )
  : m_layers(p.size() + 1)
{
  build_graph( p, region );
} // dp_graph::dp_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<class Framework>
dp_graph<Framework>::~dp_graph()
{
  for (unsigned int i=0; i!=m_layers.size(); ++i)
    for (unsigned int j=0; j!=m_layers[i].size(); ++j)
      delete m_layers[i][j];
} // dp_graph::~dp_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a layer of the graphs. (ie. all vertices with the same var_index).
 * \param i The index of the layer.
 */
template<class Framework>
const typename dp_graph<Framework>::layer_type&
dp_graph<Framework>::get_layer( unsigned int i ) const
{
  assert( i < m_layers.size() );
  return m_layers[i];
} // dp_graph::get_layer()

/*----------------------------------------------------------------------------*/
template<class Framework>
void dp_graph<Framework>::output_dot
( const problem_type& p, const std::string& filename ) const
{
  std::ofstream f( filename.c_str() );

  if (f)
    {
      f << "digraph g\n{\n";
      f << "graph [ rank = \"min\" rankdir = \"LR\" ];\n";
      //f << "node [ fontsize = \"14\"\n";
      //f << "       shape = \"ellipse\" ];\n";
      f << "node [ shape = \"box\" ];\n";

      for (unsigned int i=0; i!=m_layers.size(); ++i)
	for (unsigned int j=0; j!=m_layers[i].size(); ++j)
	  output_dot( p, f, *m_layers[i][j] );

      f << "}\n";
      f.close();
    }
} // dp_graph::output_dot()

/*----------------------------------------------------------------------------*/
template<class Framework>
void dp_graph<Framework>::output_dot
( const problem_type& p, std::ofstream& f, const vertex_type& v ) const
{
  f << "\"" << v.index() << "_" << v.weight() << "\" [\n"
    << "label = \"<f0> " << v.weight() << "| <f1>"
    << v.profit().profit_1 << ", " << v.profit().profit_2 << "\"\n"
    << "shape = \"record\"\n];\n";

  //f << "\"" << v.index() << "_" << v.weight() << "\" [\n"
  //  << "shape = \"point\"\n];\n";

  if ( v.in_degree() == 1 )
    output_dot_edge( p, f, v.parent(), v );
  else if ( v.in_degree() == 2 )
    {
      output_dot_edge( p, f, v.parent_zero(), v );
      output_dot_edge( p, f, v.parent_one(), v );
    }
} // dp_graph::output_dot()

/*----------------------------------------------------------------------------*/
template<class Framework>
void dp_graph<Framework>::output_dot_edge
( const problem_type& p, std::ofstream& f,
  const vertex_type& s, const vertex_type& t ) const
{
  f << "\"" << s.index() << "_" << s.weight() << "\":f0 -> \""
    << t.index() << "_" << t.weight() << "\":f0 [\n"
    << "label = \"";

  if ( t.weight() == s.weight() )
    f << 0;
  else
    f << p.profit(s.index()).profit_1 << ", " << p.profit(s.index()).profit_2;

  f << "\"\n];\n";

/*  f << "\"" << s.index() << "_" << s.weight() << "\" -- \""
    << t.index() << "_" << t.weight() << "\" [\n"
    << "shape = \"rvee\"\n];\n";*/
} // dp_graph::output_dot_edge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the graph of the knapsack problem.
 * \param p The problem for which we build the graph.
 * \param region The region in which the solutions are.
 */
template<class Framework>
void dp_graph<Framework>::build_graph
( const problem_type& p, const region_type& region )
{
  upper_bound_type ub(p);

  m_layers[0].push_back( new vertex_type(p) );

  for (unsigned int i=0; i!=m_layers.size()-1; ++i)
    {
      //std::cout << "building layer " << i << ": " << std::flush;
      std::queue<vertex_type*> pending;

      for (unsigned int j=0; j!=m_layers[i].size(); ++j)
	{
	  //if ( j % 10 == 0 )
	  //std::cout << " " << m_layers[i].size() - j << std::flush;

	  vertex_type& v = *m_layers[i][j];
	  
	  vertex_type* skip = new vertex_type( i + 1, v );

	  if ( interesting(ub, p, *skip, region) )
	    insert_vertex_skip(skip, pending);
	  else
	    delete skip;

	  if ( v.weight() + p.weight(i) <= p.capacity() )
	    {
	      vertex_type* keep =
		new vertex_type( i + 1, v, p.weight(i), p.profit(i) );  

	      if ( interesting(ub, p, *keep, region) )
		insert_vertex_keep(keep, pending);
	      else
		delete keep;
	    }

	  v.done();
	}

      while ( !pending.empty() )
	{
	  push_in_layer( pending.front() );
	  pending.pop();
	}

      //std::cout << std::endl;
    }

#ifdef PRINT_GRAPH_STATS
  output_stats( p, region.min_profit() );
#endif
} // dp_graph::build_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in the graph, the vertex is made by skiping the variable
 *        of its parent.
 * \param skip Vertex made when we don't take the variable of the parent vertex.
 * \param pending (in/out) The nodes of the next layer waiting to be added.
 */
template<class Framework>
void dp_graph<Framework>::insert_vertex_skip
( vertex_type* skip, std::queue<vertex_type*>& pending )
{
  bool ok = false;
  
  while ( !ok && !pending.empty() )
    if ( skip->weight() < pending.front()->weight() )
      ok = true;
    else
      {
	push_in_layer( pending.front() );
	pending.pop();
      }

  push_in_layer(skip);
} // dp_graph::insert_vertex_skip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in the graph, the vertex is made by taking the variable
 *        of its parent.
 * \param keep Vertex made when we take the variable of the parent vertex.
 * \param pending (in/out) The nodes of the next layer waiting to be added.
 */
template<class Framework>
void dp_graph<Framework>::insert_vertex_keep
( vertex_type* keep, std::queue<vertex_type*>& pending )
{
  pending.push(keep);
} // dp_graph::insert_vertex_keep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in its layer.
 * \param v The vertex.
 */
template<class Framework>
void dp_graph<Framework>::push_in_layer( vertex_type* v )
{
  layer_type& layer = m_layers[v->index()];

  assert( layer.empty() ? true : (*(--layer.end()))->weight() <= v->weight() );

  if ( layer.empty() )
    layer.push_back(v);
  else if ( layer[ layer.size() - 1 ]->weight() == v->weight() )
    {
      layer[ layer.size() - 1 ]->amalgate(*v);
      delete v;
    }
  else
    layer.push_back(v);
} // dp_graph::push_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if it's interesting to build the graph after a given vertex.
 * \param ub The upper bound object to use to compute the upper bound.
 * \param p The mono-objective version of the problem for which we're building
 *          the graph.
 * \param v The vertex from which we check.
 * \param region The region in which the solutions are.
 * 
 * We compute a linear relaxation on the variables in range
 * [v.index(), p.size()) and add the best profit we can get in vertex \a v. If
 * the sum is lesser than the lower bound, it's not interesting.
 */
template<class Framework>
bool dp_graph<Framework>::interesting
( const upper_bound_type& ub, const problem_type& p, const vertex_type& v,
  const region_type& region ) const
{
  if ( region.feasible( v.min_profit() ) )
    return ub.interesting( v.max_profit(), region.min_profit(), v.index(),
			   p.capacity() - v.weight() );
  else
    {
      //std::cout << "COUPE " << v.index() << "/" << p.size() << std::endl;
      return false;
    }
} // dp_graph::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output some statistics on the graph :
 *  - the number of variables in the problem,
 *  - the number of useless vertices,
 *  - the number of vertices.
 *
 * \param p The problem for which we made the graph.
 * \param lower_bound The objective value of the best known solution.
 */
template<class Framework>
void dp_graph<Framework>::output_stats
( const problem_type& p, const bound_type& lower_bound ) const
{
  unsigned int i = m_layers.size() - 1;
  std::set<const vertex_type*> useful;

  for (unsigned int j=0; j!=m_layers[i].size(); ++j )
    if ( m_layers[i][j]->profit() >= lower_bound )
      useful.insert( m_layers[i][j] );

  unsigned int useless = m_layers[i].size() - useful.size();
  unsigned int n = m_layers[i].size();
  unsigned int y_node = 0;

  while (i!=0)
    {
      --i;

      std::set<const vertex_type*> reachable;
      typename std::set<const vertex_type*>::const_iterator it;

      for (it=useful.begin(); it!=useful.end(); ++it)
	if ( (*it)->in_degree() == 1 )
	  reachable.insert( &(*it)->parent() );
	else
	  {
	    reachable.insert( &(*it)->parent_zero() );
	    reachable.insert( &(*it)->parent_one() );
	    ++y_node;
	  }

      useless += m_layers[i].size() - reachable.size();
      n += m_layers[i].size();
      useful = reachable;
    }

  std::cout << p.size() << "\t" << useless << "\t" << n << "\t"
	    << paths_count() << "\t" << y_node << "\t# GSTAT" << std::endl;
} // dp_graph::output_stats()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of paths in the graph.
 */
template<class Framework>
real_type dp_graph<Framework>::paths_count() const
{
  std::vector< std::map<weight_type, real_type> >
    count_table( m_layers.size() );

  count_table[0][0] = 1;
  real_type result = 0;

  const layer_type& layer = m_layers.back();

  for (unsigned int i=0; i!=layer.size(); ++i)
    result += paths_count( *layer[i], count_table );

  return result;
} // dp_graph::paths_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of paths between a given vertex and the source vertex.
 * \param v The vertex from which we count.
 * \param count_table The distance already computed.
 */
template<class Framework>
real_type dp_graph<Framework>::paths_count
( const vertex_type& v,
  std::vector< std::map<weight_type, real_type> >& count_table ) const
{
  const unsigned int index = v.index();
  weight_type w = v.weight();
  real_type result;

  if ( count_table[index].find(w) != count_table[index].end() )
    result = count_table[index][w];
  else
    {
      if ( v.in_degree() == 1 )
	result = paths_count( v.parent(), count_table );
      else
	result = paths_count( v.parent_zero(), count_table )
	  + paths_count( v.parent_one(), count_table );

      count_table[index][w] = result;
    }

  return result;
} // dp_graph::paths_count()

