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
 * \file bound_set_relaxation.tpp
 * \brief Implementation of the bound_set_relaxation class.
 * \author Julien Jorge
 */
#include "bikp/relaxation/bound_set_relaxation.hpp"
#include "bikp/phase/dp_first_phase.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The combined problem.
 */
bound_set_relaxation::bound_set_relaxation( const bi_problem& p )
 : m_problem(p)
{

} // bound_set_relaxation::combined_relaxation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the relaxation is greater than a lower bound on each
 *        objective.
 * \param i The index of a variable to remove from the problem.
 * \param fixed The profit that we already have.
 * \param bound The bound to reach.
 * \param cap The remaining capacity of the problem.
 */
bool bound_set_relaxation::interesting
( unsigned int i, const bi_profit& fixed,
  const bound_set_type& lower_bound, real_type cap ) const
{
  bound_set_type ub = relax( i, fixed, cap );
  
  return lower_bound.is_dominated_neq(ub);
} // bound_set_relaxation::interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation on the combined objective.
 * \param i The index of a variable to remove from the problem.
 * \param fixed The profit that we already have.
 * \param cap The remaining capacity of the problem.
 */
bound_set_relaxation::bound_set_type
bound_set_relaxation::relax
( unsigned int i, const bi_profit& fixed, real_type cap ) const
{
  bound_set_type result;
  result.insert(fixed);
  
  if ( m_problem.size() == 1 )
    return result;

  std::vector<real_type> c1(m_problem.size());
  std::vector<real_type> c2(m_problem.size());
  std::vector<real_type> w(m_problem.size());

  for (unsigned int j=0; j!=m_problem.size(); ++j)
    {
      c1[j] = m_problem.profit(j).profit_1;
      c2[j] = m_problem.profit(j).profit_2;
      w[j] = m_problem.weight(j);
    }

  c1.erase( c1.begin() + i );
  c2.erase( c2.begin() + i );
  w.erase( w.begin() + i );

  bi_problem p( c1, c2, w, cap );
  dp_first_phase phase;
  
  phase.run(p);

  dp_first_phase::optimal_set_type::const_iterator it;

  for ( it=phase.get_extreme_solutions().begin();
	it!=phase.get_extreme_solutions().end(); ++it )
    result.insert( fixed + make_profit( it->objective_value().profit_1,
					it->objective_value().profit_2 ) );

  for ( it=phase.get_not_extreme_solutions().begin();
	it!=phase.get_not_extreme_solutions().end(); ++it )
    result.insert( fixed + make_profit( it->objective_value().profit_1,
					it->objective_value().profit_2 ) );

  return result;
} // bound_set_relaxation::relax()

bi_profit bound_set_relaxation::make_profit
( real_type p1, real_type p2 ) const
{
  return bi_profit( p1, p2 );
} // bound_set_relaxation::make_profit()
