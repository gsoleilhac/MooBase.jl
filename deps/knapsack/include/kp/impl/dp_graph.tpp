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

#include "bikp/problem/sub_problem_by_interval.hpp"
#include "bikp/relaxation/mt_linear_relaxation.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem for which we build the graph.
 * \param region The region in which the solutions are.
 */
template<class View>
kp::dp_graph<View>::dp_graph( const view_type& p, const bounded_region& region )
  : m_layers(p.get_size() + 1)
{
  build_graph( p, region );
} // dp_graph::dp_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<class View>
kp::dp_graph<View>::~dp_graph()
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
template<class View>
const typename kp::dp_graph<View>::layer_type&
kp::dp_graph<View>::get_layer( unsigned int i ) const
{
  assert( i < m_layers.size() );
  return m_layers[i];
} // dp_graph::get_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the graph of the knapsack problem.
 * \param p The problem for which we build the graph.
 * \param region The region in which the solutions are.
 */
template<class View>
void kp::dp_graph<View>::build_graph
( const view_type& p, const bounded_region& region )
{
  mono_problem mono( p.get_size(), p.get_capacity() );

  for (unsigned int j=0; j!=p.get_size(); ++j)
    mono.set_variable
      ( j, p.get_variable(j).cost.scalar_product(region.get_direction()),
        p.get_variable(j).weight );

  m_layers[0].push_back( new vertex(p.get_nb_objectives()) );

  for (unsigned int i=0; i!=m_layers.size()-1; ++i)
    {
      std::queue<vertex*> pending;
      relax_cache_type relax_cache(p.get_capacity()+1, -1);

      for (unsigned int j=0; j!=m_layers[i].size(); ++j)
        {
          vertex& v = *m_layers[i][j];
          vertex* skip = new vertex( i + 1, v );

          if ( interesting(relax_cache, mono, *skip, region) )
            insert_vertex_skip(skip, pending, region.get_direction());
          else
            delete skip;

          const variable& var = p.get_variable(i);

          if ( v.weight() + var.weight <= p.get_capacity() )
            {
              vertex* keep = new vertex( i + 1, v, var );

              if ( interesting(relax_cache, mono, *keep, region) )
                insert_vertex_keep(keep, pending);
              else
                delete keep;
            }

          v.done();
        }

      while ( !pending.empty() )
        {
          push_in_layer( pending.front(), region.get_direction() );
          pending.pop();
        }
    }
} // dp_graph::build_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in the graph, the vertex is made by skiping the variable
 *        of its parent.
 * \param skip Vertex made when we don't take the variable of the parent vertex.
 * \param pending (in/out) The nodes of the next layer waiting to be added.
 * \param dir The direction in which we optimise.
 */
template<class View>
void kp::dp_graph<View>::insert_vertex_skip
( vertex* skip, std::queue<vertex*>& pending, const cost_vector& dir )
{
  bool ok = false;
  
  while ( !ok && !pending.empty() )
    if ( skip->weight() < pending.front()->weight() )
      ok = true;
    else
      {
        push_in_layer( pending.front(), dir );
        pending.pop();
      }

  push_in_layer(skip, dir);
} // dp_graph::insert_vertex_skip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in the graph, the vertex is made by taking the variable
 *        of its parent.
 * \param keep Vertex made when we take the variable of the parent vertex.
 * \param pending (in/out) The nodes of the next layer waiting to be added.
 */
template<class View>
void kp::dp_graph<View>::insert_vertex_keep
( vertex* keep, std::queue<vertex*>& pending )
{
  pending.push(keep);
} // dp_graph::insert_vertex_keep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vertex in its layer.
 * \param v The vertex.
 * \param dir The direction in which we optimise.
 */
template<class View>
void kp::dp_graph<View>::push_in_layer( vertex* v, const cost_vector& dir )
{
  layer_type& layer = m_layers[v->index()];

  assert( layer.empty() ? true : (*(--layer.end()))->weight() <= v->weight() );

  if ( layer.empty() )
    layer.push_back(v);
  else if ( layer[ layer.size() - 1 ]->weight() == v->weight() )
    {
      const moo::real_type v_mono = v->profit().scalar_product(dir);
      const moo::real_type s_mono =
        layer[ layer.size() - 1 ]->profit().scalar_product(dir);

      layer[ layer.size() - 1 ]->amalgate(*v, v_mono > s_mono );
      delete v;
    }
  else
    layer.push_back(v);
} // dp_graph::push_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if it's interesting to build the graph after a given vertex.
 * \param relax_cache (in/out) The result of the previous relaxations.
 * \param p The mono-objective version of the problem for which we're building
 *          the graph.
 * \param v The vertex from which we check.
 * \param region The region in which the solutions are.
 * 
 * We compute a linear relaxation on the variables in range
 * [v.index(), p.size()) and add the best profit we can get in vertex \a v. If
 * the sum is lesser than the lower bound, it's not interesting.
 */
template<class View>
bool kp::dp_graph<View>::interesting
( relax_cache_type& relax_cache, const mono_problem& p, const vertex& v,
  const bounded_region& region ) const
{
  const moo::real_type c( p.capacity() - v.weight() );

  if ( relax_cache[c] == -1 )
    {
      typedef sub_problem_by_interval<mono_problem> sub_problem_type;

      sub_problem_type sub_p( p, v.index(), p.size() - 1, c );

      mt_linear_relaxation<sub_problem_type> rl;
      relax_cache[c] = rl.calculate(sub_p).objective_value;
    }

  return relax_cache[c] + v.max_profit().scalar_product(region.get_direction())
    >= region.get_lower_bound();
} // dp_graph::interesting()
