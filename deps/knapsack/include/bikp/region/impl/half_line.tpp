#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param combined_min The minimum interesting value.
 */
template<class ProfitTraits>
half_line<ProfitTraits>::half_line
( const profit_type min_value, bool keep_equivalent )
  : m_min_value(min_value), m_keep_equivalent(keep_equivalent)
{

} // half_line::half_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum interesting profit value.
 */
template<class ProfitTraits>
typename half_line<ProfitTraits>::profit_type
half_line<ProfitTraits>::min_profit() const
{
  return m_min_value;
} // half_line::min_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region contains a quality of solution.
 * \param p The quality to check.
 */
template<class ProfitTraits>
bool half_line<ProfitTraits>::contains( const profit_type p ) const
{
  return (integer_type)p >= (integer_type)m_min_value;
} // half_line::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we can hope to reach the region from a given quality of
 *        solution.
 * \param p The quality to check.
 */
template<class ProfitTraits>
bool half_line<ProfitTraits>::feasible( const profit_type& p ) const
{
  return true;
} // half_line::feasible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The item to copy from.
 */
template<class ProfitTraits>
typename half_line<ProfitTraits>::self_type&
half_line<ProfitTraits>::operator=( const self_type& that )
{
  m_min_value = that.m_min_value;
  return *this;
} // half_line::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the region according to a set of points. The minimum point of
 *        the region is set equal to or greater than the maximum point of the
 *        set.
 * \param first Iterator on the first point in the set.
 * \param last Iterator past the last point in the set.
 */
template<class ProfitTraits>
template<class Iterator>
void half_line<ProfitTraits>::tighten( Iterator first, Iterator last )
{
  for ( ; first != last; ++first)
    if ( (integer_type)(profit_type)*first > (integer_type)m_min_value )
      m_min_value = *first;

  if ( !m_keep_equivalent )
    profit_traits_type::inc(m_min_value);
} // half_line::tighten()
