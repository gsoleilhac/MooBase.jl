/**
 * \file moo/facet.hpp
 * \brief A facet of a polytope.
 * \author Julien Jorge
 */
#ifndef __MOO_FACET_HPP__
#define __MOO_FACET_HPP__

#include "moo/constraint.hpp"
#include <vector>

namespace moo
{
  /**
   * \brief A facet of a polytope. This class is currently limited to polygons
   *        (polytopes of dimension 2).
   * \author Julien Jorge
   */
  template<typename T>
  class facet
  {
  public:
    typedef T value_type;
    typedef constraint<T> constraint_type;
    typedef vector<T> vector_type;

  public:
    facet();
    facet
    ( const constraint_type& c, const vector_type& p1, const vector_type& p2 );

    const constraint_type& get_constraint() const;

    std::ostream& output( std::ostream& os ) const;

  public:
    /** \brief The constraint defining the facet. */
    constraint_type m_constraint;

    /** \brief The first intersections with the other facets. */
    std::vector<vector_type> p;

  }; // struct facet
} // namespace moo

template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::facet<T>& f );

#include "moo/impl/facet.tpp"

#endif // __MOO_FACET_HPP__
