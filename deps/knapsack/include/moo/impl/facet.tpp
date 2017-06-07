/**
 * \file moo/impl/facet.tpp
 * \brief Implementation of the moo::facet class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
template<typename T>
moo::facet<T>::facet()
{

} // facet::facet()

/*----------------------------------------------------------------------------*/
template<typename T>
moo::facet<T>::facet
( const constraint_type& c, const vector_type& p1, const vector_type& p2 )
  : m_constraint(c)
{
  p.push_back(p1);
  p.push_back(p2);
} // facet::facet()

/*----------------------------------------------------------------------------*/
template<typename T>
const typename moo::facet<T>::constraint_type&
moo::facet<T>::get_constraint() const
{
  return m_constraint;
} // facet::get_constraint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the facet as text in an output stream.
 * \param os The stream to write in.
 * \return os.
 */
template<typename T>
std::ostream& moo::facet<T>::output( std::ostream& os ) const
{
  return os << m_constraint << " [" << p[0] << ':' << p[1] << ']';
} // facet::output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a facet as text in an output stream.
 * \param os The stream to write in.
 * \param f The facet to output.
 * \return os.
 */
template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::facet<T>& f )
{
  return f.output(os);
} // operator<<()
