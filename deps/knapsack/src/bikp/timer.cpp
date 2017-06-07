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
 * \file timer.cpp
 * \brief Implementation of the tool::timer class.
 * \author Julien Jorge
 */
#include "bikp/tool/timer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param comment Comment to add to the output.
 */
tool::timer::reference::reference( const std::string& comment )
  : m_comment(comment)
{
  gettimeofday( &m_start_time, NULL );
} // timer::reference::reference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the comment and the time elapsed since creation.
 * \param log The logger in which writing.
 */
void tool::timer::reference::output( logger& log )
{
  timeval now;
  gettimeofday( &now, NULL );

  timeval diff;
  timersub( &now, &m_start_time, &diff );

  int ms = diff.tv_usec / 1000;
  log.log() << m_comment << ": " << diff.tv_sec << ".";

  if (ms < 100)
    {
      log << 0;
      if (ms < 10)
	log << 0;
    }

  log << ms << std::endl;
} // timer::reference::output()



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
tool::timer::timer()
  : m_logger("timer")
{

} // timer::timer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
tool::timer::~timer()
{
  if ( !m_time_references.empty() )
    {
      std::cerr << "Some timers has not been closed (see log)." << std::cerr;

      while( !m_time_references.empty() )
	stop();
    }
} // timer::~timer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start a new timer.
 * \param comment A comment to add to the output.
 */
void tool::timer::start( const std::string& comment )
{
  m_time_references.push( reference(comment) );
} // timer::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the last created timer.
 */
void tool::timer::stop()
{
  m_time_references.top().output( m_logger );
  m_time_references.pop();
} // timer::stop()
