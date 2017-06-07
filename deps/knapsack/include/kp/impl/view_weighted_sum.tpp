/**
 * \file kp/impl/view_weighted_sum.tpp
 * \brief Implementation of the kp::view_weighted_sum class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The view on which we have a view.
 * \param w The weigths applied to the objective coefficients.
 */
template<typename View>
kp::view_weighted_sum<View>::view_weighted_sum
( const View& v, const kp::cost_vector& w )
  : super(v)
{
  const unsigned int s = v.get_size();
  m_variable.reserve(s);

  for (unsigned int i=0; i!=s; ++i)
    {
      const variable& var( v.get_variable(i) );
      cost_vector c(1);

      c[0] = var.cost.scalar_product(w);
      m_variable.push_back( variable(var.index, c, var.weight) );
    }
} // view_weighted_sum::view_weighted_sum()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variables.
 */
template<typename View>
const kp::variable&
kp::view_weighted_sum<View>::get_variable( unsigned int i ) const
{
  return m_variable[i];
} // view_weighted_sum::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of objectives of the problem.
 */
template<typename View>
unsigned int kp::view_weighted_sum<View>::get_nb_objectives() const
{
  return 1;
} // base_view::get_nb_objectives()
