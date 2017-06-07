/**
 * \file mobb.hpp
 * \brief A multi-objective branch and bound for the knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_MOBB_HPP__
#define __KP_MOBB_HPP__

#include <kp/order_on_items.hpp>
#include <kp/view_order.hpp>
#include <kp/types.hpp>
#include <moo/bound_set.hpp>


namespace kp
{
  /**
   * \brief A multi-objective branch and bound for the knapsack problem.
   * \author Julien Jorge
   */
  class mobb
  {
  private:
    typedef moo::bound_set<cost_compare> bound_set_type;

    /** \brief Informations about the status of the branch and bound. */
    class bb_procedure
    {
    public:
      bb_procedure( const knapsack_instance& instance, maximum_set& X, int o );

      void run();

    private:
      bool dominates( unsigned int i, unsigned int j ) const;

      void bb( moo::real_type w, knapsack_solution& sol, unsigned int n );

      unsigned int next_index( const knapsack_solution& sol ) const;

      void branch_one
      ( moo::real_type w, knapsack_solution& sol, unsigned int i,
        unsigned int n );

      void branch_zero
      ( moo::real_type w, knapsack_solution& sol, unsigned int i,
        unsigned int n );

      unsigned int unset_big_items
      ( moo::real_type w, knapsack_solution& sol,
        std::list<unsigned int>& set_to_zero ) const;

      bool interesting
      ( moo::real_type w, const knapsack_solution& sol, unsigned int n ) const;

      bool hull_test
      ( moo::real_type w, const knapsack_solution& sol,
        const std::vector<std::size_t>& free_vars ) const;

      bool bound_test
      ( moo::real_type w, const knapsack_solution& sol,
        const std::vector<std::size_t>& free_vars,
        const cost_vector& utopian ) const;

      bool diffuse_dominance_one
      ( moo::real_type& w, knapsack_solution& sol, unsigned int& n,
        std::list<unsigned int>& set_to_one ) const;

      void diffuse_dominance_zero
      ( knapsack_solution& sol, unsigned int& n,
        std::list<unsigned int>& set_to_zero ) const;

    public:
      /** \brief Total number of nodes. */
      mutable std::size_t total_nodes;

      /** \brief Nodes closed by diffuse_dominance_one(). */
      mutable std::size_t closed_diffuse_dominance_one_counter;

      /** \brief Nodes closed by cardinality test. */
      mutable std::size_t closed_cardinality_counter;

      /** \brief Nodes closed by dominance of the utopian. */
      mutable std::size_t closed_utopian_counter;

      /** \brief Nodes closed by relaxation. */
      mutable std::size_t closed_relaxation_counter;

    private:
      /** \brief The knapsack to solve. */
      const knapsack_instance& m_instance;

      /** \brief The knapsack to explore. */
      view_order<> m_kp;

      /** \brief A lower bound on the cardinality of the efficient solutions. */
      unsigned int m_kp_card_lb;

      /** \brief An lower bound set for the value of the efficient solutions. */
      bound_set_type m_bound_set;

      /** \brief The efficient solutions found. */
      maximum_set& m_solutions;

      /** \brief m_dominates[i][j] tells if a variable i dominates an other
          variable j. */
      std::vector< std::vector<bool> > m_dominates;

    }; // class bb_procedure

  public:
    void solve
    ( const knapsack_instance& inst, maximum_set& X,
      int o = order_on_items::order_topological );

  }; // class mobb
} // namespace kp

#endif // __KP_MOBB_HPP__
