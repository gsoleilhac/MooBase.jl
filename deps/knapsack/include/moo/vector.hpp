/**
 * \file moo/vector.hpp
 * \brief Base class for vectors (in a mathematical sense).
 * \author Julien Jorge
 */
#ifndef __MOO_VECTOR_HPP__
#define __MOO_VECTOR_HPP__

#include <vector>
#include <iostream>

namespace moo
{
  /**
   * \brief Base class for vectors (in a mathematical sense).
   * \author Julien Jorge
   */
  template<typename T>
  class vector
  {
  public:
    typedef T value_type;
    typedef vector<T> self_type;

  public:
    vector();
    explicit vector( unsigned int s );
    template<typename U>
    explicit vector( const vector<U>& that );

    vector( unsigned int s, value_type v );

    inline value_type& operator[]( unsigned int i );
    inline value_type operator[]( unsigned int i ) const;

    unsigned int size() const;
    bool empty() const;

    inline value_type scalar_product( const self_type& that ) const;

    inline bool operator==( const self_type& that ) const;
    inline bool operator!=( const self_type& that ) const;
    inline bool operator<( const self_type& that ) const;
    inline bool operator<=( const self_type& that ) const;
    inline bool operator>( const self_type& that ) const;
    inline bool operator>=( const self_type& that ) const;

    inline self_type& operator+=( const self_type& that );
    inline self_type& operator-=( const self_type& that );
    inline self_type& operator*=( const value_type& v );
    inline self_type& operator/=( const value_type& v );

    inline self_type operator+( const self_type& that ) const;
    inline self_type operator-( const self_type& that ) const;
    inline self_type operator*( const value_type& v ) const;
    inline self_type operator/( const value_type& v ) const;

  private:
    std::vector<value_type> m_data;

  }; // class vector
} // namespace moo

template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::vector<T>& v );

#include <moo/impl/vector.tpp>

#endif // __MOO_VECTOR_HPP__
