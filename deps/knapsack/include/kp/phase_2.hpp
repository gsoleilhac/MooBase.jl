/**
 * \file phase_2.hpp
 * \brief An algorithm to compute all non supported solutions.
 * \author Julien Jorge
 */
#ifndef __KP_PHASE_2_HPP__
#define __KP_PHASE_2_HPP__

#include <kp/knapsack_instance.hpp>
#include <kp/knapsack_solution.hpp>
#include <kp/weight_set.hpp>

#include <list>
#include <map>
#include <set>

namespace kp
{
  /**
   * \brief An algorithm to compute all supported solutions.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class phase_2
  {
  public:
    typedef View view_type;
    typedef moo::constraint<moo::real_type> constraint_type;

    typedef std::list<knapsack_solution> solution_list;
    typedef std::set<constraint_type> constraint_set;
    typedef std::list<cost_vector> cost_vector_list;
    typedef std::set<cost_vector> cost_vector_set;

  public:
    phase_2( const view_type& inst, const solution_list& XSEM,
             const constraint_set& hull );

    const maximum_set& get_XNSEM() const;

    void run();

  private:
    void solve( const bounded_region& region, pareto_set& out_profits );
    void reduce_and_solve
    ( const bounded_region& region, pareto_set& out_profits );

    void choose_weight_and_points
    ( const std::list<constraint_type>& H, const bound_set& E,
      constraint_type& h, cost_vector_list& U) const;

    void compute_distances
    ( std::map<moo::real_type, constraint_set>& val,
      std::map<constraint_type, cost_vector_set>& U,
      const cost_vector& nadir ) const;
    void compute_distances_2
    ( std::map<moo::real_type, constraint_set>& val,
      std::map<constraint_type, cost_vector_set>& U,
      const cost_vector& nadir ) const;

    moo::real_type dist( const cost_vector& u, const constraint_type& h ) const;
    void build_bound_set( bound_set& bs ) const;

    cost_vector compute_nadir();
    void solve_bi( unsigned int a, unsigned int b );

  private:
    /** \brief The problem to solve. */
    const view_type& m_kp;

    /** \brief The supported solutions. */
    const solution_list& m_XSEM;

    /** \brief The hull of feasible solutions. */
    const constraint_set& m_hull;

    /** \brief The not supported solutions, including al equivalent ones. */
    maximum_set m_XNSEM;

  }; // class phase_2
} // namespace kp

#include <kp/impl/phase_2.tpp>

#endif // __KP_PHASE_2_HPP__
