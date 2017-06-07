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
 * \file logger.hpp
 * \brief A class to easily write log informations.
 * \author Julien Jorge
 */
#ifndef __BIKP_TOOL_LOGGER_HPP__
#define __BIKP_TOOL_LOGGER_HPP__

#include <fstream>
#include <iostream>
#include <string>

namespace tool
{
  /**
   * \brief A class to easily write log informations.
   * \author Julien Jorge
   */
  class logger
  {
  public:
    logger( const std::string& name );

    template<typename T>
    std::ostream& operator<<(const T& t);

    operator std::ostream&();

    std::ostream& log();
    std::ostream& log() const;

    static void output_file( std::ofstream& f );
    static void release_file();
  
  private:
    /** \brief If not NULL, we write informations in this file. Otherwise we
	write in std::clog. */
    static std::ofstream* m_file;

    /** \brief The "name" of the logger, printed at the begining of each 
	line. */
    const std::string m_name;

  }; // class logger
} // namespace tool

#include "impl/logger.tpp"

#endif // __BIKP_TOOL_LOGGER_HPP__
