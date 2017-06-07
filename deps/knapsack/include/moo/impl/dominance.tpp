/**
 * \file moo/impl/dominance.tpp
 * \brief Implementation of dominance related classes and functions.
 * \author Julien Jorge
 */
#include <cassert>

#define DOMINANCE_THREE_OBJECTIVES 1

/*----------------------------------------------------------------------------*/
/**
 * \brief Do a dominance test between two vectors.
 * \param a The reference vector.
 * \param b The vector to compare to.
 * \param comp Forward iterator on a sequence of moo::direction, to compare the 
 *        members.
 * \pre a.size() == b.size()
 * \pre comp points to a range of, at least, a.size() directions.
 * \return The dominance of \a a on \a b (e.g. \a a weakly dominates \a b).
 */
template<typename T, typename Iterator>
inline moo::dominance
moo::compare( const vector<T>& a, const vector<T>& b, Iterator comp )
{
  assert( a.size() == b.size() );
#if DOMINANCE_THREE_OBJECTIVES == 1
  if ( a[0] < b[0] )
    return no_dominance;
  else if ( a[1] < b[1] )
    return no_dominance;
  else if ( a[2] < b[2] )
    return no_dominance;
  else if ( a[0] == b[0] )
    {
      if ( a[1] == b[1] )
        {
          if ( a[2] == b[2] )
            return no_dominance_equality;
          else // a[2] > b[2]
            return weak_dominance;
        }
      else // a[1] > b[1]
        return weak_dominance;
    }
  else // ( a[0] > b[0] )
    if ( a[1] == b[1] )
      return weak_dominance;
    else if ( a[2] == b[2] )
      return weak_dominance;
    else // a[2] > b[2]
      return strict_dominance;
#else
  unsigned int eq(0), better(0), worst(0);

  for (unsigned int i=0; i!=a.size(); ++i, ++comp)
    if ( a[i] == b[i] )
      ++eq;
    else if ( (*comp)(a[i], b[i]) )
      ++better;
    else
      ++worst;

  dominance result = no_dominance;

  if ( better > 0 )
    {
      if ( worst == 0 )
	{
	  if ( eq == 0 )
	    result = strict_dominance;
	  else
	    result = weak_dominance;
	}
    }
  else if ( (better==0) && (worst==0) )
    result = no_dominance_equality;

  return result;
#endif
} // compare()
