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
 * \file logger.tpp
 * \brief Implementation of the template methods of the tool::logger class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a value in a stream.
 * \param t The value to write.
 */
template<typename T>
std::ostream& tool::logger::operator<<(const T& t)
{
  if (m_file)
    *m_file << t;
  else
    std::clog << t;

  return *this;
} // logger::operator<<()
