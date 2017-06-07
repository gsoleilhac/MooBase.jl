/**
 * \file moo/binary_solution.hpp
 * \brief Solution vector for problems with binary variables.
 * \author Julien Jorge
 */
#ifndef __MOO_BINARY_SOLUTION_HPP__
#define __MOO_BINARY_SOLUTION_HPP__

#include <vector>
#include <iostream>

namespace moo
{
  /**
   * \brief Status of a bit in the solution.
   */
  enum bin_status
    {
      bin_free,
      bin_unset,
      bin_set
    }; // bin_status

  /**
   * \brief A solution of binary values.
   * \author Julien Jorge
   */
  class binary_solution: private std::vector<bin_status>
  {
  public:
    typedef std::vector<bin_status> super;

  public:
    explicit binary_solution( unsigned int n );

    bin_status operator[]( unsigned int i ) const;
    bin_status& operator[]( unsigned int i );

    bin_status status_of( unsigned int i ) const;
    bin_status& status_of( unsigned int i );

    unsigned int cardinality() const;
    unsigned int size() const;

    bool operator==( const binary_solution& that ) const;
    bool operator!=( const binary_solution& that ) const;
    bool operator<( const binary_solution& that ) const;
    bool operator<=( const binary_solution& that ) const;
    bool operator>( const binary_solution& that ) const;
    bool operator>=( const binary_solution& that ) const;

  }; // class binary_solution

  std::ostream& operator<<( std::ostream& os, const binary_solution& sol );

} // namespace moo

#endif // __MOO_BINARY_SOLUTION_HPP__
