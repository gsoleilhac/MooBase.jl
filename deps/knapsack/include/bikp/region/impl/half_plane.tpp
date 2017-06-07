/*
  Copyright Faculté des Sciences et Techniques de Nantes
  contributor:
    Julien Jorge <julien.jorge@univ-nantes.fr>

  This software is a computer program whose purpose is to exactly solve
  instances of the bi-objective unidimensional 0-1 knapsack problem.

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 
*/
/**
 * \file half_plane.tpp
 * \brief Implementation of the half_plane class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param origin The value of the line for x=y=0.
 * \param lambda_1 Coefficient on the first objective.
 * \param lambda_2 Coefficient on the second objective.
 */
template<class ProfitTraits>
half_plane<ProfitTraits>::half_plane
( real_type origin, real_type lambda_1, real_type lambda_2 )
  : m_lambda_1(lambda_1), m_lambda_2(lambda_2), m_origin(origin)
{

} // half_plane::half_plane()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the line is over over a point.
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::is_over( const profit_type& p ) const
{
  return m_lambda_1 * p.profit_1 + m_lambda_2 * p.profit_2 < m_origin;
} // half_plane::is_over()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the line is under a point.
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::is_under( const profit_type& p ) const
{
  return m_lambda_1 * p.profit_1 + m_lambda_2 * p.profit_2 > m_origin;
} // half_plane::is_under()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a point is on the line.
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::is_on( const profit_type& p ) const
{
  return m_lambda_1 * p.profit_1 + m_lambda_2 * p.profit_2 == m_origin;
} // half_plane::is_under()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call is_under().
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::operator<( const profit_type& p ) const
{
  return is_under(p);
} // half_plane::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call is_under() or is_on().
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::operator<=( const profit_type& p ) const
{
  return is_under(p) || is_on(p);
} // half_plane::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call is_over().
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::operator>( const profit_type& p ) const
{
  return is_over(p);
} // half_plane::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call is_over() or is_on().
 * \param p The point to check.
 */
template<class ProfitTraits>
bool half_plane<ProfitTraits>::operator>=( const profit_type& p ) const
{
  return is_over(p) || is_on(p);
} // half_plane::operator>=()




/*----------------------------------------------------------------------------*/
template<class ProfitTraits>
bool operator<( const typename ProfitTraits::profit_type& p,
		const half_plane<ProfitTraits>& hp )
{
  hp > p;
} // operator<()

/*----------------------------------------------------------------------------*/
template<class ProfitTraits>
bool operator<=( const typename ProfitTraits::profit_type& p,
		 const half_plane<ProfitTraits>& hp )
{
  hp >= p;
} // operator<()

/*----------------------------------------------------------------------------*/
template<class ProfitTraits>
bool operator>( const typename ProfitTraits::profit_type& p,
		const half_plane<ProfitTraits>& hp )
{
  hp < p;
} // operator<()

/*----------------------------------------------------------------------------*/
template<class ProfitTraits>
bool operator>=( const typename ProfitTraits::profit_type& p,
		 const half_plane<ProfitTraits>& hp )
{
  hp <= p;
} // operator<()
