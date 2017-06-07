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
 * \file base_first_phase.cpp
 * \brief Implementation of the base_first_phase class.
 * \author Julien Jorge
 */
#include "bikp/phase/base_first_phase.hpp"

#include "bikp/reduction/mono_variable_fixing.hpp"
#include "bikp/problem/transform.hpp"

#include <stack>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the algorithm to diplay in logs.
 */
base_first_phase::base_first_phase( const std::string& name )
  : m_logger(name + "_first_phase")
{

} // base_first_phase::base_first_phase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the algorithm.
 * \param p The problem to solve.
 */
void base_first_phase::run(const bi_problem& p)
{
  bi_solution x1(p); // best solution on the second objective
  bi_solution x2(p); // best solution on the first objective

  //m_logger.log() << "calculating x2" << std::endl;
  find_mono_optimal(p, 1, x2, x1);
  
  x1 = x2;

  //m_logger.log() << "calculating x1" << std::endl;
  find_mono_optimal(p, 2, x1, x2);

  /*m_logger.log() << "starting solutions x1, x2 = ("
		 << x1.objective_value().profit_1 << ", "
		 << x1.objective_value().profit_2 << ") ("
		 << x2.objective_value().profit_1 << ", "
		 << x2.objective_value().profit_2 << ")" << std::endl;*/

  assert( x1.is_full() );
  assert( x2.is_full() );

  m_extreme_solutions.insert(x1);
  m_extreme_solutions.insert(x2);

  if ( m_extreme_solutions.size() > 1 )
    {
      /*//m_logger.log() << "getting x1 and x2 pareto optimal." << std::endl;
	calculate_pareto_optimal(p, x1, x2);*/

      //m_logger.log() << "computing supported solutions." << std::endl;
      calculate_solutions(p, x1, x2);
    }
  else
    m_extreme_solutions.insert(x1);

#ifndef NDEBUG
  optimal_set_type::const_iterator it;

  for (it=m_extreme_solutions.begin(); it!=m_extreme_solutions.end(); ++it)
    assert( it->is_full() );
  for ( it=m_not_extreme_solutions.begin();
        it!=m_not_extreme_solutions.end();
        ++it )
    assert( it->is_full() );
#endif
} // base_first_phase::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the images of the supported solutions.
 * \param extreme_os Output stream for extrem solutions.
 * \param not_extreme_os Output stream for not extrem solutions.
 */
void base_first_phase::save
( std::ostream& extreme_os, std::ostream& not_extreme_os ) const
{
  optimal_set_type::const_iterator it;

  for (it=m_extreme_solutions.begin(); it!=m_extreme_solutions.end(); ++it)
    {
      extreme_os << it->objective_value().profit_1 << "\t"
		 << it->objective_value().profit_2 << "\t";

      for (unsigned int i=0; i!=it->size(); ++i)
	if ( it->is_set(i) )
	  extreme_os << "1";
	else
	  extreme_os << "0";

      extreme_os << std::endl;
    }

  for ( it=m_not_extreme_solutions.begin();
        it!=m_not_extreme_solutions.end();
        ++it )
    {
      not_extreme_os << it->objective_value().profit_1 << "\t"
		 << it->objective_value().profit_2 << "\t";

      for (unsigned int i=0; i!=it->size(); ++i)
	if ( it->is_set(i) )
	  not_extreme_os << "1";
	else
	  not_extreme_os << "0";

      not_extreme_os << std::endl;
    }
} // base_first_phase::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all solutions.
 */
void base_first_phase::clear()
{
  m_extreme_solutions.clear();
  m_not_extreme_solutions.clear();
} // base_first_phase::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get extrem solutions.
 */
const base_first_phase::optimal_set_type&
base_first_phase::get_extreme_solutions() const
{
  return m_extreme_solutions;
} // base_first_phase::get_extreme_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get not extrem solutions.
 */
const base_first_phase::optimal_set_type&
base_first_phase::get_not_extreme_solutions() const
{
  return m_not_extreme_solutions;
} // base_first_phase::get_not_extreme_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the solution which weakly dominates each optimal
 *        (mono-objective) solution.
 *
 * \param p The problem to solve.
 * \param x1 (in/out) Optimal solution on the second objective.
 * \param x2 (in/out) Optimal solution on the first objective.
 */
