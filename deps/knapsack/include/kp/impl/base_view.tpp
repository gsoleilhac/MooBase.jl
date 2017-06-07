/**
 * \file kp/impl/base_view.tpp
 * \brief Implementation of the kp::base_view class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The view on which we have a view.
 */
template<typename View>
kp::base_view<View>::base_view( const View& v )
  : m_view(v)
{

} // base_view::base_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variables.
 */
template<typename View>
const kp::variable& kp::base_view<View>::get_variable( unsigned int i ) const
{
  return m_view.get_variable(i);
} // base_view::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the capacity of the knapsack.
 */
template<typename View>
moo::real_type kp::base_view<View>::get_capacity() const
{
  return m_view.get_capacity();
} // base_view::get_capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the problem.
 */
template<typename View>
unsigned int kp::base_view<View>::get_size() const
{
  return m_view.get_size();
} // base_view::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of objectives of the problem.
 */
template<typename View>
unsigned int kp::base_view<View>::get_nb_objectives() const
{
  return m_view.get_nb_objectives();
} // base_view::get_nb_objectives()
