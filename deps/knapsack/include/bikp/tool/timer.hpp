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
 * \file timer.hpp
 * \brief The timer class is made to make easy the measure of the running time
 *        of an algorithm.
 * \author Julien Jorge
 */
#ifndef __BIKP_TOOL_TIMER_HPP__
#define __BIKP_TOOL_TIMER_HPP__

#include <stack>
#include <iostream>
#include <string>
#include <sys/time.h>

#include "bikp/tool/logger.hpp"

namespace tool
{
  /**
   * \brief The timer class is made to make easy the measure of the running time
   *        of an algorithm.
   *
   * If you want, for example, to measure the running time of your new
   * <tt> upper_bound() </tt> algorithm. Just write :
   *
   * <tt> my_timer.start( "upper_bound" ); </tt> <br>
   * <tt> ub = upper_bound(); </tt> <br>
   * <tt> my_timer.stop(); </tt> <br>
   */
  class timer
  {
  private:
    /**
     * \brief Starting time reference.
     */
    class reference
    {
    public:
      reference( const std::string& comment );
      void output( logger& log );

    private:
      /** \brief The starting time reference. */
      timeval m_start_time;

      /** \brief A comment to add to the output. */
      std::string m_comment;
    }; // class reference

  public:
    timer();
    ~timer();
    void start( const std::string& comment );
    void stop();

  private:
    /** \brief All time references not closed yet. */
    std::stack<reference> m_time_references;

    /** \brief The logger in which we will output the results. */
    logger m_logger;
  }; // class timer
} // namespace tool

#endif // __BIKP_TOOL_TIMER_HPP__
