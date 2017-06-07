/**
 * \file kp/base_view.hpp
 * \brief A base class for the views.
 * \author Julien Jorge
 */
#ifndef __KP_BASE_VIEW_HPP__
#define __KP_BASE_VIEW_HPP__

#include <kp/knapsack_instance.hpp>

namespace kp
{
  /**
   * \brief A base class for the views.
   * \author Julien Jorge
   */
  template<typename View = knapsack_instance>
  class base_view
  {
  public:
    base_view( const View& v );

    const variable& get_variable( unsigned int i ) const;
    moo::real_type get_capacity() const;
    unsigned int get_size() const;
    unsigned int get_nb_objectives() const;

  protected:
    /** \brief The instance of the problem. */
    const View& m_view;

  }; // class base_view
} // namespace kp

#include <kp/impl/base_view.tpp>

#endif // __KP_BASE_VIEW_HPP__
