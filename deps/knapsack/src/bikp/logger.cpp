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
 * \file logger.cpp
 * \brief Implementation of the tool::logger class.
 * \author Julien Jorge
 */
#include "bikp/tool/logger.hpp"

std::ofstream* tool::logger::m_file = NULL;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the logger, to print at the begining of each line.
 */
tool::logger::logger( const std::string& name )
  : m_name(name)
{

} // logger::logger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cast the logger to a std::ostream.
 */
tool::logger::operator std::ostream&()
{
  if (m_file)
    return *m_file;
  else
    return std::clog;
} // logger::operator std::ostream&()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the log of new informations.
 */
std::ostream& tool::logger::log()
{
  return *this << m_name << ": ";
} // logger::log()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the log of new informations.
 */
std::ostream& tool::logger::log() const
{
  return *const_cast<logger*>(this) << m_name << ": ";
} // logger::log()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the output file.
 * \param f The file in which writing.
 */
void tool::logger::output_file( std::ofstream& f )
{
  m_file = &f;
} // logger::output_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the output file.
 */
void tool::logger::release_file()
{
  m_file = NULL;
} // logger::release_file()
