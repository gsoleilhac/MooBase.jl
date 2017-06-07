/**
 * \file code/binary_solution.cpp
 * \brief Implementation of the moo::binary_solution class.
 * \author Julien Jorge
 */
#include <moo/binary_solution.hpp>
#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the vector.
 * \post operator[i] == bin_free for all i.
 */
moo::binary_solution::binary_solution( unsigned int n )
  : super(n, bin_free)
{ } // binary_solution::binary_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to an element.
 * \param i The index of the element.
 */
moo::bin_status moo::binary_solution::operator[]( unsigned int i ) const
{
 return status_of(i);
} // binary_solution::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to an element.
 * \param i The index of the element.
 */
moo::bin_status& moo::binary_solution::operator[]( unsigned int i )
{
  return status_of(i);
} // binary_solution::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to an element.
 * \param i The index of the element.
 */
moo::bin_status moo::binary_solution::status_of( unsigned int i ) const
{
  return super::operator[](i);
} // binary_solution::status_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Access to an element.
 * \param i The index of the element.
 */
moo::bin_status& moo::binary_solution::status_of( unsigned int i )
{
  return super::operator[](i);
} // binary_solution::status_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of variables set in the binary.
 */
unsigned int moo::binary_solution::cardinality() const
{
  unsigned int result = 0;

  for (unsigned int i=0; i!=size(); ++i)
    if ( status_of(i) == bin_set )
      ++result;

  return result;
} // binary_solution::cardinality()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the vector.
 */
unsigned int moo::binary_solution::size() const
{
 return super::size();
} // binary_solution::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator==( const binary_solution& that ) const
{
  return super(*this) == super(that);
} // binary_solution::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Not equal.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator!=( const binary_solution& that ) const
{
  return super(*this) != super(that);
} // binary_solution::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower than' operator.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator<( const binary_solution& that ) const
{
  return super(*this) < super(that);
} // binary_solution::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'lower or equal' operator.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator<=( const binary_solution& that ) const
{
  return super(*this) <= super(that);
} // binary_solution::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater than' operator.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator>( const binary_solution& that ) const
{
  return super(*this) > super(that);
} // binary_solution::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lexicographic 'greater or equal' operator.
 * \param that The vector to compare to.
 */
bool moo::binary_solution::operator>=( const binary_solution& that ) const
{
  return super(*this) >= super(that);
} // binary_solution::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a vector in an output stream.
 * \param os The stream in which we write.
 * \param v The vector to write.
 */
std::ostream& moo::operator<<( std::ostream& os, const binary_solution& sol )
{
  for (unsigned int i=0; i!=sol.size(); ++i)
    if ( sol[i] == bin_set )
      os << '1';
    else if ( sol[i] == bin_unset )
      os << '0';
    else
      os << '.';
  
  return os;
} // operator<<() [binary_solution]
