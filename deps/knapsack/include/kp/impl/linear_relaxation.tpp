/**
 * \file kp/impl/linear_relaxation.tpp
 * \brief Implementation of the kp::linear_relaxation class.
 * \author Julien Jorge
 */

#include <kp/subset_view.hpp>
#include <kp/view_order.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the value of the relaxation.
 * \param inst The instance on which we compute the relaxation.
 * \param free_vars The free variables.
 * \param w The capacity to consider for \a inst.
 */
template<typename View>
kp::cost_vector kp::linear_relaxation<View>::compute
( const view_type& inst, const std::vector<std::size_t>& free_vars,
  moo::real_type w ) const
{
  subset_view<view_type> sub_p(inst, free_vars, w);
  cost_vector result(inst.get_nb_objectives(), 0);
  const std::size_t s = sub_p.get_size();

  for (std::size_t j=0; j!=sub_p.get_nb_objectives(); ++j)
    {
      view_order< subset_view<view_type> > mono(sub_p);

      mono.decreasing_efficiency(j);

      moo::real_type rem_w = w;

      bool stop = false;
      std::size_t k;

      for (k=0; k!=s && !stop; )
	if ( mono.get_variable(k).weight <= rem_w )
	  {
	    rem_w -= mono.get_variable(k).weight;
	    result[j] += mono.get_variable(k).cost[j];
	    ++k;
	  }
	else
	  stop = true;

      if (stop)
	result[j] +=
	  (rem_w * mono.get_variable(k).cost[j]) / mono.get_variable(k).weight;
    }

  return result;
} // linear_relaxation::compute()
