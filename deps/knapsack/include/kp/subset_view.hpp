/**
 * \file kp/subset_view.hpp
 * \brief A class that gives a view on a subset of the variables.
 * \author Julien Jorge
 */
#ifndef __KP_SUBSET_VIEW_HPP__
#define __KP_SUBSET_VIEW_HPP__

#include <kp/knapsack_instance.hpp>

namespace kp
{
  /**
   * \brief A class that gives a view on a subset of the variables.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class subset_view:
    public base_view<View>
  {
  private:
    typedef base_view<View> super;
    typedef View view_type;

  public:
    subset_view( const view_type& v, const std::vector<std::size_t>& keep,
		 moo::real_type w );

    const variable& get_source_variable( const variable& v ) const;

    const variable& get_variable( unsigned int i ) const;
    moo::real_type get_capacity() const;
    unsigned int get_size() const;

  protected:
    /** \brief The variables of the problem. */
    std::vector<variable> m_variable;

    /** \brief The capacity of the sub problem. */
    const moo::real_type m_capacity;

    /** \brief The variables of the source problem. */
    std::vector<variable> m_source_variable;

  }; // class subset_view
} // namespace kp

#include <kp/impl/subset_view.tpp>

#endif // __KP_SUBSET_VIEW_HPP__
