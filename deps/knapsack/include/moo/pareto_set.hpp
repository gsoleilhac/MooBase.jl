/**
 * \file moo/pareto_set.hpp
 * \brief A class for managing a set of non dominated vectors.
 * \author Julien Jorge
 */
#ifndef __MOO_PARETO_SET_HPP__
#define __MOO_PARETO_SET_HPP__

#include <set>
#include <moo/dominance.hpp>

namespace moo
{
  /**
   * \brief A class for managing a set of non dominated vectors.
   * \author Julien Jorge
   */
  template<typename Compare>
  class pareto_set:
    private std::set<typename Compare::vector_type>
  {
  private:
    typedef std::set<typename Compare::vector_type> super;
    typedef Compare compare_type;
    typedef typename Compare::vector_type value_type;

  public:
    typedef typename super::const_iterator const_iterator;

  public:
    int insert( const value_type& v );
    template<typename Iterator>
    void insert( Iterator first, Iterator last );

    void erase( const value_type& v );

    bool is_dominated( const value_type& v ) const;
    bool is_dominated_eq( const value_type& v ) const;
    dominance compare_to_all( const value_type& v ) const;

    unsigned int size() const;

    const_iterator begin() const;
    const_iterator end() const;

  private:
    static const compare_type s_compare;

  }; // class pareto_set
} // namespace moo

#include <moo/impl/pareto_set.tpp>

#endif // __MOO_PARETO_SET_HPP__
