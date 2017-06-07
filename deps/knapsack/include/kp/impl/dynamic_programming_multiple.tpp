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
 * \brief Constructor.
 * \param p The problem to solve.
 * \param region The region where efficient solutions are located.
 */
template<typename View>
kp::dynamic_programming_multiple<View>::dynamic_programming_multiple
( const view_type& p, const bounded_region& region )
  : m_problem(p), m_region(region), m_graph(m_problem, m_region)
{

} // dynamic_programming_multiple::dynamic_programming_multiple()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the maximum complete set of solutions.
 * \param sols (out) The solutions.
 */
template<typename View>
void kp::dynamic_programming_multiple<View>::run
( std::list<knapsack_solution>& sols ) const
{
  const typename graph_type::layer_type& layer =
    m_graph.get_layer( m_problem.get_size() );

  std::list<unsigned int> best_index;
  moo::real_type best_val = m_region.get_lower_bound();

  for (unsigned int i=0; i!=layer.size(); ++i)
    {
      const moo::real_type val =
        layer[i]->profit().scalar_product(m_region.get_direction());

      if ( val == best_val )
        best_index.push_front(i);
      else if ( val > best_val )
        {
          best_index.clear();
          best_val = val;
          best_index.push_front(i);
        }
    }

  std::list<unsigned int>::const_iterator it;

  for (it=best_index.begin(); it!=best_index.end(); ++it)
    build_all_solutions(sols, *it);
} // dynamic_programming_multiple::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solutions ending in a given vertex.
 * \param sols (out) The solutions.
 * \param index The index of the vertex in the last layer, from which we start
 *        to build the solutions.
 */
template<typename View>
void kp::dynamic_programming_multiple<View>::build_all_solutions
( std::list<knapsack_solution>& sols, unsigned int index ) const
{
  knapsack_solution sol(m_problem);

  build_all_solutions_from
    ( sols, sol, *m_graph.get_layer( m_problem.get_size() )[index] );
} // dynamic_programming_multiple::build_all_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solutions ending in a given vertex.
 * \param sols (out) The solutions.
 * \param sol (out) The solution that we are building.
 * \param v The vertex in the last layer, from which we build the solutions.
 */
template<typename View>
void kp::dynamic_programming_multiple<View>::build_all_solutions_from
( std::list<knapsack_solution>& sols, knapsack_solution& sol,
  const vertex& v ) const
{
  if ( v.index() == 0 )
    sols.push_front(sol);
  else
    {
      const variable var = m_problem.get_variable(v.index() - 1);

      if ( v.in_degree() == 1 )
        {
          if ( v.parent().weight() == v.weight() )
            sol.unset( var );
          else
            sol.set( var );

          build_all_solutions_from( sols, sol, v.parent() );
          sol.free( var );
        }
      else
        {
          const moo::real_type p0 =
            v.parent_zero().profit().scalar_product(m_region.get_direction());
          const moo::real_type p1 =
            v.parent_one().profit().scalar_product(m_region.get_direction());
          const moo::real_type p =
            v.profit().scalar_product(m_region.get_direction());

          if ( p0 == p )
            {
              sol.unset( var );
              build_all_solutions_from( sols, sol, v.parent_zero() );
              sol.free( var );

              if ( p1 + var.cost.scalar_product(m_region.get_direction()) == p )
                {
                  sol.set( var );
                  build_all_solutions_from( sols, sol, v.parent_one() );
                  sol.free( var );
                }
            }
          else
            {
              sol.set( var );
              build_all_solutions_from( sols, sol, v.parent_one() );
              sol.free( var );
            }
        }
    }
} // dynamic_programming_multiple::build_all_solutions()

