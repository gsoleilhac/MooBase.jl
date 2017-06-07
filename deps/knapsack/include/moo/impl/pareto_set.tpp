/**
 * \file moo/impl/pareto_set.tpp
 * \brief Implementation of the moo::pareto_set class.
 * \author Julien Jorge
 */
#include <moo/dominance.hpp>

/*----------------------------------------------------------------------------*/
template<typename Compare>
const Compare moo::pareto_set<Compare>::s_compare = Compare();

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a vector in the set.
 * \param v The vector to add.
 * \return The number of points removed, or -1 if v is known, or -2 if v is
 *         dominated.
 */
template<typename Compare>
int moo::pareto_set<Compare>::insert( const value_type& v )
{
  int result(0);

  const typename super::const_iterator ref_it(super::lower_bound(v));
  const typename super::const_iterator eit(super::end());

  if ( (ref_it!=eit) && (*ref_it == v) )
    result = -1;
  else
    {
      typename super::const_iterator it(super::begin());
      
      while ( it!=ref_it )
        if ( s_compare(v, *it) & general_dominance )
          {
            typename super::iterator tmp=it;
            ++tmp;
            super::erase(it);
            it=tmp;
            ++result;
          }
        else
          ++it;

      // (it == ref_it)
      for ( ; (result==0) && (it!=eit); ++it )
        if ( s_compare(*it, v) & general_dominance )
          result = -2;

      if ( result >= 0 )
        super::insert(ref_it, v);
    }

  return result;
} // pareto_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add some vectors.
 * \param first Iterator in the first vector to add.
 * \param last Iterator just past the last vector to add.
 */
template<typename Compare>
template<typename Iterator>
void moo::pareto_set<Compare>::insert( Iterator first, Iterator last )
{
  for( ; first!=last; ++first )
    insert(*first);
} // pareto_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a vector.
 * \param v The vector to remove.
 */
template<typename Compare>
void moo::pareto_set<Compare>::erase( const value_type& v )
{
  bool found = false;
  typename super::iterator it;

  for( it=super::begin(); !found && (it!=super::end()); )
    if ( *it == v )
      found = true;
    else
      ++it;

  if (found)
    super::erase(it);
} // pareto_set::erase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a vector is dominated by a vector from the set.
 * \param v The vector to check.
 */
template<typename Compare>
bool moo::pareto_set<Compare>::is_dominated( const value_type& v ) const
{
  return compare_to_all(v) & general_dominance;
} // pareto_set::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a vector is dominated by or equal to a vector from the set.
 * \param v The vector to check.
 */
template<typename Compare>
bool moo::pareto_set<Compare>::is_dominated_eq( const value_type& v ) const
{
  return compare_to_all(v) != no_dominance;
} // pareto_set::is_dominated_eq()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the dominance of all vectors from the set on a given vector.
 * \param v The vector to check.
 */
template<typename Compare>
moo::dominance
moo::pareto_set<Compare>::compare_to_all( const value_type& v ) const
{
  dominance result = no_dominance;
  typename super::const_iterator it(super::lower_bound(v));
  const typename super::const_iterator eit(super::end());

  for ( ; (result == no_dominance) && (it!=eit); ++it)
    result = s_compare(*it, v);

  return result;
} // pareto_set::compare_to_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of points in the set.
 */
template<typename Compare>
unsigned int moo::pareto_set<Compare>::size() const
{
  return super::size();
} // pareto_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the set.
 */
template<typename Compare>
typename moo::pareto_set<Compare>::const_iterator
moo::pareto_set<Compare>::begin() const
{
  return super::begin();
} // pareto_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set.
 */
template<typename Compare>
typename moo::pareto_set<Compare>::const_iterator
moo::pareto_set<Compare>::end() const
{
  return super::end();
} // pareto_set::end()
