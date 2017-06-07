/**
 * \file moo/bound_set.hpp
 * \brief A class for managing a domain of non dominated vectors.
 * \author Julien Jorge
 */
#ifndef __MOO_BOUND_SET_HPP__
#define __MOO_BOUND_SET_HPP__

#include <set>

namespace moo
{
  /**
   * \brief A class for managing a domain of non dominated vectors.
   * \author Julien Jorge
   */
  template<typename Compare>
  class bound_set:
    private std::set<typename Compare::vector_type>
  {
  private:
    typedef std::set<typename Compare::vector_type> super;
    typedef Compare compare_type;
    typedef typename Compare::vector_type value_type;

  public:
    typedef typename super::const_iterator const_iterator;

  public:
    bound_set( const value_type& v );

    void erase( const value_type& v );
    void insert( const value_type& v );
    bool reduce( const value_type& v );
    bool reduce( const value_type& v, const value_type& ref );
    template<typename Iterator>
    void reduce( Iterator first, Iterator last );

    unsigned int size() const;
    bool empty() const;

    const_iterator begin() const;
    const_iterator end() const;

    bool dominates_any( const value_type& v ) const;

  private:
    bool reduce
    ( const value_type& v, const value_type& ref, bool& ref_removed );

  private:
    static const compare_type s_compare;

  }; // class bound_set
} // namespace moo

#include <moo/impl/bound_set.tpp>

#endif // __MOO_BOUND_SET_HPP__
