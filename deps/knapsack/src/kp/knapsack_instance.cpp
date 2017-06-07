/**
 * \file knapsack_instance.cpp
 * \brief Implementation of the kp::knapsack_instance class.
 * \author Julien Jorge
 */
#include "kp/knapsack_instance.hpp"
#include "kp/knapsack_solution.hpp"

#include <algorithm>
#include <stdexcept>


/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f A stream from which the datas of the instance are read.
 */
kp::knapsack_instance::knapsack_instance( std::istream& f )
{
  unsigned int size, constraints;

  f >> size >> m_nb_objectives >> constraints;

  std::vector<cost_vector> cost( size, cost_vector(m_nb_objectives) );
  std::vector<moo::real_type> weight( size );

  for( unsigned int i=0; i!=m_nb_objectives; ++i )
    for (unsigned int j=0; j!=size; ++j )
      f >> cost[j][i];

  for (unsigned int i=0; i!=size; ++i )
    f >> weight[i];

  f >> m_capacity;

  if ( !f )
    {
      std::cerr << "error while reading the instance." << std::endl;
      throw std::runtime_error("error while reading the instance.");
    }

  m_variables.reserve(size);

  for (unsigned int i=0; i!=size; ++i)
    m_variables.push_back( variable(i, cost[i], weight[i]) );
} // knapsack_instance::knapsack_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param vp vopt problem.
 */
kp::knapsack_instance::knapsack_instance( int nb_obj, int sz, float *obj, float *weigths ,float capacity )
{
  unsigned int size;

  size = sz;
  m_nb_objectives = nb_obj;

  std::vector<cost_vector> cost( size, cost_vector(m_nb_objectives) );
  std::vector<moo::real_type> weight( size );

  for( unsigned int i=0; i!=m_nb_objectives; ++i )
    for (unsigned int j=0; j!=size; ++j )
      cost[j][i] = obj[i*sz + j];

  for (unsigned int i=0; i!=size; ++i )
    weight[i] = weigths[i];


  m_capacity = capacity;
  m_variables.reserve(size);

  for (unsigned int i=0; i!=size; ++i)
    m_variables.push_back( variable(i, cost[i], weight[i]) );

} // knapsack_instance::knapsack_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The number of variables.
 * \param p The number of objectives.
 * \param w The capacity.
 */
kp::knapsack_instance::knapsack_instance
( unsigned int n, unsigned int p, moo::real_type c )
  : m_capacity(c), m_nb_objectives(p)
{
  m_variables.reserve(n);

  for (unsigned int i=0; i!=n; ++i)
    m_variables.push_back( variable(i, cost_vector(p), 0) );
} // knapsack_instance::knapsack_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variable (starting from zero).
 */
kp::variable& kp::knapsack_instance::get_variable( unsigned int i )
{
  return m_variables[i];
} // knapsack_instance::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variable (starting from zero).
 */
const kp::variable& kp::knapsack_instance::get_variable( unsigned int i ) const
{
  return m_variables[i];
} // knapsack_instance::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the capacity of the knapsack.
 */
moo::real_type kp::knapsack_instance::get_capacity() const
{
  return m_capacity;
} // knapsack_instance::get_capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of variables in the problem.
 */
unsigned int kp::knapsack_instance::get_size() const
{
  return m_variables.size();
} // knapsack_instance::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of objectives of the problem.
 */
unsigned int kp::knapsack_instance::get_nb_objectives() const
{
  return m_nb_objectives;
} // knapsack_instance::get_nb_objectives()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the capacity of the knapsack.
 * \param c The new capacity.
 */
void kp::knapsack_instance::set_capacity( moo::real_type c )
{
  m_capacity = c;
} // knapsack_instance::set_capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the knapsack instance.
 * \param os The stream in which we write the instance.
 * \remark The result uses the format of the MCDM library.
 */
void kp::knapsack_instance::save( std::ostream& os ) const
{
  os << "# N\n" << get_size() << "\n\n"
     << "# P\n" << get_nb_objectives() << "\n\n"
     << "# K\n1\n\n";

  for (unsigned int j=0; j!=get_nb_objectives(); ++j)
    {
      os << "# Objective " << (j+1) << '\n';

      for (unsigned int i=0; i!=get_size(); ++i)
	os << m_variables[i].cost[j] << '\n';

      os << '\n';
    }

  os << "# Constraint 1\n";

  for (unsigned int i=0; i!=get_size(); ++i)
    os << m_variables[i].weight << '\n';
  
  os << "\nCapacity\n" << get_capacity() << '\n';
} // knapsack_instance::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a solution is feasible and its values correct.
 * \param sol The solution to check.
 */
bool kp::knapsack_instance::check( const knapsack_solution& sol ) const
{
  bool result = true;
  solution_value value( get_nb_objectives() );

  for ( unsigned int i=0; i!=get_size(); ++i )
    if ( sol.status_of( get_variable(i) ) == moo::bin_set )
      {
        value.cost += get_variable(i).cost;
        value.weight += get_variable(i).weight;
      }

  if ( sol.value() != value )
    {
      result = false;
      std::cerr << "Solution value is incorrect: " << sol.value() << " != "
                << value << std::endl;
    }

  if ( sol.value().weight > get_capacity() )
    {
      result = false;
      std::cerr << "Solution is not feasible: " << sol.value().weight << " > "
                << value << std::endl;
    }

  return result;
} // knapsack_instance::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the minimum number of variables in the efficient solutions.
 */
unsigned int kp::knapsack_instance::card_lb() const
{
  std::vector<moo::real_type> w( m_variables.size() );
  std::transform( m_variables.begin(), m_variables.end(), w.begin(),
		  variable::weight_func() );
  std::sort( w.begin(), w.end(), std::greater<moo::real_type>() );

  moo::real_type sum = 0;
  unsigned int i = 0;

  do
    {
      sum += w[i];
      ++i;
    }
  while ( (i!=w.size()) && (sum <= m_capacity) );

  return i-1;
} // knapsack_instance::card_lb()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the maximum number of variables in the efficient solutions.
 */
unsigned int kp::knapsack_instance::card_ub() const
{
  std::vector<moo::real_type> w( m_variables.size() );
  std::transform( m_variables.begin(), m_variables.end(), w.begin(),
		  variable::weight_func() );
  std::sort( w.begin(), w.end() );

  moo::real_type sum = 0;
  unsigned int i = 0;

  do
    {
      sum += w[i];
      ++i;
    }
  while ( (i!=w.size()) && (sum <= m_capacity) );

  return i-1;
} // knapsack_instance::card_ub()
