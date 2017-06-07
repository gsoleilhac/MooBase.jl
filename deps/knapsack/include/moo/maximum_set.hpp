/**
 * \file moo/maximum_set.hpp
 * \brief A class for managing a set of efficient solutions, with equivalent
 *        ones.
 * \author Julien Jorge
 */
#ifndef __MOO_MAXIMUM_SET_HPP__
#define __MOO_MAXIMUM_SET_HPP__

#include <list>
#include <moo/pareto_set.hpp>

namespace moo
{
  /**
   * \brief A class for managing a set of efficient solutions, with equivalent
   *        ones.
   * \author Julien Jorge
   */
  template<typename Solution>
  class maximum_set:
    private std::list<Solution>
  {
  private:
    typedef std::list<Solution> super;
    typedef Solution value_type;
    typedef typename Solution::compare_type compare_type;
    typedef typename compare_type::vector_type vector_type;

  public:
    typedef typename super::const_iterator const_iterator;
    typedef
    typename pareto_set<compare_type>::const_iterator const_image_iterator;

    typedef maximum_set<Solution> self_type;

  public:
    int insert( const value_type& v );
    template<typename Iterator>
    void insert( Iterator first, Iterator last );

    unsigned int size() const;
    bool empty() const;

    bool operator==( const self_type& that ) const;
    bool operator!=( const self_type& that ) const;

    const_iterator begin() const;
    const_iterator end() const;

    const_image_iterator image_begin() const;
    const_image_iterator image_end() const;

    bool is_dominated( const value_type& v ) const;
    bool is_dominated( const vector_type& v ) const;

  private:
    static const compare_type s_compare;

    /** \brief The image of the solutions in the objective space. */
    pareto_set<compare_type> m_points;

  }; // class maximum_set
} // namespace moo

#include <moo/impl/maximum_set.tpp>

#endif // __MOO_MAXIMUM_SET_HPP__
