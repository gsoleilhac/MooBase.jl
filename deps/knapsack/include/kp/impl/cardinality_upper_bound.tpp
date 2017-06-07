/**
 * \file kp/impl/cardinality_upper_bound.tpp
 * \brief Implementation of the kp::cardinality_upper_bound class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the value of the bound.
 * \param inst The instance on which we compute the bound.
 * \param free_vars The free variables.
 * \param w The capacity to consider for \a inst.
 */
template<typename View>
std::size_t kp::cardinality_upper_bound<View>::compute
( const view_type& inst, const std::vector<std::size_t>& free_vars,
  moo::real_type w ) const
{
  const std::size_t s = free_vars.size();

  std::vector<moo::real_type> weights(s);

  for (std::size_t i=0; i!=s; ++i)
    weights[i] = inst.get_variable(free_vars[i]).weight;

  std::sort( weights.begin(), weights.end() );

  std::size_t result = 0;
  moo::real_type sum_w = 0;

  for ( ; (sum_w <= w) && (result!=s) ; ++result )
    sum_w += weights[result];

  if (sum_w <= w)
    return result;
  else
    return result-1;
} // cardinality_upper_bound::compute()
