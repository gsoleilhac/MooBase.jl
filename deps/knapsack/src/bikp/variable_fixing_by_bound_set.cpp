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
 * \file variable_fixing_by_bound_set.tpp
 * \brief Implementation of the variable_fixing_by_bound_set class.
 * \author Julien Jorge
 */
#include "bikp/reduction/variable_fixing_by_bound_set.hpp"
#include "bikp/relaxation/bound_set_relaxation.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param X_SE The set of supported efficient solutions.
 */
variable_fixing_by_bound_set::variable_fixing_by_bound_set
( const bi_problem& p, const std::list<bi_solution>& X_SE )
  : super(p)
{
  bi_profit zero;
  zero *= 0;

  bound_set_type lower_bound;
  bound_set_relaxation relaxation(p);
  real_type cap = p.capacity();
  std::vector<bound_set_type> front_zero(p.size());
  std::vector<bound_set_type> front_one(p.size());

  std::string pattern( p.size(), ' ' );
  make_pattern( pattern, X_SE );

  std::cout << "pattern bound in: " << pattern << std::endl;

  lower_bound.insert( X_SE.begin(), X_SE.end() );
  bound_set_type lower_b( lower_bound );

  bound_set_type lb_nadir;
  build_nadir( lower_b, lb_nadir );

  unsigned int nb_0(0), nb_1(0), rl_1(0);

  std::cout << "relaxing " << std::flush;

  for (unsigned int i=0; i!=p.size(); ++i)
    if ( pattern[i] == '0' )
      {
	++nb_0;
	std::cout << " x_" << i << " = 1," << std::flush;
	front_one[i] = relaxation.relax( i, p.profit(i), cap - p.weight(i) );
	lower_b.insert( front_one[i].begin(), front_one[i].end() );

	/*std::ostringstream oss;
	oss  << "points_x" << i << "=1";
	std::ofstream f( oss.str().c_str() );
	bound_set_type::const_iterator it;

	for ( it=front_one[i].begin(); it!=front_one[i].end(); ++it )
	  f << it->profit_1 << " " << it->profit_2 << std::endl;

	f.close();*/
      }
    else if ( pattern[i] == '1' )
      {
	++nb_1;
	
	bool do_the_relax = true;

	for (unsigned int j=0; (j!=p.size()) && do_the_relax; ++j)
	  if ( pattern[j] != '1' )
	    {
	      bound_set_type shifted_lb;
	      std::list<bi_solution>::const_iterator it;

	      for (it=X_SE.begin(); it!=X_SE.end(); ++it)
		if ( !it->is_set(j) )
		  if (p.capacity() - it->weight() + p.weight(i) >= p.weight(j))
		    shifted_lb.insert
		      ( it->objective_value() + p.profit(j) - p.profit(i) );

	      if ( !shifted_lb.empty() )
		do_the_relax = is_conv_dominated(shifted_lb, lb_nadir);
	    }

	if ( do_the_relax )
	  {
	    ++rl_1;
	    std::cout << " x_" << i << " = 0," << std::flush;
	    front_zero[i] = relaxation.relax( i, zero, cap );
	    lower_b.insert( front_zero[i].begin(), front_zero[i].end() );

	    /*std::ostringstream oss;
	    oss  << "points_x" << i << "=0";
	    std::ofstream f( oss.str().c_str() );
	    bound_set_type::const_iterator it;
	    
	    for ( it=front_zero[i].begin(); it!=front_zero[i].end(); ++it )
	      f << it->profit_1 << " " << it->profit_2 << std::endl;

	    f.close();*/
	  }
      }

  std::cout << std::endl;
  std::cout << nb_0 + nb_1 << " vars to relax (" << nb_0 << "x0, " << nb_1
	    << "x1).\n" << rl_1 << " supported sets computed for x=1\n"
	    << rl_1 + nb_0 << " supported sets computed." << std::endl;

  lb_nadir.clear();
  build_nadir( lower_b, lb_nadir );
  
 /* std::ofstream f( "lb" );
  bound_set_type::const_iterator it;
	    
  for ( it=lower_b.begin(); it!=lower_b.end(); ++it )
    f << it->profit_1 << " " << it->profit_2 << std::endl;

  f.close();

  f.open( "nadirs" );
	    
  for ( it=lb_nadir.begin(); it!=lb_nadir.end(); ++it )
    f << it->profit_1 << " " << it->profit_2 << std::endl;

  f.close();*/

  std::cout << "pattern bound out: ";

  for (unsigned int i=0; i!=p.size(); ++i)
    {
      if ( lower_bound.is_dominated_neq( front_zero[i] )
	   && is_conv_dominated( front_zero[i], lb_nadir ) )
	{
	  this->m_set_variables.insert(i);
	  std::cout << "1";
	}
      else if ( lower_bound.is_dominated_neq( front_one[i] )
		&& is_conv_dominated( front_one[i], lb_nadir ) )
	{
	  this->m_unset_variables.insert(i);
	  std::cout << "0";
	}
      else
	std::cout << ".";
    }
  std::cout << std::endl;

  this->create_sub_problem();
} // variable_fixing_by_bound_set::variable_fixing_by_bound_set()

/*----------------------------------------------------------------------------*/
void variable_fixing_by_bound_set::make_pattern
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
} // base_second_phase::make_pattern()

/*----------------------------------------------------------------------------*/
void variable_fixing_by_bound_set::build_nadir
( const bound_set_type& set, bound_set_type& out ) const
{
  bound_set_type::const_iterator it=set.begin();
  bound_set_type::const_iterator it2=it;

  ++it2;

  for ( ; it2!=set.end(); ++it, ++it2 )
    out.insert( make_profit( it->profit_1, it2->profit_2 ) );
} // variable_fixing_by_bound_set::build_nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all points of a set (\a lb) are "above" the lines made by each
 *        successive pair of points of an other set (\a ub).
 */
bool variable_fixing_by_bound_set::is_conv_dominated
( const bound_set_type& ub, const bound_set_type& lb ) const
{
  bool stop = false;
  bound_set_type::const_iterator it=ub.begin();
  bound_set_type::const_iterator it2=it;
  bound_set_type::const_iterator lit;

  ++it2;

  for ( ; (it2!=ub.end()) && !stop; ++it, ++it2 )
    {
      real_type coeff_1 = it->profit_2 - it2->profit_2;
      real_type coeff_2 = it2->profit_1 - it->profit_1;
      real_type bnd =
	it->profit_1 * coeff_1 + it->profit_2 * coeff_2;
      bool skip = false;

      for ( lit=lb.begin(); (lit!=lb.end()) && !stop && !skip; ++lit )
	if ((lit->profit_1 >= it->profit_1) && (lit->profit_1 <= it2->profit_1))
	  {
	    real_type ev =
	      lit->profit_1 * coeff_1 + lit->profit_2 * coeff_2;
	    
	    stop = ev < bnd;
	  }
	else if ( lit->profit_1 > it2->profit_1 )
	  skip = true;
    }

  return !stop;
} // variable_fixing_by_bound_set::is_conv_dominated()

bi_profit variable_fixing_by_bound_set::make_profit
( real_type p1, real_type p2 ) const
{
  return bi_profit( p1, p2 );
} // variable_fixing_by_bound_set::make_profit()
