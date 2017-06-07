/**
 * \file phase_1.hpp
 * \brief An algorithm to compute the coordinates of all supported solutions.
 * \author Julien Jorge
 */
#ifndef __KP_PHASE_1_HPP__
#define __KP_PHASE_1_HPP__

#include <kp/knapsack_instance.hpp>
#include <kp/knapsack_solution.hpp>
#include <kp/weight_set.hpp>

#include <list>
#include <map>
#include <set>

namespace kp
{
  /**
   * \brief An algorithm to compute the coordinates of all supported solutions.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class phase_1
  {
  public:
    typedef View view_type;
    typedef weight_set weight_set_type;
    typedef weight_set_type::constraint_type constraint_type;

  private:
    typedef weight_set_type::facet_type facet_type;
    typedef weight_set_type::neighbor_type neighbor_type;
    typedef std::map<cost_vector, std::list<cost_vector> > adjacency_type;

  public:
    phase_1( const view_type& inst );

    const std::list<knapsack_solution>& get_XSEm() const;
    const std::list<knapsack_solution>& get_XSEM() const;
    const std::set<constraint_type>& get_hull() const;

    void run();

  private:
    neighbor_type choose_facet
    ( const weight_set_type& w_p, const cost_vector& neighbor ) const;

    void compute_polytope( weight_set_type& w_p );
    void update_polytope
    ( const std::list<knapsack_solution>& X, weight_set_type& w_p );

    void rebuild_potentially_adjacents( weight_set_type& w_p );
    void lexicographic_optimal();
    void bicriteria_solver
    ( const cost_vector& lambda_1, const cost_vector& lambda_2,
      std::list<knapsack_solution>& X ) const;

    void update_adjacency
    ( const cost_vector& lambda_1, const cost_vector& lambda_2,
      const std::list<knapsack_solution>& X, const cost_vector& y,
      const neighbor_type& f );
    void update_border_adjacency
    ( const cost_vector& lambda, const cost_vector& p,
      const cost_vector& y, const cost_vector& neighbor );
    void update_local_adjacency( const std::list<cost_vector>& Z );

    void add_constraint_to_hull( const neighbor_type& f );
    bool make_lambda
    ( const facet_type::vector_type& w, facet_type::vector_type& lambda ) const;

    void find_equivalent_solutions();

  private:
    /** \brief The problem to solve. */
    const view_type& m_kp;

    /** \brief The supported solutions found up to now. */
    std::list<knapsack_solution> m_XSEm;

    /** \brief The supported solutions, including al equivalent ones. */
    std::list<knapsack_solution> m_XSEM;

    /** \brief The points adjacent to a given point. */
    adjacency_type m_adjacent;

    /** \brief The points potentially adjacent to a given point. */
    adjacency_type m_potentially_adjacent;

    /** \brief The constraints defining the hull of the supported solutions. */
    std::set<constraint_type> m_hull;

  }; // class phase_1
} // namespace kp

#include <kp/impl/phase_1.tpp>

#endif // __KP_PHASE_1_HPP__
