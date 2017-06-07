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
 * \file dynamic_programming_multiple.cpp
 * \brief Implementation of the dynamic_programming_multiple class.
 * \author Julien Jorge
 */
#include "bikp/solver/dynamic_programming_multiple.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
dynamic_programming_multiple::dynamic_programming_multiple
( const problem_type& p, const region_type& region )
  : super(p, region)
{
  
} // dynamic_programming_multiple::dynamic_programming_multiple()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
void dynamic_programming_multiple::run( std::list<solution_type>& sols ) const
{
  const graph_type::layer_type& layer = m_graph.get_layer( m_problem.size() );

  std::list<unsigned int> best_index;
  real_type best_val = m_region.min_profit();

  for (unsigned int i=0; i!=layer.size(); ++i)
    if ( (real_type)layer[i]->profit() == best_val )
      best_index.push_front(i);
    else if ( (real_type)layer[i]->profit() > best_val )
      {
	best_index.clear();
	best_val = (real_type)layer[i]->profit();
	best_index.push_front(i);
      }

  std::list<unsigned int>::const_iterator it;

  for (it=best_index.begin(); it!=best_index.end(); ++it)
    build_all_solutions(sols, *it);
} // dynamic_programming_multiple::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
void dynamic_programming_multiple::build_all_solutions
( std::list<solution_type>& sols, unsigned int index ) const
{
  solution_type sol(m_problem);

  build_all_solutions_from
    ( sols, sol, *m_graph.get_layer( m_problem.size() )[index] );
} // dynamic_programming_multiple::build_all_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
void dynamic_programming_multiple::build_all_solutions_from
( std::list<solution_type>& sols, solution_type& sol,
  const vertex_type& v ) const
{
  if ( v.index() == 0 )
    sols.push_front(sol);
  else if ( v.in_degree() == 1 )
    {
      if ( v.parent().weight() == v.weight() )
	sol.unset( v.index() - 1 );
      else
	sol.set( v.index() - 1 );

      build_all_solutions_from( sols, sol, v.parent() );
      sol.free( v.index() - 1 );
    }
  else
    {
      real_type p0 = (real_type)v.parent_zero().profit();
      real_type p1 = (real_type)v.parent_one().profit();
      real_type p = (real_type)v.profit();

      if ( p0 == p )
	{
	  sol.unset( v.index() - 1 );
	  build_all_solutions_from( sols, sol, v.parent_zero() );
	  sol.free( v.index() - 1 );

	  if ( p1 + (real_type)m_problem.profit(v.index() - 1) == p )
	    {
	      sol.set( v.index() - 1 );
	      build_all_solutions_from( sols, sol, v.parent_one() );
	      sol.free( v.index() - 1 );
	    }
	}
      else
	{
	  sol.set( v.index() - 1 );
	  build_all_solutions_from( sols, sol, v.parent_one() );
	  sol.free( v.index() - 1 );
	}
    }
} // dynamic_programming_multiple::build_all_solutions()

