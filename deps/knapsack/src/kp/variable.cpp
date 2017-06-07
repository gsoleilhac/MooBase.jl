/**
 * \file variable.cpp
 * \brief Implementation of the kp::variable class.
 * \author Julien Jorge
 */
#include "kp/variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the weight of a variable.
 * \param v The variable of which we want the weight.
 */
moo::real_type kp::variable::weight_func::operator()( const variable& v ) const
{
  return v.weight;
} // variable::weight_func::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param i The index of the variable in the problem.
 * \param c The cost of the variable on all objectives.
 * \param w The weight of the variable.
 */
kp::variable::variable
( unsigned int i, const cost_vector& c, moo::real_type w )
  : index(i), cost(c), weight(w)
{

} // variable::variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the efficiency of the variable.
 */
moo::vector<moo::real_type> kp::variable::efficiency() const
{
  return moo::vector<moo::real_type>(cost / weight);
} // variable::efficiency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a variable in an output stream.
 * \param os The stream in which we write.
 * \param v The variable to write.
 */
std::ostream& operator<<( std::ostream& os, const kp::variable& v )
{
  return os << v.index << '\t' << v.cost << '\t' << v.weight;
} // operator<<() [variable]
