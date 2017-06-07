/**
 * \file moo/impl/maximum_set.tpp
 * \brief Implementation of the moo::maximum_set class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
template<typename Solution>
const typename moo::maximum_set<Solution>::compare_type
moo::maximum_set<Solution>::s_compare =
  typename moo::maximum_set<Solution>::compare_type();

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set and remove dominated ones.
 * \param v The solution to add.
 * \return 0 if \a v was not added, -1 if \a v was added but its image
 *         corresponds to an existing value, 1 otherwise.
 */
template<typename Solution>
int moo::maximum_set<Solution>::insert( const value_type& v )
{
  int removed = m_points.insert(v.image());
  int result(0);

  if ( removed == 0 )     // unknown point and not dominated
    {
      this->push_front(v);
      result = 1;
    }
  else if ( removed > 0 ) // unknown point and dominating
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
      result = 1;
    }
  else if ( removed == -1 ) // known point, so not dominating
    {
      typename super::iterator it = super::begin();
      bool known = false;

      while ( !known && (it!=super::end()) )
	if ( *it == v )
	  known = true;
	else
	  ++it;

      if (!known)
        {
          result = -1;
          this->push_front(v);
        }
    }

  return result;
} // maximum_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a range of solutions in the set and remove dominated ones.
 * \param first Iterator on the solution to insert.
 * \param last Iterator on the first solution not to insert.
 */
template<typename Solution>
template<typename Iterator>
void moo::maximum_set<Solution>::insert( Iterator first, Iterator last )
{
  for ( ; first!=last; ++first )
    insert(*first);
} // maximum_set::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of points in the set.
 */
template<typename Solution>
unsigned int moo::maximum_set<Solution>::size() const
{
  return super::size();
} // maximum_set::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set is empty.
 */
template<typename Solution>
bool moo::maximum_set<Solution>::empty() const
{
  return super::empty();
} // maximum_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sets contains the same solutions.
 * \param that The set to compare to.
 */
template<typename Solution>
bool moo::maximum_set<Solution>::operator==( const self_type& that ) const
{
  return static_cast<super>(*this) == static_cast<super>(that);
} // maximum_set::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sets contains different solutions.
 * \param that The set to compare to.
 */
template<typename Solution>
bool moo::maximum_set<Solution>::operator!=( const self_type& that ) const
{
  return static_cast<super>(*this) != static_cast<super>(that);
} // maximum_set::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the set.
 */
template<typename Solution>
typename moo::maximum_set<Solution>::const_iterator
moo::maximum_set<Solution>::begin() const
{
  return super::begin();
} // maximum_set::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set.
 */
template<typename Solution>
typename moo::maximum_set<Solution>::const_iterator
moo::maximum_set<Solution>::end() const
{
  return super::end();
} // maximum_set::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the images of the solutions in the
 *        set.
 */
template<typename Solution>
typename moo::maximum_set<Solution>::const_image_iterator
moo::maximum_set<Solution>::image_begin() const
{
  return m_points.begin();
} // maximum_set::image_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the images of the solutions in the
 *        set.
 */
template<typename Solution>
typename moo::maximum_set<Solution>::const_image_iterator
moo::maximum_set<Solution>::image_end() const
{
  return m_points.end();
} // maximum_set::image_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set contains a solution better than a given one.
 * \param v The solution to check.
 */
template<typename Solution>
bool moo::maximum_set<Solution>::is_dominated( const value_type& v ) const
{
  return m_points.is_dominated( v.image() );
} // maximum_set::is_dominated() [value_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the set contains a solution whose image dominates a given
 *        point.
 * \param v The point to check.
 */
template<typename Solution>
bool moo::maximum_set<Solution>::is_dominated( const vector_type& v ) const
{
  return m_points.is_dominated(v);
} // maximum_set::is_dominated() [vector_type]