void base_first_phase::calculate_pareto_optimal
( const bi_problem& p, bi_solution& x1, bi_solution& x2 ) const
{
  assert( x1.objective_value().profit_1 < x2.objective_value().profit_1 );
  assert( x1.objective_value().profit_2 > x2.objective_value().profit_2 );

  // only for assert
#ifndef NDEBUG
  real_type z1 = x2.objective_value().profit_1;
  real_type z2 = x1.objective_value().profit_2;
#endif

  bi_profit utopian;

  utopian.profit_1 = x2.objective_value().profit_1 + 1;
  utopian.profit_2 = x1.objective_value().profit_2 - 1;
  calculate_lambda_unique( p, x1.objective_value(), utopian, x1 );

  utopian.profit_1 = x2.objective_value().profit_1 - 1;
  utopian.profit_2 = x1.objective_value().profit_2 + 1;
  calculate_lambda_unique( p, utopian, x2.objective_value(), x2 );

#ifndef NDEBUG
  if ( !(z2 == x1.objective_value().profit_2) )
    std::cout << z2 << " != " << x1.objective_value().profit_2 << std::endl;

  assert(z2 == x1.objective_value().profit_2);

  if ( !(z1 == x2.objective_value().profit_1) )
    std::cout << z1 << " != " << x2.objective_value().profit_1 << std::endl;

  assert(z1 == x2.objective_value().profit_1);
#endif
} // base_first_phase::calculate_pareto_optimal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate not extrem solutions in a triangle of the objective space.
 *
 * \param p The problem to solve.
 * \param x1 Top-left solution.
 * \param x2 Bottom-right solution.
 */
void base_first_phase::calculate_solutions
( const bi_problem& p, const bi_solution& x1, const bi_solution& x2 )
{
  assert( x1.objective_value().profit_1 < x2.objective_value().profit_1 );
  assert( x1.objective_value().profit_2 > x2.objective_value().profit_2 );

  typedef std::pair<bi_solution, bi_solution> pair_bi_sol;
  
  std::stack<pair_bi_sol> pending_solutions;
  pending_solutions.push( pair_bi_sol(x1, x2) );
  unsigned int max_depth = 1;
  unsigned int count = 0;

  optimal_set_type new_solutions;
  optimal_set_type::const_iterator it1; 
  optimal_set_type::const_iterator it2;

  while( !pending_solutions.empty() )
    {
      ++count;
      const unsigned int depth = pending_solutions.size();

      if (depth > max_depth)
	max_depth = depth;

      pair_bi_sol sols = pending_solutions.top();
      pending_solutions.pop();

      new_solutions.clear();
      calculate_triangle(p, sols.first, sols.second, new_solutions);

      if (new_solutions.size() > 0)
	if ( update_solutions(sols.first, sols.second, new_solutions) )
	  {
	    it1 = new_solutions.begin();
	    it2 = new_solutions.end();
	    --it2;

	    if ( it2->objective_value().profit_1
		 != sols.second.objective_value().profit_1 )
	      pending_solutions.push( pair_bi_sol(*it2, sols.second) );

	    if ( sols.first.objective_value().profit_2
		 != it1->objective_value().profit_2 )
	      pending_solutions.push( pair_bi_sol(sols.first, *it1) );
	  }
    }
} // base_first_phase::calculate_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate not extrem solutions in a triangle of the objective space.
 *
 * \param p Bi-objective problem to solve.
 * \param x1 Top-left solution.
 * \param x2 Bottom-right solution.
 * \param new_solutions (in/out) The calculated solutions.
 */
void base_first_phase::calculate_triangle
( const bi_problem& p, const bi_solution& x1, const bi_solution& x2,
  optimal_set_type& new_solutions ) const
{
  assert( x1.objective_value().profit_1 < x2.objective_value().profit_1 );
  assert( x1.objective_value().profit_2 > x2.objective_value().profit_2 );

  new_solutions.insert(x1);
  //new_solutions.insert(x2);
  calculate_lambda_multiple( p, x1, x2, new_solutions );
} // base_first_phase::calculate_triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate one optimal solution of a linear combination of a
 *        bi-objective problem.
 *
 * \param p Bi-objective problem to solve.
 * \param z1 Value of the top-left solution.
 * \param z2 Value of the bottom-right solution.
 * \param sol (in/out) The calculated solution.
 */
