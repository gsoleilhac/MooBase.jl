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
 * \file base_second_phase.cpp
 * \brief Implementation of the base_second_phase class.
 * \author Julien Jorge
 */
#include "bikp/phase/base_second_phase.hpp"
#include "bikp/problem/transform.hpp"
#include "bikp/reduction/variable_fixing_by_bound_set.hpp"
#include "bikp/reduction/extern_variable_fixing.hpp"
#include "bikp/reduction/variable_fixing_by_direction.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
base_second_phase::base_second_phase( const std::string& name )
  : m_logger(name + "_second_phase")
{

} // base_second_phase::base_second_phase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the algorithm.
 * \param p The problem to solve.
 * \param Z_SE The images of the supported solutions.
 */
void base_second_phase::run
( const bi_problem& p, const std::list<bi_solution>& X_SE )
{
  assert(X_SE.size() >= 2);

  variable_fixing_by_direction red(p, X_SE);

  const bi_problem& sub_p = red.get_sub_problem();
  bi_profit guaranteed_profit = red.guaranteed_profit();

  std::cout << "Global reduction from " << p.size() << " to " << sub_p.size()
	    << " variables."
	    << std::endl;

  std::list<bi_solution> sub_X_SE;
  std::list<bi_solution>::const_iterator it;
  
  for ( it=X_SE.begin(); it!=X_SE.end(); ++it )
    if ( (it->objective_value().profit_1 >= guaranteed_profit.profit_1)
	 && (it->objective_value().profit_2 >= guaranteed_profit.profit_2) )
      {
	bi_solution sub_sol( sub_p );
	red.create_solution( *it, sub_sol );
	sub_X_SE.push_back( sub_sol );
      }

  reduce_and_run( sub_p, sub_X_SE );

  maximum_complete_set<bi_solution> sub_sols = m_X_NSE;
  maximum_complete_set<bi_solution>::const_iterator it_s;
    
  m_X_NSE.clear();

  for (it_s=sub_sols.begin(); it_s!=sub_sols.end(); ++it_s)
    {
      bi_solution s(p);
      red.restore_solution( *it_s, s );
      m_X_NSE.insert(s);
    }
} // base_second_phase::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solutions found by the algorithm.
 */
const maximum_complete_set<bi_solution>&
base_second_phase::get_solutions() const
{
  return m_X_NSE;
} // base_second_phase::get_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the images of the non supported solutions.
 * \param os Output stream for solutions.
 */
void base_second_phase::save( std::ostream& os ) const
{
  maximum_complete_set<bi_solution>::const_iterator it;

  for ( it=m_X_NSE.begin(); it!=m_X_NSE.end(); ++it )
    {
      os << it->objective_value().profit_1 << "\t"
	 << it->objective_value().profit_2 << "\t"
	 << it->weight() << "\t";

      for (unsigned int i=0; i!=it->size(); ++i)
	if ( it->is_set(i) )
	  os << "1";
	else
	  os << "0";

      os << std::endl;
    }
} // base_second_phase::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the algorithm.
 * \param p The problem to solve.
 * \param Z_SE The images of the supported solutions.
 */
void base_second_phase::reduce_and_run
( const bi_problem& p, const std::list<bi_solution>& X_SE )
{
  std::list<bi_profit> Z_SE;
  std::list<bi_solution>::const_iterator it;
  
  for ( it=X_SE.begin(); it!=X_SE.end(); ++it )
    Z_SE.push_back( it->objective_value() );

  run_solver( p, Z_SE );
} // base_second_phase::reduce_and_run()

