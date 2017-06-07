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
 * \file bi_problem.cpp
 * \brief Implementation of the bi_problem class.
 * \author Julien Jorge
 */
#include "bikp/problem/bi_problem.hpp"

#include <cassert>
#include <stdexcept>
#include "bikp/tool/algorithm.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The stream on which we read the input data.
 * \remark The stream must respect the MCDMlib format, without comments.
 */
bi_problem::bi_problem( std::istream& f )
{
  unsigned int size, nb_objectives, nb_constraints;

  f >> size;
  f >> nb_objectives;
  f >> nb_constraints;

  assert(nb_objectives == 2);
  assert(nb_constraints == 1);

  m_profit.resize(size);
  m_weight.resize(size);
  m_efficiency_1.resize(size);
  m_efficiency_2.resize(size);

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    f >> m_profit[i].profit_1;

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    f >> m_profit[i].profit_2;

  for (unsigned int i=0; i!=m_weight.size(); ++i)
    f >> m_weight[i];

  f >> m_capacity;
  
  if ( !f )
    {
      std::cerr << "error while reading the instance." << std::endl;
      throw std::runtime_error("error while reading the instance.");
    }

  compute_efficiencies();
} // probleme_bi::probleme_bi()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param vp vopt problem.
 * \remark problem must have 2 objectives and be unidimensional.
 */
bi_problem::bi_problem( int nb_obj, int sz, int *obj1, int *obj2, int *weigths ,int capacity  )
{
  unsigned int size;

  size = sz;

  m_profit.resize(size);
  m_weight.resize(size);
  m_efficiency_1.resize(size);
  m_efficiency_2.resize(size);

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    m_profit[i].profit_1 = obj1[i];

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    m_profit[i].profit_2 = obj2[i];

  for (unsigned int i=0; i!=m_weight.size(); ++i)
    m_weight[i] = weigths[i];

  m_capacity = capacity;

  compute_efficiencies();
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c1 Cost of the variables on the first objective.
 * \param c2 Cost of the variables on the second objective.
 * \param w Weight of the variables.
 * \param cap Capacity of the knapsack.
 */
bi_problem::bi_problem
( const std::vector<real_type>& c1, const std::vector<real_type>& c2,
  const std::vector<real_type>& w, real_type cap )
{
  assert( cap >= 0 );

  m_capacity = cap;
  m_profit.resize(c1.size());
  m_weight.resize(c2.size());
  
  for (unsigned int i=0; i!=c1.size(); ++i)
    {
      m_profit[i].profit_1 = c1[i];
      m_profit[i].profit_2 = c2[i];
      m_weight[i] = w[i];
    }
    
  compute_efficiencies();
} // bi_problem::bi_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the problem.
 * \param cap The capacity of the knapsack.
 */
bi_problem::bi_problem( unsigned int n, const weight_type cap )
  : m_profit(n), m_weight(n), m_capacity(cap), m_efficiency_1(n),
    m_efficiency_2(n)
{

} // bi_problem::bi_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the instance.
 * \param f The file in which we save the instance.
 */
void bi_problem::save( std::ostream& f ) const
{
  f << size() << '\n';
  f << 2 << '\n';
  f << 1 << '\n';

  f << "\n# profits 1\n";

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    f << m_profit[i].profit_1 << '\n';

  f << "\n# profits 2\n";

  for (unsigned int i=0; i!=m_profit.size(); ++i)
    f << m_profit[i].profit_2 << '\n';

  f << "\n# weights\n";

  for (unsigned int i=0; i!=m_weight.size(); ++i)
    f << m_weight[i] << '\n';

  f << "\n# capacity\n";
  f << m_capacity << std::endl;
} // bi_problem::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the profit & weigth of a variable.
 * \param i The index of the variable.
 * \param p The profit of the variable.
 * \param w The weight of the variable.
 */
void bi_problem::set_variable
( unsigned int i, const profit_type& p, weight_type w )
{
  assert( i < size() );

  m_profit[i] = p;
  m_weight[i] = w;
  m_efficiency_1[i] = (real_type)p.profit_1 / (real_type)w;
  m_efficiency_2[i] = (real_type)p.profit_2 / (real_type)w;
} // bi_problem::set_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the problem into a mono objective problem.
 * \param obj The objective to keep.
 */
mono_problem* bi_problem::convert_to_mono( unsigned int obj ) const
{
  mono_problem* p = new mono_problem( size(), capacity() );

  switch(obj)
    {
    case 1:
      for (unsigned int i=0; i!=size(); ++i)
	p->set_variable( i, m_profit[i].profit_1, m_weight[i] );
      break;
    case 2:
      for (unsigned int i=0; i!=size(); ++i)
	p->set_variable( i, m_profit[i].profit_2, m_weight[i] );
    }

  return p;
} // bi_problem::convert_to_mono()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of variables in the problem.
 */
unsigned int bi_problem::size() const
{
  assert( m_profit.size() == m_weight.size() );

  return m_profit.size();
} // bi_problem::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum capacity.
 */
bi_problem::weight_type bi_problem::capacity() const
{
  return m_capacity;
} // bi_problem::capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the efficiencies of the variables on each objective.
 */
void bi_problem::compute_efficiencies()
{
  m_efficiency_1.resize( size() );
  m_efficiency_2.resize( size() );
  
  for (unsigned int i=0; i!=size(); ++i)
    {
      m_efficiency_1[i] =
	(real_type)m_profit[i].profit_1 / (real_type)m_weight[i];
      m_efficiency_2[i] =
	(real_type)m_profit[i].profit_2 / (real_type)m_weight[i];
    }
} // bi_problem::compute_efficiencies()
