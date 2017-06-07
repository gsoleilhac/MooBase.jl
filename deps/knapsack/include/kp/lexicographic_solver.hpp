/**
 * \file kp/lexicographic_solver.hpp
 * \brief An algorithm to compute the coordinates of all lexicographic optimal
 *        solutions.
 * \author Julien Jorge
 */
#ifndef __KP_LEXICOGRAPHIC_SOLVER_HPP__
#define __KP_LEXICOGRAPHIC_SOLVER_HPP__

#include <kp/knapsack_solution.hpp>
#include <list>

namespace kp
{
  /**
   * \brief An algorithm to compute the coordinates of all lexicographic optimal
   *        solutions.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class lexicographic_solver
  {
  public:
    typedef View view_type;

  public:
    void solve( const view_type& inst, std::list<knapsack_solution>& X ) const;

  private:
    void compute_coeff
    ( const std::vector<std::size_t>& order, const cost_vector& ideal,
      const cost_vector& ref, cost_vector& coeff,
      unsigned int index = 0 ) const;

    cost_vector
    compute_ideal( const view_type& inst,
		   std::vector<knapsack_solution>& points ) const;

    void solve_mono
    ( const view_type& inst, const cost_vector& coeff,
      knapsack_solution& result ) const;

  }; // class lexicographic_solver
} // namespace kp

#include <kp/impl/lexicographic_solver.tpp>

#endif // __KP_LEXICOGRAPHIC_SOLVER_HPP__