/*----------------------------------------------------------------------------*/
void base_second_phase::run_solver
( const bi_problem& p, const std::list<bi_profit>& Z_SE )
{
  std::vector<triangle> triangles;
  std::list<bi_profit>::const_iterator it1;
  std::list<bi_profit>::const_iterator it2;

  it1 = Z_SE.begin();
  it2 = it1;
  ++it2;

  for( ; it2 != Z_SE.end(); ++it1, ++it2 )
    {
      m_pareto_front.insert(*it1);
      triangles.push_back( triangle(*it1, *it2) );
      m_solution_sets.add( triangles.back() );
    }

  m_pareto_front.insert(*it1);

  pre_sort( triangles );

  unsigned int i = next_triangle_index( triangles );
  unsigned int progress = 1;

  while ( i!=triangles.size() )
    {
      m_logger.log() << progress << "/" << triangles.size() << " "
		     << triangles[i].top_left().profit_1
		     << ","
		     << triangles[i].top_left().profit_2
		     << " to "
		     << triangles[i].bottom_right().profit_1
		     << ","
		     << triangles[i].bottom_right().profit_2
		     << " ("
		     << (triangles[i].width() * triangles[i].height()) / 2
		     << ")"
		     << std::endl;

      calculate_triangle( p, triangles[i] );

      m_solution_sets.lock( triangles[i] );

      i = next_triangle_index( triangles );
      ++progress;
    }
} // base_second_phase::run_solver()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the triangles in the order you want to explore them.
 * \param triangles The triangles to order.
 */
void base_second_phase::pre_sort( std::vector<triangle>& triangles ) const
{
  // nothing to do
} // base_second_phase::save()

/*----------------------------------------------------------------------------*/
unsigned int base_second_phase::next_triangle_index
( const std::vector<triangle>& triangles ) const
{
  unsigned int index = triangles.size();
  real_type w(0);

  for (unsigned int i=0; i!=triangles.size(); ++i)
    if ( !m_solution_sets.locked( triangles[i] ) )
      if ( m_solution_sets.strip_width( triangles[i] ) > w )
	w = m_solution_sets.strip_width( triangles[i] );

  for (unsigned int i=0; i!=triangles.size(); ++i)
    if ( !m_solution_sets.locked( triangles[i] ) )
      if ( m_solution_sets.strip_width( triangles[i] ) <= w )
	{
	  index = i;
	  w = m_solution_sets.strip_width( triangles[i] );
	}

  return index;
} // base_second_phase::next_triangle_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate solutions inside a triangle made by two successive points.
 * \param p The problem to solve.
 * \param t The triangle to solve.
 */
void
base_second_phase::calculate_triangle( const bi_problem& p, const triangle& t )
{
  std::list<bi_solution> new_solutions;
  combined_problem mono_p(p, t.height(), t.width());

  std::list<combined_solution> mono_s;
  std::list<combined_profit> out_points;

  mono_p.sort_by_decreasing_efficiency();

  solve( mono_p, t, mono_s, out_points );

  transform::restore_solutions( mono_p, p, mono_s, new_solutions );

  m_X_NSE.insert( new_solutions.begin(), new_solutions.end() );
  
  m_solution_sets.insert( out_points.begin(), out_points.end() );
  m_pareto_front.insert( new_solutions.begin(), new_solutions.end() );
  m_pareto_front.insert( out_points.begin(), out_points.end() );

  m_solution_sets.tighten();
} // base_second_phase::calculate_triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the set of solutions.
 * \param mono_p Combined problem to solve.
 * \param t The triangle to explore.
 * \param new_solutions (in/out) The calculated solutions, in the triangle.
 * \param out_points (in/out) The values of the solutions found outside the
 *        triangle.
 */
