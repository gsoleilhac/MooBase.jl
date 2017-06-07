/**
 * \file kp/bounded_region.hpp
 * \brief A region in the objective space, that contains the feasible solutions.
 * \author Julien Jorge
 */
#ifndef __KP_BOUNDED_REGION_HPP__
#define __KP_BOUNDED_REGION_HPP__

#include "kp/types.hpp"

namespace kp
{
  /**
   * \brief A region in the objective space, that contains the feasible
   *        solutions.
   * \author Julien Jorge
   */
  class bounded_region
  {
  public:
    bounded_region
    ( const cost_vector& direction, const moo::real_type& lower_bound = 0 );
    bounded_region
    ( const cost_vector& direction, const cost_vector& nadir,
      const moo::real_type& lower_bound = 0 );

    bool reduce( const cost_vector& v );
    bool is_dominated( const cost_vector& v ) const;
    bool is_dominated_eq( const cost_vector& v ) const;
    bool contains( const cost_vector& v ) const;

    const cost_vector& get_direction() const;
    const moo::real_type& get_lower_bound() const;
    const cost_vector& get_nadir() const;

    pareto_set::const_iterator pareto_begin() const;
    pareto_set::const_iterator pareto_end() const;

    bound_set::const_iterator bound_set_begin() const;
    bound_set::const_iterator bound_set_end() const;

    std::size_t non_dominated_size() const { return m_non_dominated.size(); }
    std::size_t bound_set_size() const { return m_bound_set.size(); }

    bounded_region& operator=( const bounded_region& that );

    void gnuplot() const;

    template<typename Iterator>
    void insert_nadirs( Iterator first, Iterator last );

  private:
    void update_lower_bound();

    moo::real_type lower_bound_on_face(std::size_t j) const;

  private:
    /** \brief The set of non dominated points that serves as an upper bound. */
    pareto_set m_non_dominated;

    /** \brief The direction in which we optimise. */
    const cost_vector m_direction;

    /** \brief Global nadir point passed to the constructor. */
    const cost_vector m_nadir;

    /** \brief The bound set that serves as a lower bound. */
    bound_set m_bound_set;

    /** \brief The lower bound in a mono-objective viewpoint. */
    moo::real_type m_lower_bound;

    /** \brief The point on which the lower bound was computed. */
    cost_vector m_lower_bound_point;

  }; // class bounded region
} // namespace kp

#include "kp/impl/bounded_region.tpp"

#endif // __KP_BOUNDED_REGION_HPP__
