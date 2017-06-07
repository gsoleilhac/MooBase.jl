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
 * \file half_plane.hpp
 * \brief This class is a part of a plane splited with a line, in the objective
 *        space.
 * \author Julien Jorge
 */
#ifndef __BIKP_HALF_PLANE_HPP__
#define __BIKP_HALF_PLANE_HPP__

/**
 * \brief This class is a part of a plane splited with a line, in the objective
 *        space.
 *
 * \author Julien Jorge
 */
template<class ProfitTraits>
class half_plane
{
public:
  /** \brief Basic operations on profits. */
  typedef ProfitTraits profit_traits_type;

  /** \brief The type of the profits in our search region. */
  typedef typename profit_traits_type::profit_type profit_type;

  /** \brief The type of the current class. */
  typedef half_plane<profit_traits_type> self_type;

public:
  half_plane( real_type origin, real_type lambda_1,
	      real_type lambda_2 );

  bool is_over( const profit_type& p ) const;
  bool is_under( const profit_type& p ) const;
  bool is_on( const profit_type& p ) const;

  bool operator<( const profit_type& p ) const;
  bool operator<=( const profit_type& p ) const;
  bool operator>( const profit_type& p ) const;
  bool operator>=( const profit_type& p ) const;

private:
  /** \brief Coefficient on the first objective. */
  const real_type m_lambda_1;

  /** \brief Coefficient on the second objective. */
  const real_type m_lambda_2;

  /** \brief The value of the line in (0,0). */
  const real_type m_origin;

}; // class half_plane

template<class ProfitTraits>
bool operator<( const typename ProfitTraits::profit_type& p,
		const half_plane<ProfitTraits>& hp );

template<class ProfitTraits>
bool operator<=( const typename ProfitTraits::profit_type& p,
		 const half_plane<ProfitTraits>& hp );

template<class ProfitTraits>
bool operator>( const typename ProfitTraits::profit_type& p,
		const half_plane<ProfitTraits>& hp );

template<class ProfitTraits>
bool operator>=( const typename ProfitTraits::profit_type& p,
		 const half_plane<ProfitTraits>& hp );

#include "bikp/region/impl/half_plane.tpp"

#endif // __BIKP_HALF_PLANE_HPP__
