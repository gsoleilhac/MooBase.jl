/**
 * \file moo/impl/bound_set.tpp
 * \brief Implementation of the moo::bound_set class.
 * \author Julien Jorge
 */
#include <moo/dominance.hpp>

#include <moo/pareto_set.hpp>
#include <list>

/*----------------------------------------------------------------------------*/
template<typename Compare>
const Compare moo::bound_set<Compare>::s_compare = Compare();

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the domain given a vector.
 * \param v An initial point describing the domain. This point must be chosen
 *        such that being dominated by any interesting point.
 */
template<typename Compare>
moo::bound_set<Compare>::bound_set( const value_type& v )
{
  super::insert(v);
} // bound_set::bound_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase a (nadir-like) point from the set.
 * \param v The point to erase.
 */
template<typename Compare>
void moo::bound_set<Compare>::erase( const value_type& v )
{
  super::erase(v);
} // bound_set::erase()

/*----------------------------------------------------------------------------*/
/**
 * \brief insert a (nadir-like) point in the set.
 * \param v The point to insert.
 */
template<typename Compare>
void moo::bound_set<Compare>::insert( const value_type& v )
{
  pareto_set<Compare> p;
  p.insert(begin(), end());
  p.insert(v);
  super::clear();
  super::insert(p.begin(), p.end());
} // bound_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the domain given a vector.
 * \param v The vector used for reduction.
 * \return true If the set has been modified.
 */
template<typename Compare>
bool moo::bound_set<Compare>::reduce( const value_type& v )
{
  bool dummy(false);
  return reduce(v, *begin(), dummy);
} // bound_set::reduce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the domain given a vector.
 * \param v The vector used for reduction.
 * \param ref A vector to look for in the set.
 * \return true If \a ref has been removed from the set.
 */
template<typename Compare>
bool moo::bound_set<Compare>::reduce
( const value_type& v, const value_type& ref )
{
  bool result(false);
  reduce(v, ref, result);
  return result;
} // bound_set::reduce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the domain given some vectors.
 * \param first Iterator in the first vector used for reduction.
 * \param last Iterator just past the last vector used for reduction.
 */
template<typename Compare>
template<typename Iterator>
void moo::bound_set<Compare>::reduce( Iterator first, Iterator last )
{
  for( ; first!=last; ++first )
    reduce(*first);
} // bound_set::reduce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of points in the set.
 */
template<typename Compare>
unsigned int moo::bound_set<Compare>::size() const
{
  return super::size();
} // bound_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set is empty.
 */
template<typename Compare>
bool moo::bound_set<Compare>::empty() const
{
  return super::empty();
} // bound_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the set.
 */
template<typename Compare>
typename moo::bound_set<Compare>::const_iterator
moo::bound_set<Compare>::begin() const
{
  return super::begin();
} // bound_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set.
 */
template<typename Compare>
typename moo::bound_set<Compare>::const_iterator
moo::bound_set<Compare>::end() const
{
  return super::end();
} // bound_set::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given point dominates a point from the set.
 * \param v The point to check.
 */
template<typename Compare>
bool moo::bound_set<Compare>::dominates_any( const value_type& v ) const
{
  bool result = false;

  const typename super::const_iterator eit( super::lower_bound(v) );
  typename super::const_iterator it;

  for ( it=super::begin(); !result && (it!=eit); ++it )
    result = ((s_compare(v, *it) & general_dominance) != no_dominance);

  return result;
} // bound_set::dominates_any()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reduce the domain given a vector.
 * \param v The vector used for reduction.
 * \param ref A vector to look for in the set.
 * \param ref_removed (out) Tell if \a ref has been removed from the set.
 * \return true If the set has been modified.
 */
template<typename Compare>
bool moo::bound_set<Compare>::reduce
( const value_type& v, const value_type& ref, bool& ref_removed )
{
  std::list<value_type> removed;
  const typename super::const_iterator reit(super::lower_bound(v));

  ref_removed = false;

  for ( typename super::iterator it=super::begin(); it!=reit; )
    if ( s_compare(v, *it) & strict_dominance )
      {
        if (*it == ref)
          ref_removed = true;

        removed.push_front(*it);
        typename super::iterator tmp(it);
        ++it;
        super::erase(tmp);
      }
    else
      ++it;

  typename std::list<value_type>::const_iterator it;
  std::list<value_type> candidates;

  for (it=removed.begin(); it!=removed.end(); ++it)
    for (unsigned int i=0; i!=v.size(); ++i)
      {
        value_type new_point(*it);
        new_point[i] = v[i];

        bool do_insertion(true);
        const typename super::const_iterator eit(super::lower_bound(new_point));
        typename super::const_iterator itb;

        for ( itb=super::begin(); do_insertion && (itb!=eit); ++itb)
          if ( s_compare(new_point, *itb) & general_dominance )
            do_insertion = false;

        if ( do_insertion )
          candidates.push_back(new_point);
      }

  while ( !candidates.empty() )
    {
      value_type p(candidates.front());
      bool stop(false);
      candidates.pop_front();
      typename std::list<value_type>::iterator itc;

      for ( itc=candidates.begin(); !stop && (itc!=candidates.end()); )
        if ( s_compare(p, *itc) & general_dominance )
          stop = true;
        else if ( s_compare(*itc, p) & general_dominance )
          {
            typename std::list<value_type>::iterator tmp(itc);
            ++itc;
            candidates.erase(tmp);
          }
        else
          ++itc;

      if ( !stop )
        super::insert(p);
    }

#ifndef NDEBUG
  pareto_set<Compare> p;
  p.insert(begin(), end());
  assert( p.size() == size() );
#endif

  return !removed.empty();
} // bound_set::reduce()
