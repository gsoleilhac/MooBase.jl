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
 * \file pareto_set.hpp
 * \brief A set of non-dominated points.
 * \author Julien Jorge
 */
#ifndef __BIKP_PARETO_SET_HPP__
#define __BIKP_PARETO_SET_HPP__

#include "bikp/concept/profit_traits.hpp"

#include "bikp/concept/bi_minimum_complete_set.hpp"
#include "bikp/concept/mono_minimum_complete_set.hpp"

/**
 * \brief A set of non-dominated points
 *
 * \b Template \b arguments :
 * - \a Point : The type of the stored points.
 * - \a PointTraits : The type of the traits on the points.
 *
 * \author Julien Jorge
 */
template <class Point, class PointTraits = profit_traits<Point> >
class pareto_set :
  public bi_minimum_complete_set<Point, PointTraits>
{

}; // pareto_set

/**
 * \brief A set of not dominated points, specialized for combined_profit.
 * \author Julien Jorge
 */
/*template<>
class pareto_set< combined_profit, profit_traits<combined_profit> > :
  public bi_minimum_complete_set< combined_profit,
                                  profit_traits<combined_profit> >
{

};*/ // class pareto_set [combined_profit]

/**
 * \brief A set of not dominated points, specialized for real_type.
 * \author Julien Jorge
 */
template<>
class pareto_set< real_type, profit_traits<real_type> > :
  public mono_minimum_complete_set< real_type, profit_traits<real_type> >
{

}; // class pareto_set [real_type]

#endif // __BIKP_PARETO_SET_HPP__
