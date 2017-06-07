/**
 * \file astar.hpp
 * \brief An A* algorithm for the multi-objective knapsack problem.
 * \author Julien Jorge
 */
#ifndef __KP_ASTAR_HPP__
#define __KP_ASTAR_HPP__

#include <kp/solution_data.hpp>
#include <kp/order_on_items.hpp>

#include <kp/view_weighted_sum.hpp>
#include <kp/phase_1.hpp>
#include <kp/subset_view.hpp>

#include <queue>

namespace kp
{
  /**
   * \brief A multi-objective branch and bound for the knapsack problem.
   * \author Julien Jorge
   */
  class astar
  {
  private:
    typedef moo::bound_set<cost_compare> bound_set_type;

    class node;
    typedef std::pair<node*, node*> node_pair;

    /** \brief A node in the exploration. */
    class node
    {
    private:
      typedef subset_view< view_order<> > sub_problem_type;
      typedef phase_1<sub_problem_type> phase_1_type;
      typedef std::set<phase_1_type::constraint_type> constraint_set_type;

    public:
      node( solution_data& data );

      moo::real_type get_remaining_capacity() const;
      moo::real_type get_tightness_ratio() const;
      std::size_t get_free_variables_count() const;
      std::size_t get_cardinality() const;
      const maximum_set& get_solutions() const;
      std::size_t get_nb_dominated_solutions() const;

      void update_nb_dominated_solutions();

      node_pair extends() const;
      bool bound_is_interesting();

    private:
      explicit node( const node& that );

      bool my_solutions_all_have( std::size_t i, moo::bin_status s ) const;
      void make_pattern();

      std::size_t next_index() const;
      bool branch_one( std::size_t i );
      bool branch_zero( std::size_t i );

      void unset_big_items();

      bool all_prefered_not_unset( std::size_t i ) const;
      bool all_dominated_not_set( std::size_t i ) const;

      bool interesting();
      bool hull_test() const;
      bool bound_test() const;

      bool diffuse_dominance_one();
      void diffuse_dominance_zero();

      moo::real_type fill_free_variables();
      void fill_my_solutions();

      void compute_utopian();
      void compute_utopian_upper_bound();

    private:
      /** \brief Some help to solve the instance. */
      solution_data& m_data;

      /** \brief The solutions found with this node. */
      maximum_set m_my_solutions;

      /** \brief The variables with the same value in m_my_solutions. */
      moo::binary_solution m_pattern;

      /** \brief The count of dominated solutions in m_my_solutions. */
      std::size_t m_nb_dominated_solutions;

      /** \brief The solution under construction. */
      knapsack_solution m_pending_solution;

      /** \brief The number of free variables. */
      std::size_t m_free_variables_count;

      /** \brief The remaining capacity. */
      moo::real_type m_remaining_capacity;

      /** \brief The tightness ratio on the free variables. */
      moo::real_type m_tightness_ratio;

      /** \brief The indices of the free variables, after branching. */
      std::vector<std::size_t> m_free_variables;

      /** \brief Utopian point, starting from this node. */
      cost_vector m_utopian;

      /** \brief An upper bound toward the utopian point. */
      moo::real_type m_utopian_upper_bound;

      /** \brief The hull of the solutions found with this node (with the
          reduced problem). */
      constraint_set_type m_reduced_hull;

      /** \brief Tell if this has been marked as not interesting. */
      bool m_dead;

    }; // class node

    class node_comparator
    {
    public:
      bool operator()( const node* const a, const node* const b ) const;

    private:
      int compare_min( std::size_t a, std::size_t b ) const;
      int compare_max( std::size_t a, std::size_t b ) const;
    }; // class node_comparator

  private:
    typedef std::list<node*>::iterator candidate_iterator_type;
    typedef std::list<candidate_iterator_type> candidate_list_type;
    typedef std::priority_queue<node*, std::vector<node*>, node_comparator>
    queue_type;

  public:
    void solve
    ( const knapsack_instance& inst, maximum_set& X,
      int o = order_on_items::order_topological );

  private:
    void update_nb_dominated_solutions( queue_type& pending );

  }; // class astar
} // namespace kp

#endif // __KP_ASTAR_HPP__
