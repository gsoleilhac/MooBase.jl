/**
 * \file weight_set.hpp
 * \brief A weight_set for the knapsack. It stores the facets and the adjacent
 *        points.
 * \author Julien Jorge
 */
#ifndef __KP_WEIGHT_SET_HPP__
#define __KP_WEIGHT_SET_HPP__

#include <moo/facet.hpp>
#include <moo/types.hpp>
#include <kp/cost_vector.hpp>
#include <list>

namespace kp
{
  /**
   * \brief A weight_set for the knapsack. It stores the facets and the adjacent
   *        points. This class is currently limited to polygons (weight sets of
   *        dimension 2).
   * \author Julien Jorge
   */
  class weight_set
  {
  public:
    typedef moo::real_type value_type;
    typedef moo::constraint<moo::real_type> constraint_type;
    typedef cost_vector vector_type;
    typedef moo::facet<moo::real_type> facet_type;
    typedef std::pair<vector_type*, facet_type> neighbor_type;

    typedef std::list<neighbor_type> facet_list_type;
    typedef facet_list_type::const_iterator const_facet_iterator;

  public:
    weight_set( const vector_type& y );
    ~weight_set();

    const vector_type& get_point() const;

    const_facet_iterator facets_begin() const;
    const_facet_iterator facets_end() const;

    void add_constraint( const vector_type& adj );
    bool check( const vector_type& v ) const;

    bool empty() const;

    std::ostream& output( std::ostream& os ) const;
    void gnuplot( std::ostream& gp, std::ostream& data,
		  const std::string& data_name ) const;

  private:
    bool conflicting( const constraint_type& c ) const;
    bool redundant( const constraint_type& c ) const;
    void insert_and_update( const vector_type& v, const constraint_type& c );

  private:
    /** \brief The point optimal for the weights of this polytope. */
    const vector_type& m_point;

    /** \brief Tell if the weight set is empty. */
    bool m_empty;

    /** \brief The constraints that define the weight_set. */
    facet_list_type m_constraint;

  }; // class weight_set
} // namespace kp

std::ostream& operator<<( std::ostream& os, const kp::weight_set& p );

#endif // __KP_WEIGHT_SET_HPP__