void base_first_phase::calculate_lambda_unique
( const bi_problem& p, const bi_profit& z1, const bi_profit& z2,
  bi_solution& sol ) const
{
  assert( sol.is_feasible() );

  const real_type lambda_1 = z1.profit_2 - z2.profit_2;
  const real_type lambda_2 = z2.profit_1 - z1.profit_1;

  combined_problem mono_p(p, lambda_1, lambda_2);
  combined_solution mono_s(mono_p);

  mono_p.sort_by_decreasing_efficiency();

  mono_p.create_solution( sol, mono_s );

  find_unique( mono_p, mono_s );
  
  mono_p.restore_solution( mono_s, sol );
} // base_first_phase::calculate_lambda_unique()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the set of solutions of a linear combination of a
 *        bi-objective problem.
 *
 * \param p Bi-objective problem to solve.
 * \param x1 Top-left solution
 * \param x2 bottom right solution.
 * \param new_solutions (in/out) (in) A lower bound on solutions, (out) the
 *        calculated solutions.
 */
void base_first_phase::calculate_lambda_multiple
( const bi_problem& p, const bi_solution& x1, const bi_solution& x2,
  optimal_set_type& new_solutions ) const
{
  real_type lambda_1 =
    x1.objective_value().profit_2 - x2.objective_value().profit_2;
  real_type lambda_2 =
    x2.objective_value().profit_1 - x1.objective_value().profit_1;

  combined_problem mono_p(p, lambda_1, lambda_2);
  combined_optimal_set_type sols;

  mono_p.sort_by_decreasing_efficiency();

  transform::create_solutions(mono_p, new_solutions, sols);

  find_multiple( mono_p, sols );

#ifndef NDEBUG
  combined_optimal_set_type::const_iterator it;

  const real_type min_lambda = lambda_1 * x1.objective_value().profit_1
    + lambda_2 * x1.objective_value().profit_2;
  const real_type min_lambda_check = lambda_1 * x2.objective_value().profit_1
    + lambda_2 * x2.objective_value().profit_2;

  if (min_lambda != min_lambda_check)
    {
      std::cerr
	<< "Linear combination of the objectives gives different values: (x1)"
	<< min_lambda << " != " << min_lambda_check << " (x2)" << std::endl;
      assert(false);
    }

  for (it=sols.begin(); it!=sols.end(); ++it)
    {
      const real_type lambda_val = lambda_1 * it->objective_value().profit_1
	+ lambda_2 * it->objective_value().profit_2;

      /*std::cout << lambda_1 << " * " << it->objective_value().profit_1
		<< " + " << lambda_2 << " * " << it->objective_value().profit_2
		<< " = " << lambda_val << std::endl;*/

      if( lambda_val < min_lambda )
	{
	  std::cerr << "solution should not have been found: " << lambda_val
		    << " < " << min_lambda << std::endl;
	  assert(false);
	}
    }
#endif

  new_solutions.clear();
  transform::restore_solutions(mono_p, p, sols, new_solutions);
} // base_first_phase::calculate_lambda_multiple()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate an optimal solution on a mono criteria problem.
 *
 * \param p The problem to solve.
 * \param obj The index of the profit vector to keep.
 * \param sol (in) The best known solution, (out) the calculated solution.
 * \param low Lower bound of the solution to find.
 */
void base_first_phase::find_mono_optimal
( const bi_problem& p, unsigned int obj, bi_solution& sol,
  const bi_solution& low ) const
{
  assert( sol.is_feasible() );
  assert( low.is_feasible() );

  mono_problem* mono_p = p.convert_to_mono(obj);
  mono_solution mono_s(*mono_p);

  mono_p->sort_by_decreasing_efficiency();

  mono_p->create_solution( low, mono_s );

  mtr_variable_fixing reduction(*mono_p, mono_s);

  const mono_problem& sub_p = reduction.get_sub_problem();

  if ( sub_p.size() > 0 )
    {
      mono_solution sub_s(sub_p);
      real_type lb =
	mono_s.objective_value() - reduction.guaranteed_profit();

      solve(sub_p, sub_s, lb);

      if ( mono_profit_traits::better
	   ( sub_s.objective_value() + reduction.guaranteed_profit(),
	     mono_s.objective_value() ) )
	reduction.restore_solution(sub_s, mono_s);
    }

  assert( mono_s.is_full() );
  assert( mono_s.is_feasible() );

  mono_p->restore_solution( mono_s, sol );
  delete mono_p;
} // base_first_phase::find_mono_optimal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate one optimal solution of a linear combination of a
 *        bi-objective problem.
 *
 * \param mono_p Combined problem to solve.
 * \param sol (in/out) The calculated solution.
 */