void base_second_phase::solve
( const combined_problem& mono_p, const triangle& t,
  std::list<combined_solution>& new_solutions,
  std::list<combined_profit>& out_points )
{
  const real_type lambda_1( t.height() );
  const real_type lambda_2( t.width() );
  const combined_profit z_left(lambda_1, lambda_2,  t.top_left() );
  const combined_profit z_right( lambda_1, lambda_2, t.bottom_right());

  real_type lb_lambda = m_solution_sets.get_lower_bound(t);

  combined_reduction_type* reduction =
    reduce( mono_p, z_left, z_right, lb_lambda );

  const combined_problem& sub_p = reduction->get_sub_problem();

  m_logger.log() << "\tProblem reduced from " << mono_p.size() << " to "
                 << sub_p.size() << " variables." << std::endl;

  if ( sub_p.size() > 0 )
    {
      std::list<combined_solution> sub_sols;
      std::list<combined_profit> sub_out_points;
      pareto_set<combined_profit> ub;

      combined_profit z1_opt( z_right - reduction->guaranteed_profit() );
      combined_profit z2_opt( z_left - reduction->guaranteed_profit() );

      assert( lb_lambda >= (real_type)reduction->guaranteed_profit() );
      lb_lambda -= (real_type)reduction->guaranteed_profit();

      scale_known_solutions
	( ub, reduction->guaranteed_profit(), lambda_1, lambda_2, t );

      combined_triangle_front region
	( true, z2_opt, z1_opt, lb_lambda, lambda_1, lambda_2 );

      solve( sub_p, region, sub_sols, sub_out_points );

      std::list<combined_solution>::const_iterator it;

      for (it=sub_sols.begin(); it!=sub_sols.end(); ++it)
	{
	  combined_solution s(mono_p);
	  reduction->restore_solution( *it, s );
	  new_solutions.push_front(s);
	}

      std::list<combined_profit>::const_iterator it_p;
  
      for (it_p=sub_out_points.begin(); it_p!=sub_out_points.end(); ++it_p)
	out_points.push_front( *it_p + reduction->guaranteed_profit() );
    }

  delete reduction;
} // base_second_phase::solve()

/*----------------------------------------------------------------------------*/
void base_second_phase::mono_to_sub_mono
( const combined_reduction_type& reduction,
  const std::list<combined_solution>& new_solutions,
  std::list<combined_solution>& sols, real_type min_lambda ) const
{
  std::list<combined_solution>::const_iterator it;

  for (it=new_solutions.begin(); it!=new_solutions.end(); ++it)
    if ( (real_type)it->objective_value() > min_lambda )
      {
	combined_solution s(reduction.get_sub_problem());
	reduction.create_solution( *it, s );
	sols.push_front(s);
      }

} // base_second_phase::mono_to_sub_mono()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rescale the known points to fit to a sub problem.
 * \param ub (out) The known points, scaled.
 * \param delta The guaranteed profit on all objectives.
 * \param lambda_1 Coefficient on the first objective.
 * \param lambda_2 Coefficient on the second objective.
 * \param t The triangle in which the solutions are.
 */
void base_second_phase::scale_known_solutions
( pareto_set<combined_profit>& ub, const combined_profit& delta,
  real_type lambda_1, real_type lambda_2, const triangle& t ) const
{
  pareto_set<bi_profit>::const_iterator it;

  for ( it=m_pareto_front.begin(); it!=m_pareto_front.end(); ++it )
    {
      combined_profit p( lambda_1, lambda_2, *it );

      if ( (p.profit_1 >= delta.profit_1) && (p.profit_2 >= delta.profit_2)
	   && (p.lambda >= delta.lambda) )
	ub.insert(p - delta);
    }

  std::list<bi_profit> large;
  m_solution_sets.large_cut( large );

  std::list<bi_profit>::const_iterator it_large;

  for ( it_large=large.begin(); it_large!=large.end(); ++it_large )
    {
      bi_profit point = *it_large;

      if ( point.profit_1 == t.top_left().profit_1 )
	point.profit_1 -= 1;

      if ( point.profit_2 == t.bottom_right().profit_2 )
	point.profit_2 -= 1;

      combined_profit p( lambda_1, lambda_2, point );

      if ( (p.profit_1 >= delta.profit_1) && (p.profit_2 >= delta.profit_2)
	   && (p.lambda >= delta.lambda) )
	ub.insert(p - delta);
    }
} // base_second_phase::scale_known_solutions()
