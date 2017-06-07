/**
 * \file kp/view_order.hpp
 * \brief A class that gives a view on a knapsack instance by applying to it an
 *        order on the variables.
 * \author Julien Jorge
 */
#ifndef __KP_VIEW_ORDER_HPP__
#define __KP_VIEW_ORDER_HPP__

#include <kp/base_view.hpp>
#include <kp/types.hpp>

namespace kp
{
  /**
   * \brief A class that gives a view on a knapsack instance by applying to it
   *        an order on the variables.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class view_order:
    public base_view<View>
  {
  private:
    typedef View view_type;
    typedef base_view<View> super;
    typedef view_order<View> self_type;

  private:
    /** \brief Compare two variables by their efficiency. */
    class efficiency_greater
    {
    public:
      efficiency_greater( const view_type& v, const cost_vector& w );
      bool operator()(unsigned int i, unsigned int j) const;
      
    private:
      /** \brief The problem where we take the variables. */
      const view_type& m_view;

      /** \brief The weight applied to the cost of the variables. */
      const cost_vector& m_weight;

    }; // class efficiency_greater

    /** \brief Compare two variables by their frequency. */
    class frequency_greater
    {
    public:
      frequency_greater( const std::vector<std::size_t>& freq );
      bool operator()(unsigned int i, unsigned int j) const;
      
    private:
      /** \brief The frequency of the variables. */
      const std::vector<std::size_t>& m_frequency;

    }; // class frequency_greater

    /** \brief Compare to variables according to an associated value. */
    class valued_min_order
    {
    public:
      valued_min_order( const std::vector<moo::real_type>& v );

      bool operator()( unsigned int i, unsigned int j ) const;

    private:
      /** \brief The values associated to the variables. */
      const std::vector<moo::real_type>& m_value;

    }; // class valued_min_order

  public:
    view_order( const view_type& v );

    const variable& get_variable( unsigned int i ) const;

    void decreasing_efficiency( unsigned int obj );
    void decreasing_efficiency( const cost_vector& v );
    void topological_efficiency();

    void max_rank();
    void min_rank();
    void sum_rank();

    void frequency( const maximum_set& X );

    void random();
    void domination_rank();
    void domination_count();

  private:
    void compute_rank( std::vector< std::vector<std::size_t> >& r );
    void get_efficiencies
    ( std::vector< moo::vector<moo::real_type> >& eff ) const;

  private:
    /** \brief The order applied to the variables. */
    std::vector<std::size_t> m_order;

  }; // class view_order
} // namespace kp

#include <kp/impl/view_order.tpp>

#endif // __KP_VIEW_ORDER_HPP__
