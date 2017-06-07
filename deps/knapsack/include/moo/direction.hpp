/**
 * \file moo/direction.hpp
 * \brief Some classes to compare values, given a direction of optimization.
 * \author Julien Jorge
 */
#ifndef __MOO_DIRECTION_HPP__
#define __MOO_DIRECTION_HPP__

#include <moo/dominance.hpp>

namespace moo
{
  /**
   * \brief Base class for comparisons.
   * \author Julien Jorge
   */
  template<typename T>
  struct direction
  {
    /**
     * \brief Tell if a data \a a is better that \a b, according to the
     *        direction of the optimization.
     * \param a Reference data.
     * \param b The data to compare to.
     */
    virtual bool operator()( const T& a, const T& b ) const = 0;
  }; // struct direction

  /**
   * \brief Comparison of datas in the maximization case.
   * \author Julien Jorge
   */
  template<typename T>
  struct maximize
  {
  public:
    typedef T value_type;

  public:
    /**
     * \brief Tell if a data \a a is better that \a b (ie. a > b).
     * \param a Reference data.
     * \param b The data to compare to.
     */
    bool operator()( const value_type& a, const value_type& b ) const
    {
      return a > b;
    }
  }; // struct maximize

#if 0
  // not supported by pareto set

  /**
   * \brief Comparison of datas in the minimization case.
   * \author Julien Jorge
   */
  template<typename T>
  struct minimize
  {
  public:
    typedef T value_type;

  public:
    /**
     * \brief Tell if a data \a a is better that \a b (ie. a < b).
     * \param a Reference data.
     * \param b The data to compare to.
     */
    bool operator()( const value_type& a, const value_type& b ) const
    {
      return a < b;
    }
  }; // struct minimize
#endif

  /**
   * \brief Base class for comparison of vectors where the optimization is in
   *        one direction.
   * \author Julien Jorge
   */
  template<typename Direction>
  struct compare_unidirectional
  {
  public:
    typedef typename Direction::value_type value_type;
    typedef vector<value_type> vector_type;

  private:
    /**
     * \brief Fake forward iterator that always points on an instance of
     *        Direction.
     */
    struct static_iterator
    {
    public:
      static_iterator& operator++() { return *this; }
      const Direction& operator*() const { return m_dir; }

    private:
      const Direction m_dir;

    }; // struct static_iterator

  public:
    dominance operator()( const vector_type& a, const vector_type& b ) const
    {
      return compare( a, b, static_iterator() );
    }

  }; // struct compare_unidirectional

  template<typename T>
  class compare_unidirectional_max:
    public compare_unidirectional< maximize<T> >
  { };

#if 0
  // not supported by pareto set
  template<typename T>
  class compare_unidirectional_min:
    public compare_unidirectional< minimize<T> >
  { };
#endif

} // namespace moo

#endif // __MOO_DIRECTION_HPP__
