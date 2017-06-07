/**
 * \file moo/dominance.hpp
 * \brief All about dominance between vectors.
 * \author Julien Jorge
 */
#ifndef __MOO_DOMINANCE_HPP__
#define __MOO_DOMINANCE_HPP__

#include <moo/vector.hpp>

namespace moo
{
  typedef char dominance;

  static const char no_dominance          = 0;
  static const char strict_dominance      = 1;
  static const char weak_dominance        = 2;
  static const char general_dominance     = 3;
  static const char no_dominance_equality = 4;

  template<typename T, typename Iterator>
  inline dominance compare
  ( const vector<T>& a, const vector<T>& b, Iterator comp );

} // namespace moo

#include <moo/impl/dominance.tpp>

#endif // __MOO_DOMINANCE_HPP__
