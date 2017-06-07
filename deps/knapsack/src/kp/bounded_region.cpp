/**
 * \file bouded_region.cpp
 * \brief implementation of the kp::bounded_region class.
 * \author Julien Jorge
 */
#include "kp/bounded_region.hpp"

#include <map>
#include <limits>
#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param direction The direction in which we optmise.
 * \param lower_bound The lower bound to use.
 */
kp::bounded_region::bounded_region
( const cost_vector& direction, const moo::real_type& lower_bound )
  : m_direction(direction), m_nadir( cost_vector(m_direction.size(), 0) ),
    m_bound_set(m_nadir), m_lower_bound(lower_bound)
{

} // bounded_region::bounded_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param direction The direction in which we optmise.
 * \param nadir The nadir point of the region.
 * \param lower_bound The lower bound to use.
 */
kp::bounded_region::bounded_region
( const cost_vector& direction, const cost_vector& nadir,
  const moo::real_type& lower_bound )
  : m_direction(direction), m_nadir(nadir), m_bound_set(m_nadir),
    m_lower_bound(lower_bound)
{
  update_lower_bound();
} // bounded_region::bounded_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a point in the sets and adjust the bounds.
 * \param v The point to add.
 */
bool kp::bounded_region::reduce( const cost_vector& v )
{
  assert( contains(v) );

  bool result = false;

  if ( m_non_dominated.insert(v) >= 0 )
    if ( m_bound_set.reduce(v) )
      {
        update_lower_bound();
        result= true;
      }

  return result;
} // bounded_region::reduce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a point is dominated by one of the known points in the region.
 * \param v The point to check.
 */
bool kp::bounded_region::is_dominated( const cost_vector& v ) const
{
  return m_non_dominated.is_dominated(v);
} // bounded_region::is_dominated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a point is dominated or equal to one of the known points in
 *        the region.
 * \param v The point to check.
 */
bool kp::bounded_region::is_dominated_eq( const cost_vector& v ) const
{
  return m_non_dominated.is_dominated_eq(v);
} // bounded_region::is_dominated_eq()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region contains a given point.
 * \param v The point to check.
 */
bool kp::bounded_region::contains( const cost_vector& v ) const
{
  if ( v.scalar_product(m_direction) < m_lower_bound )
    return false;
  else
    return !is_dominated(v) && m_bound_set.dominates_any(v);
} // bounded_region::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the direction in which we optimise.
 */
const kp::cost_vector& kp::bounded_region::get_direction() const
{
  return m_direction;
} // bounded_region::get_direction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the lower bound.
 */
const moo::real_type& kp::bounded_region::get_lower_bound() const
{
  return m_lower_bound;
} // bounded_region::get_lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the global nadir point.
 */
const kp::cost_vector& kp::bounded_region::get_nadir() const
{
  return m_nadir;
} // bounded_region::get_nadir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get and iterator on the beginning of the set of non dominated points.
 */
kp::pareto_set::const_iterator kp::bounded_region::pareto_begin() const
{
  return m_non_dominated.begin();
} // bounded_region::pareto_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get and iterator past the end of the set of non dominated points.
 */
kp::pareto_set::const_iterator kp::bounded_region::pareto_end() const
{
  return m_non_dominated.end();
} // bounded_region::pareto_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get and iterator on the beginning of the nadir-like points.
 */
kp::bound_set::const_iterator kp::bounded_region::bound_set_begin() const
{
  return m_bound_set.begin();
} // bounded_region::bound_set_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get and iterator past the end of the nadir-like points.
 */
kp::bound_set::const_iterator kp::bounded_region::bound_set_end() const
{
  return m_bound_set.end();
} // bounded_region::bound_set_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 */
kp::bounded_region& kp::bounded_region::operator=( const bounded_region& that )
{
  assert( m_direction == that.m_direction );
  assert( m_nadir == that.m_nadir );

  m_non_dominated = that.m_non_dominated;
  m_bound_set = that.m_bound_set;
  m_lower_bound = that.m_lower_bound;

  return *this;
} // bounded_region::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a gnuplot file of the region.
 */
