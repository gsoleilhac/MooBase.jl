/**
 * \file types.hpp
 * \brief Definition of the integer and real number types.
 * \author Julien Jorge
 */
#ifndef __MOO_TYPES_HPP__
#define __MOO_TYPES_HPP__

#ifdef LIBMOO_USES_GMP
  #include <gmpxx.h>
  #include <limits>
#else // LIBMOO_USES_GMP
  #include <moo/real.hpp>
#endif // LIBMOO_USES_GMP

namespace moo
{
#ifdef LIBMOO_USES_GMP

  typedef mpz_class integer_type;
  typedef mpf_class real_type;

#else // LIBMOO_USES_GMP

  #ifndef __STRICT_ANSI__
    typedef unsigned long long int integer_type;
  #else // __STRICT_ANSI__
    typedef unsigned long int integer_type;
  #endif // __STRICT_ANSI__

  #ifndef MOO_REAL_PRECISION
  #define MOO_REAL_PRECISION 9
  #endif

  typedef real<double> real_type;

#endif // LIBMOO_USES_GMP
} // namespace moo

#ifdef LIBMOO_USES_GMP
namespace std
{
  template<>
  struct numeric_limits<mpf_class>:
    public numeric_limits<double>
  {

  }; // struct numeric_limits
} // namespace std
#endif // LIBMOO_USES_GMP

#endif // __MOO_TYPES_HPP__
