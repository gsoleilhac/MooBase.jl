/**
 * \file kp/impl/bounded_region.tpp
 * \brief Implementation of the template methods of the kp::bounded_region
 *        class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the nadir points.
 * \param first Iterator on the first nadir.
 * \param last Iterator just after the last nadir.
 */
template<typename Iterator>
void kp::bounded_region::insert_nadirs( Iterator first, Iterator last )
{
  for ( ; first!=last; ++first )
    m_bound_set.insert(*first);

  update_lower_bound();
} // bounded_region::insert_nadirs()