void base_first_phase::find_unique
( const combined_problem& mono_p, combined_solution& sol ) const
{
  assert( sol.is_feasible() );

  mono_variable_fixing reduction( mono_p, sol );
  const combined_problem& sub_p = reduction.get_sub_problem();

  if ( sub_p.size() > 0 )
    {
      combined_solution sub_s(sub_p);
      combined_profit lb =
	sol.objective_value() - reduction.guaranteed_profit();

      solve(sub_p, sub_s, lb);

      if ( combined_profit_traits::better
	   ( sub_s.objective_value() + reduction.guaranteed_profit(),
	     sol.objective_value() ) )
	  reduction.restore_solution(sub_s, sol);
    }
} // base_first_phase::find_unique()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the set of solutions by a branch and bound on a linear
 *        combination of a bi-objective problem.
 *
 * \param mono_p Combined problem to solve.
 * \param new_solutions (in/out) The calculated solutions.
 */
void base_first_phase::find_multiple
( const combined_problem& mono_p,
  combined_optimal_set_type& new_solutions ) const
{
  assert(new_solutions.size() > 0);

  mono_variable_fixing reduction( mono_p, new_solutions, true );
  const combined_problem& sub_p = reduction.get_sub_problem();

  combined_optimal_set_type tmp_sols(new_solutions);
  new_solutions.clear();

  real_type max_profit = 0;
  combined_optimal_set_type::const_iterator it;

  // due to dominance on bi objective profits, tmp_sols can contain solutions
  // that are not optimal on the combined objective
  for (it=tmp_sols.begin(); it!=tmp_sols.end(); ++it)
    if ( (real_type)it->objective_value() > max_profit )
      max_profit = it->objective_value();

  if ( sub_p.size() > 0 )
    {
      combined_profit lb;
      lb.profit_1 = lb.profit_2 = 0;
      lb.lambda = max_profit - (real_type)reduction.guaranteed_profit();

      combined_optimal_set_type sols;

      solve(sub_p, sols, lb);

      combined_optimal_set_type::const_iterator it;

      for (it=sols.begin(); it!=sols.end(); ++it)
	{
	  combined_solution s(mono_p);
	  reduction.restore_solution( *it, s );
	  tmp_sols.insert(s);
	}
    }
  
  for (it=tmp_sols.begin(); it!=tmp_sols.end(); ++it)
    if ( (real_type)it->objective_value() > max_profit )
      max_profit = it->objective_value();

  for (it=tmp_sols.begin(); it!=tmp_sols.end(); ++it)
    if ( (real_type)it->objective_value() == max_profit )
      new_solutions.insert(*it);
} // base_first_phase::find_multiple()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the sets of solutions.
 *
 * \param x1 Top-left solution.
 * \param x2 Bottom-right solution.
 * \param new_solutions The solutions to add.
 *
 * \return True if the solutions are better in term of lambda combination.
 */
bool base_first_phase::update_solutions
( const bi_solution& x1, const bi_solution& x2,
  const optimal_set_type& new_solutions ) 
{
  assert(new_solutions.size() > 0);

  bool ok = false;
  optimal_set_type::const_iterator it1; 
  optimal_set_type::const_iterator it2;

#ifndef NDEBUG
  for (it1=new_solutions.begin(); it1!=new_solutions.end(); ++it1)
    {
      //std::cout << it1->objective_value() << std::endl;
      assert(it1->is_feasible());
      assert(it1->is_full());
      assert(it1->objective_value().profit_1 >= x1.objective_value().profit_1);
      assert(it1->objective_value().profit_1 <= x2.objective_value().profit_1);
      assert(it1->objective_value().profit_2 <= x1.objective_value().profit_2);
      assert(it1->objective_value().profit_2 >= x2.objective_value().profit_2);
    }
#endif

  it1 = new_solutions.begin();
  it2 = new_solutions.end();
  --it2;

  if ( (it1->objective_value() != x1.objective_value())
       && (it1->objective_value() != x2.objective_value()) )
    {
      // the first and the last solutions go in extreme_solutions, the others
      // are not exrem.
      m_extreme_solutions.insert(*it1);
      m_extreme_solutions.insert(*it2);
      ok = true;
    }

  if ( new_solutions.size() > 2)
    {
      ++it1;

      m_not_extreme_solutions.insert(it1, it2);
    }

  return ok;
} // base_first_phase::update_solutions()

