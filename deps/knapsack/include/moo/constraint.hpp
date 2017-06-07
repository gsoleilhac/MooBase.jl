/**
 * \file moo/constraint.hpp
 * \brief A linear contraint.
 * \author Julien Jorge
 */
#ifndef __MOO_CONSTRAINT_HPP__
#define __MOO_CONSTRAINT_HPP__

#include "moo/vector.hpp"

namespace moo
{
  /**
   * \brief A linear contraint.
   * \author Julien Jorge
   */
  template<typename T>
  class constraint
  {
  public:
    typedef T value_type;
    typedef constraint<T> self_type;
    typedef vector<T> vector_type;

  public:
    /** \brief Direction of the comparison with the right hand side. */
    enum direction
      {
        less,
        less_equal,
        equal,
        greater,
        greater_equal
      }; // enum direction

  public:
    constraint();
    constraint( const vector_type& c, direction d, value_type rhs );

    const vector_type& get_coefficient() const;
    const value_type& get_right_hand_side() const;

    bool check( const vector_type& v ) const;

    bool parallel( const self_type& that ) const;
    vector_type intersection( const self_type& that ) const;

    std::ostream& output( std::ostream& os ) const;
    void gnuplot( std::ostream& gp ) const;

    bool operator<( const self_type& that ) const;
    bool operator==( const self_type& that ) const;

  private:
    /** \brief The coefficients in the constraint. */
    vector_type m_coefficient;

    /** \brief The direction of the comparison with the right hand side. */
    direction m_direction;

    /** \brief The value of the right hand side. */
    value_type m_right_hand_side;

  }; // class constraint
} // namespace moo

template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::constraint<T>& c );

#include "moo/impl/constraint.tpp"

#endif // __MOO_CONSTRAINT_HPP__
