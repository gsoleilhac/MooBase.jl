/**
 * \file moo/impl/minimum_set.tpp
 * \brief Implementation of the moo::minimum_set class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
template<typename Solution>
const typename moo::minimum_set<Solution>::compare_type
moo::minimum_set<Solution>::s_compare =
  typename moo::minimum_set<Solution>::compare_type();

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set and remove dominated ones.
 * \param v The solution to add.
 */
template<typename Solution>
void moo::minimum_set<Solution>::insert( const value_type& v )
{
  if ( !m_points.is_dominated_eq(v.image()) )
    {
      typename super::iterator it = super::begin();

      while ( it!=super::end() )
	if ( s_compare(v.image(), it->image()) & moo::general_dominance )
	  {
	    typename super::iterator tmp=it;
	    ++tmp;
	    this->erase(it);
	    it=tmp;
	  }
	else
	  ++it;

      this->push_front(v);
      m_points.insert(v.image());
    }
} // minimum_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add some solutions.
 * \param first Iterator in the first solution to add.
 * \param last Iterator just past the last solution to add.
 */
template<typename Solution>
template<typename Iterator>
void moo::minimum_set<Solution>::insert( Iterator first, Iterator last )
{
  for( ; first!=last; ++first )
    insert(*first);
} // minimum_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of points in the set.
 */
template<typename Solution>
unsigned int moo::minimum_set<Solution>::size() const
{
  return super::size();
} // minimum_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set is empty.
 */
template<typename Solution>
bool moo::minimum_set<Solution>::empty() const
{
  return super::empty();
} // minimum_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sets contains the same solutions.
 * \param that The set to compare to.
 */
template<typename Solution>
bool moo::minimum_set<Solution>::operator==( const self_type& that ) const
{
  return static_cast<super>(*this) == static_cast<super>(that);
} // minimum_set::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sets contains different solutions.
 * \param that The set to compare to.
 */
template<typename Solution>
bool moo::minimum_set<Solution>::operator!=( const self_type& that ) const
{
  return static_cast<super>(*this) != static_cast<super>(that);
} // minimum_set::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the set.
 */
template<typename Solution>
typename moo::minimum_set<Solution>::const_iterator
moo::minimum_set<Solution>::begin() const
{
  return super::begin();
} // minimum_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set.
 */
template<typename Solution>
typename moo::minimum_set<Solution>::const_iterator
moo::minimum_set<Solution>::end() const
{
  return super::end();
} // minimum_set::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the images of the solutions in the
 *        set.
 */
template<typename Solution>
typename moo::minimum_set<Solution>::const_image_iterator
moo::minimum_set<Solution>::image_begin() const
{
  return m_points.begin();
} // minimum_set::image_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the images of the solutions in the
 *        set.
 */
template<typename Solution>
typename moo::minimum_set<Solution>::const_image_iterator
moo::minimum_set<Solution>::image_end() const
{
  return m_points.end();
} // minimum_set::image_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set contains a solution better than a given one.
 * \param v The solution to check.
 */
template<typename Solution>
bool moo::minimum_set<Solution>::is_dominated( const value_type& v ) const
{
  return m_points.is_dominated( v.image() );
} // minimum_set::is_dominated() [value_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set contains a solution whose image dominates a given
 *        point.
 * \param v The point to check.
 */
template<typename Solution>
bool moo::minimum_set<Solution>::is_dominated( const vector_type& v ) const
{
  return m_points.is_dominated(v);
} // minimum_set::is_dominated() [vector_type]
