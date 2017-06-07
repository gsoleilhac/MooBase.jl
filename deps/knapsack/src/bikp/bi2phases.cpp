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
 * \file main.cpp
 * \brief The main procedure.
 */
#include <fstream>
#include <iostream>

#include "bikp/problem/bi_problem.hpp"
#include "bikp/solver/bi_two_phases.hpp"
#include "bikp/bi2phases.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Program entry point.
 * \param argc Arguments count.
 * \param argv Program's arguments.
 */
extern "C" 
void bi2phases( int nb_obj, int sz, int *obj1, int *obj2, int *weigths ,int capacity ,int * &z1, int * &z2, int * &solW, bool * &res, int &nbSol)
{
  bi_problem p(nb_obj, sz, obj1, obj2, weigths ,capacity);
  bi_two_phases solver;
  std::list<bi_solution> X_E;
  
  solver.run(p, X_E);

  X_E.sort();

  nbSol = X_E.size();

  z1 = new int[nbSol];
  z2 = new int[nbSol];
  solW = new int[nbSol];
  res = new bool[nbSol*sz];

  int cptSol = 0;
  for ( ; !X_E.empty() ; X_E.pop_front() )
  {
    const bi_solution& s(X_E.front());

    z1[cptSol] = (int)s.objective_value().profit_1;
    z2[cptSol] = (int)s.objective_value().profit_2;
    solW[cptSol] = (int)s.weight();

    for (std::size_t i=0; i!=s.size(); ++i){
      res[cptSol*sz + i] = s.is_set(i);
    }

    cptSol++;
  }




}