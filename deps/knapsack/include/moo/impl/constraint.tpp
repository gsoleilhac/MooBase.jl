/**
 * \file moo/impl/constraint.tpp
 * \brief Implementation of the moo::contraint class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
template<typename T>
moo::constraint<T>::constraint()
{
  
} // constraint::constraint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The coefficients of the expression in the constraint.
 * \param d The direction 
 * \param rhs The value of the right hand side.
 */
template<typename T>
moo::constraint<T>::constraint
( const vector_type& c, direction d, value_type rhs )
  : m_coefficient(c), m_direction(d), m_right_hand_side(rhs)
{
  
} // constraint::constraint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coefficients of this constraint.
 */
template<typename T>
const typename moo::constraint<T>::vector_type&
moo::constraint<T>::get_coefficient() const
{
  return m_coefficient;
} // constraint::get_coefficient()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the right hand side of this constraint.
 */
template<typename T>
const typename moo::constraint<T>::value_type&
moo::constraint<T>::get_right_hand_side() const
{
  return m_right_hand_side;
} // constraint::get_right_hand_side()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the constraint is satisfied for a given vector.
 * \param v The vector to check.
 */
template<typename T>
bool moo::constraint<T>::check( const vector_type& v ) const
{
  value_type eval = v.scalar_product( m_coefficient );
  bool result = false;

  switch(m_direction)
    {
    case less:          result = eval <  m_right_hand_side; break;
    case less_equal:    result = eval <= m_right_hand_side; break;
    case equal:         result = eval == m_right_hand_side; break;
    case greater:       result = eval >  m_right_hand_side; break;
    case greater_equal: result = eval >= m_right_hand_side; break;
    }

  return result;
} // constraint::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this constraint is parallel to an other one.
 * \param that The other one.
 */
template<typename T>
bool moo::constraint<T>::parallel( const self_type& that ) const
{
  return m_coefficient[0] * that.m_coefficient[1] ==
    that.m_coefficient[0] * m_coefficient[1];
} // constraint::parallel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the intersection of two constraints.
 * \param that The ohter one.
 */
template<typename T>
typename moo::constraint<T>::vector_type
moo::constraint<T>::intersection( const self_type& that ) const
{
  assert( !parallel(that) );

  vector_type result( m_coefficient.size(), 0 );

  result[0] = ( m_coefficient[1] * that.m_right_hand_side
		- that.m_coefficient[1] * m_right_hand_side )
    / ( m_coefficient[1] * that.m_coefficient[0]
	- that.m_coefficient[1] * m_coefficient[0] );

  if ( m_coefficient[1] != 0 )
    result[1] =
      (m_right_hand_side - m_coefficient[0] * result[0]) / m_coefficient[1];
  else
    result[1] = ( that.m_right_hand_side - that.m_coefficient[0] * result[0])
      / that.m_coefficient[1];

  return result;
} // constraint::intersection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the constraint as text in an output stream.
 * \param os The stream to write in.
 * \return os.
 */
template<typename T>
std::ostream& moo::constraint<T>::output( std::ostream& os ) const
{
  value_type v(m_coefficient[0]);
  os << (double)v << "x_{1} ";

  for (unsigned int i=1; i!=m_coefficient.size(); ++i)
    os << "+ " << (double)m_coefficient[i] << "x_{" << (i+1) << "} ";

  switch(m_direction)
    {
    case less:          os << "< "; break;
    case less_equal:    os << "<="; break;
    case equal:         os << "= "; break;
    case greater:       os << "> "; break;
    case greater_equal: os << ">="; break;
    }

  return os << ' ' << (double)m_right_hand_side;
} // constraint::output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the constraint set in a format understood by gnuplot.
 * \param gp The stream in which we write gnuplot instructions.
 */
template<typename T>
void moo::constraint<T>::gnuplot( std::ostream& gp ) const
{
  if ( m_coefficient[1] == 0 )
    gp << m_right_hand_side << " / " << m_coefficient[0] << " title \""
       << *this << '"';
  else
    gp << '(' << m_right_hand_side << " - " << m_coefficient[0] << " * x)"
       << " / " << m_coefficient[1] << " title \"" << *this << '"';

  gp << " with lines 1";
} // constraint::gnuplot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two constraints in lexicographical order.
 * \param that The constraint to compare to.
 */
template<typename T>
bool moo::constraint<T>::operator<( const self_type& that ) const
{
  if ( m_coefficient == that.m_coefficient)
    {
      if ( m_direction == that.m_direction )
	return m_right_hand_side < that.m_right_hand_side;
      else
	return m_direction < that.m_direction;
    }
  else
    return m_coefficient < that.m_coefficient;
} // constraint::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The constraint to compare to.
 */
template<typename T>
bool moo::constraint<T>::operator==( const self_type& that ) const
{
  return ( m_coefficient == that.m_coefficient)
    && ( m_direction == that.m_direction )
    && ( m_right_hand_side == that.m_right_hand_side );
} // constraint::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a constraint as text in an output stream.
 * \param os The stream to write in.
 * \param c The constraint to output.
 * \return os.
 */
template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::constraint<T>& c )
{
  return c.output(os);
} // operator<<()
