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
 * \file dynamic_programming_single.tpp
 * \brief Implementation of the dynamic_programming_single class.
 * \author Julien Jorge
 */
#include <list>

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename Problem>
dynamic_programming_single<Problem>::dynamic_programming_single
( const problem_type& p, const region_type& region )
  : m_problem(p), m_region(region), m_graph(m_problem, m_region)
{
  
} // dynamic_programming_single::dynamic_programming_single()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename Problem>
void dynamic_programming_single<Problem>::run( solution_type& sol ) const
{
  const typename graph_type::layer_type& layer =
    m_graph.get_layer( m_problem.size() );

  unsigned int best_index = layer.size();
  real_type best_val = sol.objective_value();

  if ( (real_type)m_region.min_profit() > best_val )
    best_val = m_region.min_profit();

  for (unsigned int i=0; i!=layer.size(); ++i)
    if ( (real_type)layer[i]->profit() > best_val )
      {
	best_val = layer[i]->profit();
	best_index = i;
      }

  if ( best_index != layer.size() )
    build_one_solution(sol, best_index);
} // dynamic_programming_single::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename Problem>
void dynamic_programming_single<Problem>::build_one_solution
( solution_type& sol, unsigned int index ) const
{
  sol.clear();

  const vertex_type* v = m_graph.get_layer( m_problem.size() )[index];

  do
    {
      const vertex_type* parent;

      if ( v->in_degree() == 1 )
	parent = &v->parent();
      else if ( v->parent_zero().profit() == v->profit() )
	parent = &v->parent_zero();
      else
	parent = &v->parent_one();

      if ( parent->weight() == v->weight() )
	sol.unset( parent->index() );
      else
	sol.set( parent->index() );

      v = parent;
    }
  while( v->index() > 0 );
} // dynamic_programming_single::build_one_solution()
