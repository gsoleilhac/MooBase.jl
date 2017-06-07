/**
 * \file kp/impl/cardinality_lower_bound.tpp
 * \brief Implementation of the kp::cardinality_lower_bound class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the value of the bound.
 * \param inst The instance on which we compute the bound.
 */
template<typename View>
unsigned int
kp::cardinality_lower_bound<View>::compute( const view_type& inst ) const
{
  const unsigned int s = inst.get_size();

  std::vector<moo::real_type> weights(s);

  for (unsigned int i=0; i!=s; ++i)
    weights[i] = inst.get_variable(i).weight;

  std::sort( weights.begin(), weights.end(), std::greater<moo::real_type>() );

  unsigned int result = 0;
  moo::real_type sum_w = 0;
  const moo::real_type w = inst.get_capacity();

  for ( ; (sum_w <= w) && (result!=s) ; ++result )
    sum_w += weights[result];

  if (sum_w <= w)
    return result;
  else
    return result-1;
} // cardinality_lower_bound::compute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the value of the bound.
 * \param inst The instance on which we compute the bound.
 * \param free_vars The free variables.
 * \param w The capacity to consider for \a inst.
 */
template<typename View>
unsigned int kp::cardinality_lower_bound<View>::compute
( const view_type& inst, const std::vector<std::size_t>& free_vars,
  moo::real_type w ) const
{
  const unsigned int s = free_vars.size();

  std::vector<moo::real_type> weights(s);

  for (unsigned int i=0; i!=s; ++i)
    weights[i] = inst.get_variable(free_vars[i]).weight;

  std::sort( weights.begin(), weights.end(), std::greater<moo::real_type>() );

  unsigned int result = 0;
  moo::real_type sum_w = 0;

  for ( ; (sum_w <= w) && (result!=s) ; ++result )
    sum_w += weights[result];

  if (sum_w <= w)
    return result;
  else
    return result-1;
} // cardinality_lower_bound::compute()
