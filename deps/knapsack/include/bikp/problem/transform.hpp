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
 * \file transform.hpp
 * \brief A class that implements methods for converting solutions between two
 *        problems.
 * \author Julien Jorge
 */
#ifndef __BIKP_TRANSFORM_HPP__
#define __BIKP_TRANSFORM_HPP__

#include <list>
#include <algorithm>

/**
 * \brief A class that implements methods for converting solutions between two
 *        problems.
 * \author Julien Jorge
 */
class transform
{
public:
  template<class Problem, class Input, class Output>
  static void create_solutions
  ( const Problem& p, const Input& in_sols, Output& out_sols )
  {
    typename Input::const_iterator it;

    for (it=in_sols.begin(); it!=in_sols.end(); ++it)
      {
        typename Output::value_type sol(p);    // the resulting solution
        p.create_solution( *it, sol );
        out_sols.insert(sol);
      }
  }

  template<class Problem, class Input, class S>
  static void create_solutions
  ( const Problem& p, const Input& in_sols, std::list<S>& out_sols )
  {
    typename Input::const_iterator it;
 
    for (it=in_sols.begin(); it!=in_sols.end(); ++it)
      {
	S sol(p);    // the resulting solution
	p.create_solution( *it, sol );
	out_sols.push_back(sol);
      }
  }

  template<class InProblem, class OutProblem, class Input, class Output>
  static void restore_solutions
  ( const InProblem in_p, const OutProblem& out_p,
    const Input& in_sols, Output& out_sols )
  {
    typename Input::const_iterator it;

    for (it=in_sols.begin(); it!=in_sols.end(); ++it)
      {
        typename Output::value_type sol(out_p);    // the resulting solution
        in_p.restore_solution( *it, sol );
        out_sols.insert(sol);
      }
  }

  template<class InProblem, class OutProblem, class Input, class S>
  static void restore_solutions
  ( const InProblem in_p, const OutProblem& out_p,
    const Input& in_sols, std::list<S>& out_sols )
  {
    typename Input::const_iterator it;
 
    for (it=in_sols.begin(); it!=in_sols.end(); ++it)
      {
        S sol(out_p);    // the resulting solution
        in_p.restore_solution( *it, sol );
        out_sols.push_back(sol);
      }
  }

  template<class Problem>
  static unsigned int minimum_cardinality( const Problem& p )
  {
    std::vector<real_type> w(p.size());

    for (unsigned int i=0; i!=p.size(); ++i)
      w[i] = p.weight(i);

    std::sort( w.begin(), w.end(), std::greater<real_type>() );

    real_type cap = p.capacity();
    bool stop = false;
    unsigned int card = 0;

    for (unsigned int i=0; (i!=w.size()) && !stop; ++i)
      if ( w[i] <= cap )
        {
          ++card;
          cap -= w[i];
        }
      else
        stop = true;

    return card;
  } // minimum_cardinality()
}; // class transform

#endif // __BIKP_TRANSFORM_HPP__
