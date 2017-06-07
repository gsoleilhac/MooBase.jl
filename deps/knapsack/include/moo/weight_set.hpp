/**
 * \file moo/weight_set.hpp
 * \brief A weight_set.
 * \author Julien Jorge
 */
#ifndef __MOO_WEIGHT_SET_HPP__
#define __MOO_WEIGHT_SET_HPP__

#include "moo/facet.hpp"
#include <list>

namespace moo
{
  /**
   * \brief A weight_set. This class is currently limited to polygons (weight
   *        sets of dimension 2).
   * \author Julien Jorge
   */
  template<typename T>
  class weight_set
  {
  public:
    typedef T value_type;
    typedef constraint<T> constraint_type;
    typedef vector<T> vector_type;
    typedef facet<T> facet_type;

  public:
    weight_set();

    void add_constraint( const constraint_type& c );
    bool check( const vector_type& v )const ;

    bool empty() const;

    std::ostream& output( std::ostream& os ) const;
    void gnuplot( std::ostream& gp, std::ostream& data,
		  const std::string& data_name ) const;

  private:
    bool conflicting( const constraint_type& c ) const;
    bool redundant( const constraint_type& c ) const;
    void insert_and_update( const constraint_type& c );

  private:
    /** \brief Tell if the weight set is empty. */
    bool m_empty;

    /** \brief The constraints that define the weight_set. */
    std::list<facet_type> m_constraint;

  }; // class weight_set
} // namespace moo

template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::weight_set<T>& p );

#include "moo/impl/weight_set.tpp"

#endif // __MOO_WEIGHT_SET_HPP__
