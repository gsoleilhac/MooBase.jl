/**
 * \file kp/view_weighted_sum.hpp
 * \brief A class that gives a mono-objective view on a knapsack instance by
 *        applying a weighted sum on its objective coefficients.
 * \author Julien Jorge
 */
#ifndef __KP_VIEW_WEIGHTED_SUM_HPP__
#define __KP_VIEW_WEIGHTED_SUM_HPP__

#include <kp/base_view.hpp>

namespace kp
{
  /**
   * \brief A class that gives a view on a knapsack instance by applying to it
   *        an order on the variables.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class view_weighted_sum:
    public base_view<View>
  {
  private:
    typedef View view_type;
    typedef base_view<View> super;

  public:
    view_weighted_sum( const view_type& v, const cost_vector& w );

    const variable& get_variable( unsigned int i ) const;
    unsigned int get_nb_objectives() const;

  private:
    /** \brief The variables of the problem. */
    std::vector<variable> m_variable;

  }; // class view_weighted_sum
} // namespace kp

#include <kp/impl/view_weighted_sum.tpp>

#endif // __KP_VIEW_WEIGHTED_SUM_HPP__
