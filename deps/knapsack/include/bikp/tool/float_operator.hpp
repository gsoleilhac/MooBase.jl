/*
  Copyright Faculté des Sciences et Techniques de Nantes
  contributor:
    Julien Jorge <julien.jorge@univ-nantes.fr>

  This software is a computer program whose purpose is to exactly solve
  instances of the bi-objective unidimensional 0-1 knapsack problem.

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 
*/
/**
 * \file float_operator.hpp
 * \brief Some classes made to ease the use of epsilon values when using
 *        floating points numbers.
 * \author Julien Jorge
 */
#ifndef __BIKP_FLOAT_OPERATOR_HPP__
#define __BIKP_FLOAT_OPERATOR_HPP__

/**
 * \brief This class generates an epsilon value of a given precision.*
 * \author Julien Jorge
 */
template<typename T, unsigned int precision>
struct make_epsilon
{
  static inline const T value()
  {
    return make_epsilon<T, precision-1>::value() / T(10);
  }
};

/**
 * \brief End of the recusivity of the make_epsilon class.
 */
template<typename T>
struct make_epsilon<T, 0>
{
  static inline const T value() { return T(1); }
};

/**
 * \brief This class groups arithmetic operations on floating point numbers.
 * \author Julien Jorge
 */
template<typename T, unsigned int precision>
class float_operator
{
public:
  typedef T value_type;

  static inline bool less( const value_type& a, const value_type& b )
  {
    return a < (b - get_precision());
  }

  static inline bool less_equal( const value_type& a, const value_type& b )
  {
    return a <= (b + get_precision());
  }

  static inline bool greater( const value_type& a, const value_type& b )
  {
    return less(b, a);
  }

  static inline bool greater_equal( const value_type& a, const value_type& b )
  {
    return less_equal(b, a);
  }

  static inline bool equal( const value_type& a, const value_type& b )
  {
    return less_equal(a, b) && less_equal(b, a);
  }

  static inline value_type get_precision()
  {
    return make_epsilon<value_type, precision>::value();
  }
}; // class float_operator

#endif // __BIKP_FLOAT_OPERATOR_HPP__
