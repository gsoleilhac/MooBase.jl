/**
 * \file moo/impl/vector.tpp
 * \brief Implementation of the moo::vector class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template<typename T>
moo::vector<T>::vector()
{

} // vector::vector()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The size of the vector.
 */
template<typename T>
moo::vector<T>::vector( unsigned int s )
  : m_data(s)
{

} // vector::vector()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The vector to copy from.
 */
template<typename T>
template<typename U>
moo::vector<T>::vector( const vector<U>& that )
  : m_data(that.m_data)
{

} // vector::vector()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor
 * \param s The size of the vector.
 * \param v The initial values of the elements.
 */
template<typename T>
moo::vector<T>::vector( unsigned int s, value_type v )
  : m_data(s, v)
{

} // vector::vector()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to a element.
 * \param param i Index of the element.
 */
template<typename T>
typename moo::vector<T>::value_type&
moo::vector<T>::operator[]( unsigned int i )
{
  return m_data[i];
} // vector::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to a element.
 * \param param i Index of the element.
 */
template<typename T>
typename moo::vector<T>::value_type
moo::vector<T>::operator[]( unsigned int i ) const
{
  return m_data[i];
} // vector::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the vector.
 */
template<typename T>
unsigned int moo::vector<T>::size() const
{
  return m_data.size();
} // vector::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the vector does not contains data.
 */
template<typename T>
bool moo::vector<T>::empty() const
{
  return m_data.empty();
} // vector::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the scalar product of two vectors.
 * \param that The other vector.
 */
template<typename T>
typename moo::vector<T>::value_type
moo::vector<T>::scalar_product( const self_type& that ) const
{
  unsigned int i=0;
  value_type result = (*this)[i] * that[i];

  for ( ++i; i!=size(); ++i )
    result += (*this)[i] * that[i];

  return result;
} // vector::scalar_product()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator==( const self_type& that ) const
{
  return m_data == that.m_data;
} // vector::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Not equal.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator!=( const self_type& that ) const
{
  return m_data != that.m_data;
} // vector::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower than' operator.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator<( const self_type& that ) const
{
  return m_data < that.m_data;
} // vector::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower or equal' operator.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator<=( const self_type& that ) const
{
  return m_data <= that.m_data;
} // vector::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater than' operator.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator>( const self_type& that ) const
{
  return m_data > that.m_data;
} // vector::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater or equal' operator.
 * \param that The vector to compare to.
 */
template<typename T>
bool moo::vector<T>::operator>=( const self_type& that ) const
{
  return m_data >= that.m_data;
} // vector::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Addition and assignment.
 * \param that The vector to add.
 */
template<typename T>
typename moo::vector<T>::self_type&
moo::vector<T>::operator+=( const self_type& that )
{
  for (unsigned int i=0; i!=size(); ++i)
    m_data[i] += that.m_data[i];

  return *this;
} // vector::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtraction and assignment.
 * \param that The vector to subtract.
 */
template<typename T>
typename moo::vector<T>::self_type&
moo::vector<T>::operator-=( const self_type& that )
{
  for (unsigned int i=0; i!=size(); ++i)
    m_data[i] -= that.m_data[i];

  return *this;
} // vector::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiplication and assignment.
 * \param v The value to multiply with.
 */
template<typename T>
typename moo::vector<T>::self_type&
moo::vector<T>::operator*=( const value_type& v )
{
  for (unsigned int i=0; i!=size(); ++i)
    m_data[i] *= v;

  return *this;
} // vector::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Division and assignment.
 * \param v The value to divide with.
 */
template<typename T>
typename moo::vector<T>::self_type&
moo::vector<T>::operator/=( const value_type& v )
{
  for (unsigned int i=0; i!=size(); ++i)
    m_data[i] /= v;

  return *this;
} // vector::operator/=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Addition.
 * \param that The vector to add.
 */
template<typename T>
typename moo::vector<T>::self_type
moo::vector<T>::operator+( const self_type& that ) const
{
  return self_type(*this) += that;
} // vector::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtraction.
 * \param that The vector to subtract.
 */
template<typename T>
typename moo::vector<T>::self_type
moo::vector<T>::operator-( const self_type& that ) const
{
  return self_type(*this) -= that;
} // vector::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiplication.
 * \param v The value to multiply with.
 */
template<typename T>
typename moo::vector<T>::self_type
moo::vector<T>::operator*( const value_type& v ) const
{
  return self_type(*this) *= v;
} // vector::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Division.
 * \param v The value to divide with.
 */
template<typename T>
typename moo::vector<T>::self_type
moo::vector<T>::operator/( const value_type& v ) const
{
  return self_type(*this) /= v;
} // vector::operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a vector in an output stream.
 * \param os The stream in which we write.
 * \param v The vector to write.
 */
// template<typename T>
// std::ostream& operator<<( std::ostream& os, const moo::vector<T>& v )
// {
//   os << v[0];

//   for (unsigned int i=1; i!=v.size(); ++i)
//     os << '\t' << v[i];

//   return os;
// } // operator<<() [vector]