void kp::bounded_region::gnuplot() const
{
  static std::size_t index(0);
  ++index;

  std::ostringstream oss;
  oss << index << '_' << m_nadir[0] << '_' << m_nadir[1] << '_' << m_nadir[2];

  std::ofstream f_gnuplot((oss.str() + ".gp").c_str());

  f_gnuplot << "#!gnuplot\n"
            << "set term png\n"
            << "set output \"" << oss.str() << ".png\"\n"
            << "\n"
            << "splot \""
            << oss.str() << "-nadirs.data\" title \"nadirs\", \\\n"
            << "      \""
            << oss.str() << "-points.data\" title \"points\"\n";

  std::ofstream f_nadirs((oss.str() + "-nadirs.data").c_str());
  std::ofstream f_points((oss.str() + "-points.data").c_str());

  for ( bound_set::const_iterator it=m_bound_set.begin();
        it!=m_bound_set.end(); ++it )
    f_nadirs << *it << std::endl;

  for ( pareto_set::const_iterator it=m_non_dominated.begin();
        it!=m_non_dominated.end(); ++it )
    f_points << *it << std::endl;
} // bounded_region::gnuplot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the value of the lower bound.
 */
void kp::bounded_region::update_lower_bound()
{
  moo::real_type result = std::numeric_limits<moo::real_type>::max();
  bound_set::const_iterator it;
  moo::real_type val;

  for ( std::size_t i=0;
        (result>m_lower_bound) && (i!=m_direction.size()); ++i )
    {
      val = lower_bound_on_face(i);
      
      if ( val < result )
        result = val;
    }

  cost_vector delta( m_direction.size(), 1 );

  for ( it=m_bound_set.begin();
        (result>m_lower_bound) && (it!=m_bound_set.end()); ++it )
    {
      val = (*it + delta).scalar_product(m_direction);

      if ( val < result )
        result = val;
    }

  if ( result > m_lower_bound )
    m_lower_bound = result;
} // bounded_region::update_lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the lower bound for all points having the same value on a given
 *        objective.
 * \param j The objective on which the points have the same value.
 */
moo::real_type kp::bounded_region::lower_bound_on_face( std::size_t j ) const
{
  moo::real_type result = std::numeric_limits<moo::real_type>::max();
  bound_set::const_iterator it;

  std::map< moo::real_type, std::list<cost_vector> > groups;
  std::map< moo::real_type, std::list<cost_vector> >::iterator itg;
  std::size_t p0;
  std::size_t p1;

  if ( j==0 )
    {
      p0 = 1;
      p1 = 2;
    }
  else if ( j==1 )
    {
      p0 = 0;
      p1 = 2;
    }
  else // j == 2
    {
      p0 = 0;
      p1 = 1;
    }

  // this loop is incorrect when we allow points out of the search area (see
  // the contains() method)
  pareto_set::const_iterator itp(m_non_dominated.begin());

  for( ; itp!=m_non_dominated.end(); ++itp )
    groups[(*itp)[j]].push_back(*itp);

  for ( itg=groups.begin();
        (result>m_lower_bound) && (itg!=groups.end()); ++itg )
    {
      itg->second.sort();
      std::list<cost_vector>::const_iterator first(itg->second.begin());
      std::list<cost_vector>::const_iterator second(first);
      ++second;
      cost_vector tmp(*first);

      moo::real_type val;
      val = (*first).scalar_product(m_direction);

      if ( val < result )
        result = val;

      for ( ; second!=itg->second.end(); ++first, ++second )
        {
          if ( (*first)[p1] < (*second)[p1] )
            std::cout << "kp::bounded_region::lower_bound_on_face(): OOOH "
                      << (*first)[p1] << ' '<< (*second)[p1] << std::endl;

          val = (*first).scalar_product(m_direction);

          if ( val < result )
            result = val;

          tmp[p0] = (*first)[p0] + 1;
          tmp[p1] = (*second)[p1] + 1;

          val = tmp.scalar_product(m_direction);

          if ( val < result )
            result = val;
              
        }

      val = (*first).scalar_product(m_direction);

      if ( val < result )
        result = val;
    }

  return result;
} // bounded_region::lower_bound_on_face()
