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
 * \file extern_variable_fixing.tpp
 * \brief Implementation of the extern_variable_fixing class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param file_name The name of the file in which we read the variable values.
 */
template<class Problem>
extern_variable_fixing<Problem>::extern_variable_fixing
( const problem_type& p, const std::string& file_name )
  : super(p)
{
  std::ifstream f( file_name.c_str() );
  std::string pattern;

  f >> pattern;

  for (unsigned int i=0; i!=p.size(); ++i)
    if ( pattern[i] == '1' )
      this->m_set_variables.insert(i);
    else if ( pattern[i] == '0' )
      this->m_unset_variables.insert(i);
  
  this->create_sub_problem();
} // extern_variable_fixing::extern_variable_fixing()
