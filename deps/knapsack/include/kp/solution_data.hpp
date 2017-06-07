/**
 * \file solution_date.hpp
 * \brief Solution data for solvers.
 * \author Julien Jorge
 */
#ifndef __KP_SOLUTION_DATA_HPP__
#define __KP_SOLUTION_DATA_HPP__

#include <kp/view_order.hpp>
#include <kp/types.hpp>
#include <moo/bound_set.hpp>

namespace kp
{
  /**
   * \brief Solution data for solvers.
   * \author Julien Jorge
   */
  class solution_data
  {
  public:
    typedef moo::bound_set<cost_compare> bound_set_type;

  public:
    solution_data
    ( const knapsack_instance& inst, maximum_set& solutions, int o );

    const knapsack_instance& get_instance() const;
    const view_order<> get_view() const;
    std::size_t get_cardinality_lower_bound() const;
      
    void insert( const knapsack_solution& sol );

    bool is_dominated( const knapsack_solution& sol ) const;
    bool is_dominated( const cost_vector& y ) const;
    bool first_dominates_second( std::size_t i, std::size_t j ) const;

    bound_set_type::const_iterator bound_set_begin() const;
    bound_set_type::const_iterator bound_set_end() const;

    bool is_modified() const;
    void unset_modified();

  private:
    bool dominates( std::size_t i, std::size_t j ) const;

  private:
    /** \brief The initial knapsack instance. */
    const knapsack_instance& m_instance;

    /** \brief The knapsack to explore. */
    view_order<> m_kp;

    /** \brief A lower bound on the cardinality of the efficient solutions. */
    std::size_t m_kp_card_lb;

    /** \brief An lower bound set for the value of the efficient solutions. */
    bound_set_type m_bound_set;

    /** \brief The efficient solutions found. */
    maximum_set& m_solutions;

    /** \brief m_dominates[i][j] tells if a variable i dominates an other
        variable j. */
    std::vector< std::vector<bool> > m_dominates;

    /** \brief Tell if m_solutions has received a solution corresponding to a
        new non dominated point. */
    bool m_modified;

  }; // class solution_data
} // namespace kp

#endif // __KP_SOLUTION_DATA_HPP__
