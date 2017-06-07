/**
 * \file kp/impl/subset_view.hpp
 * \brief Implementation of the kp::subset_view class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The view on which we have a view.
 * \param keep The indices of the variables to keep.
 * \param w The capacity of the (sub)knapsack.
 */
template<typename View>
kp::subset_view<View>::subset_view
( const view_type& v, const std::vector<std::size_t>& keep, moo::real_type w )
  : super(v), m_capacity(w)
{
  const unsigned int s = keep.size();
  m_variable.reserve(s);
  m_source_variable.reserve(s);

  for (unsigned int i=0; i!=s; ++i)
    {
      variable v(this->m_view.get_variable( keep[i] ));
      m_source_variable.push_back(v);
      v.index = i;
      m_variable.push_back( v );
    }
} // subset_view::subset_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the source problem.
 * \param v The variable in the sub problem.
 */
template<typename View>
const kp::variable&
kp::subset_view<View>::get_source_variable( const variable& v ) const
{
  return m_source_variable[v.index];
} // subset_view::get_source_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variable.
 */
template<typename View>
const kp::variable& kp::subset_view<View>::get_variable( unsigned int i ) const
{
  return m_variable[i];
} // subset_view::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the capacity of the problem.
 */
template<typename View>
moo::real_type kp::subset_view<View>::get_capacity() const
{
  return m_capacity;
} // subset_view::get_capacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the problem.
 */
template<typename View>
unsigned int kp::subset_view<View>::get_size() const
{
  return m_variable.size();
} // subset_view::get_size()
