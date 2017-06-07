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
 * \file variable_fixing_by_direction.tpp
 * \brief Implementation of the variable_fixing_by_direction class.
 * \author Julien Jorge
 */
#include "bikp/reduction/variable_fixing_by_direction.hpp"

#include "bikp/problem/combined_problem.hpp"
#include "bikp/reduction/simple_variable_fixing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param X_SE The set of supported efficient solutions.
 */
variable_fixing_by_direction::variable_fixing_by_direction
( const bi_problem& p, const std::list<bi_solution>& X_SE )
  : super(p)
{
  std::string pattern( p.size(), ' ' );
  make_pattern( pattern, X_SE );

  bound_set_type Z_SE;
  Z_SE.insert( X_SE.begin(), X_SE.end() );

  bound_set_type::const_iterator it = Z_SE.begin();
  bound_set_type::const_iterator it2 = it;

  for (++it2; it2!=Z_SE.end(); ++it, ++it2)
    {
      combined_problem mono_p
	( p, it->profit_2 - it2->profit_2, it2->profit_1 - it->profit_1 );

      // can be tightened
      const real_type lb_lambda =
	mono_p.get_lambda_1() * it->profit_1
	+ mono_p.get_lambda_2() * it2->profit_2;

      mono_p.sort_by_decreasing_efficiency();

      // do the reduction on the combined objective only
      mono_problem* one_objective = mono_p.convert_to_mono(0);

      simple_variable_fixing red( *one_objective, lb_lambda );
      delete one_objective;

      for (unsigned int i=0; i!=p.size(); ++i)
	if ( pattern[ mono_p.real_index(i) ] == '1' )
	  {
	    if ( red.set_variables().find(i) == red.set_variables().end() )
	      pattern[ mono_p.real_index(i) ] = '.';
	  }
	else if ( pattern[ mono_p.real_index(i) ] == '0' )
	  {
	    if ( red.unset_variables().find(i) == red.unset_variables().end() )
	      pattern[ mono_p.real_index(i) ] = '.';
	  }
    }

  for (unsigned int i=0; i!=p.size(); ++i)
    if ( pattern[i] == '1' )
      m_set_variables.insert(i);
    else if ( pattern[i] == '0' )
      m_unset_variables.insert(i);

  this->create_sub_problem();
} // variable_fixing_by_direction::variable_fixing_by_direction()

/*----------------------------------------------------------------------------*/
void variable_fixing_by_direction::make_pattern
( std::string& pattern, const std::list<bi_solution>& X_SE ) const
{
  std::list<bi_solution>::const_iterator it = X_SE.begin();
  unsigned int i;

  for (i=0; i!=pattern.size(); ++i)
    if ( it->is_set(i) )
      pattern[i] = '1';
    else
      pattern[i] = '0';

  for (it=X_SE.begin(); it!=X_SE.end(); ++it)
    for (i=0; i!=pattern.size(); ++i)
      if ( pattern[i] == '0' && it->is_set(i) )
	pattern[i] = '.';
      else if ( pattern[i] == '1' && it->is_unset(i) )
	pattern[i] = '.';
} // variable_fixing_by_direction::make_pattern()
