/**
 * \file moo/real.hpp
 * \brief Custom precision real numbers.
 * \author Julien Jorge
 */
#ifndef __MOO_REAL_HPP__
#define __MOO_REAL_HPP__

#include <iostream>
#include <limits>
#include <cmath>
#include <cfloat>

namespace moo
{
  template<typename T>
  class real;
}

template<typename T>
std::istream& operator>>( std::istream& is, moo::real<T>& self );

template<typename T>
moo::real<T> operator-( const moo::real<T>& self );

#include <iomanip>
#include <cstring>

namespace moo
{
  /**
   * \brief This class generates an epsilon value of a given precision.
   * \author Julien Jorge
   */
  template<typename T>
  struct make_epsilon
  {

  }; // struct make_epsilon

  /**
   * \brief Epsilon for a double value.
   */
  template<>
  struct make_epsilon<double>
  {
    static inline double value( double v )
    {
      return std::abs(v) <= 1 ? 0.0000000001 : std::abs(v) * 0.0000000001;
    }
  }; // struct make_epsilon

  /**
   * \brief Epsilon for a float value.
   */
  template<>
  struct make_epsilon<float>
  {
    static inline float value( float v )
    {
      return std::abs(v) < 1 ? FLT_EPSILON : std::abs(v) * FLT_EPSILON;
    }
  }; // struct make_epsilon

  /**
   * \brief Custom precision real numbers.
   * \author Julien Jorge
   */
  template<typename T>
  class real
  {
    friend
    std::istream& ::operator>> <>( std::istream& is, real<T>& self );

    friend
    real<T> operator- <>( const real<T>& self );

  public:
    typedef T value_type;
    typedef real<T> self_type;

  public:
    real()
      : m_value(0), m_epsilon( make_epsilon<value_type>::value(m_value) )
    { }

    real( const value_type& v )
      : m_value(v), m_epsilon( make_epsilon<value_type>::value(m_value) )
    {

    }

    real( const self_type& that )
      : m_value(that.m_value), m_epsilon(that.m_epsilon)
    { }

    self_type abs() const
    {
      return self_type( std::abs(m_value) );
    }

    inline bool operator<( const self_type& that ) const
    {
      if ( that.m_value == std::numeric_limits<value_type>::infinity() )
        return m_value != std::numeric_limits<value_type>::infinity();
      else if ( that.m_value == -std::numeric_limits<value_type>::infinity() )
        return false;
      else if ( m_value == std::numeric_limits<value_type>::infinity() )
        return false;
      else if ( m_value == -std::numeric_limits<value_type>::infinity() )
        return that.m_value != -std::numeric_limits<value_type>::infinity();
      else
        return m_value < (that.m_value - std::max(m_epsilon, that.m_epsilon));
    }

    inline bool operator<=( const self_type& that ) const
    {
      return !(that < *this);
    }

    inline bool operator>( const self_type& that ) const
    {
      return that < *this;
    }

    inline bool operator>=( const self_type& that ) const
    {
      return that <= *this;
    }

    inline bool operator==( const self_type& that ) const
    {
      if ( that.m_value == std::numeric_limits<value_type>::infinity() )
        return m_value == std::numeric_limits<value_type>::infinity();
      else if ( that.m_value == -std::numeric_limits<value_type>::infinity() )
        return m_value == -std::numeric_limits<value_type>::infinity();
      else if ( m_value == that.m_value )
        return true;
      else
        return std::abs(m_value - that.m_value)
          <= std::max(m_epsilon, that.m_epsilon);
    }

    inline bool operator!=( const self_type& that ) const
    {
      return !((*this) == that);
    }

    inline self_type operator+( const self_type& that ) const
    {
      return self_type(m_value + that.m_value);
    }

    inline self_type operator-( const self_type& that ) const
    {
      return self_type(m_value - that.m_value);
    }

    inline self_type operator*( const self_type& that ) const
    {
      return self_type(m_value * that.m_value);
    }

    inline self_type operator/( const self_type& that ) const
    {
      return self_type(m_value / that.m_value);
    }

    inline self_type& operator+=( const self_type& that )
    {
      m_value += that.m_value;
      m_epsilon = make_epsilon<value_type>::value(m_value);
      return *this;
    }

    inline self_type& operator-=( const self_type& that )
    {
      m_value -= that.m_value;
      m_epsilon = make_epsilon<value_type>::value(m_value);
      return *this;
    }

    inline self_type& operator*=( const self_type& that )
    {
      m_value *= that.m_value;
      m_epsilon = make_epsilon<value_type>::value(m_value);
      return *this;
    }

    inline self_type& operator/=( const self_type& that )
    {
      m_value /= that.m_value;
      m_epsilon = make_epsilon<value_type>::value(m_value);
      return *this;
    }

    std::ostream& output( std::ostream& os ) const
    {
      return os << m_value; // << '~' << m_epsilon;
    }

    template<typename U>
    operator U() const
    {
      return (U)m_value;
    }

  private:
    /** \brief The value of the number. */
    value_type m_value;

    /** \brief Precision applied to operators. */
    value_type m_epsilon;

  }; // class real
} // namespace moo

namespace std
{
  template<typename T>
  struct numeric_limits< moo::real<T> >:
    public numeric_limits<T>
  {

  }; // struct numeric_limits

  template<typename T>
  moo::real<T> abs( const moo::real<T>& v )
  {
    return v.abs();
  }
} // namespace std

// unary minus
template<typename T>
moo::real<T> operator-( const moo::real<T>& self )
{
  return moo::real<T>( -self.m_value );
} // operator-()

template<typename T>
moo::real<T> operator-( T v, const moo::real<T>& self )
{
  return moo::real<T>(v) - self;
} // operator-()

template<typename T>
std::ostream&
operator<<( std::ostream& os, const moo::real<T>& self )
{
  return self.output(os);
}

template<typename T>
std::istream& operator>>( std::istream& is, moo::real<T>& self )
{
  return is >> self.m_value;
}

#endif // __MOO_REAL_HPP__

