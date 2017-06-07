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
 * \file base_second_phase_triangles.cpp
 * \brief Implementation of the base_second_phase sub classes concerning
 *        triangles.
 * \author Julien Jorge
 */
#include "bikp/phase/base_second_phase.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
base_second_phase::triangle_set::local_point_set::local_point_set()
{

} // base_second_phase::triangle_set::local_point_set::local_point_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
base_second_phase::triangle_set::local_point_set::local_point_set
( const bi_profit& p1, const bi_profit& p2 )
  : locked(false), z1(p1), z2(p2)
{
  real_type lambda_1 = z1.profit_2 - z2.profit_2;
  real_type lambda_2 = z2.profit_1 - z1.profit_1;

  upper_bound = lambda_1 * z1.profit_1 + lambda_2 * z1.profit_2;
  lower_bound = lambda_1 * (z1.profit_1 + 1) + lambda_2 * z2.profit_2;

  real_type m = lambda_1 * z1.profit_1 + lambda_2 * (z2.profit_2 + 1);

  if (m < lower_bound)
    lower_bound = m;
} // base_second_phase::triangle_set::local_point_set::local_point_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the value of the lower bound.
 */
void
base_second_phase::triangle_set::local_point_set::compute_lower_bound()
{
  real_type lambda_1 = z1.profit_2 - z2.profit_2;
  real_type lambda_2 = z2.profit_1 - z1.profit_1;

  combined_profit z_left( lambda_1, lambda_2, z1 );
  combined_profit z_right( lambda_1, lambda_2, z2 );

  combined_triangle_front region( true, z_left, z_right, lambda_1, lambda_2 );

  std::list<combined_profit> profits;
  pareto_set<bi_profit>::const_iterator it;

  for ( it=point_set.begin(); it!=point_set.end(); ++it )
    profits.push_front( combined_profit(lambda_1, lambda_2, *it) );

  region.tighten( profits.begin(), profits.end() );
  lower_bound = region.min_profit();
} // base_second_phase::triangle_set::local_point_set::compute_lower_bound()




/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
base_second_phase::triangle_set::~triangle_set()
{
  point_set_map::iterator it;

  for (it=m_triangles.begin(); it!=m_triangles.end(); ++it)
    delete it->second;
} // base_second_phase::triangle_set::~triangle_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a triangle in the set.
 * \param t The triangle to add.
 */
void base_second_phase::triangle_set::add( const triangle& t )
{
  m_triangles[t.top_left().profit_1] =
    new local_point_set( t.top_left(), t.bottom_right() );
} // base_second_phase::triangle_set::add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solutions of a search area.
 * \param t The area from which we want the solutions.
 */
const pareto_set<bi_profit>&
base_second_phase::triangle_set::get_set( const triangle& t )
{
  assert( m_triangles.find( t.top_left().profit_1 ) != m_triangles.end() );
  assert( !m_triangles[t.top_left().profit_1]->locked );

  return m_triangles[t.top_left().profit_1]->point_set;
} // base_second_phase::triangle_set::get_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Lock a set of solutions. No more solutions will be added.
 * \param t The search space to lock.
 */
void base_second_phase::triangle_set::lock( const triangle& t )
{
  assert( m_triangles.find( t.top_left().profit_1 ) != m_triangles.end() );
  assert( !m_triangles[t.top_left().profit_1]->locked );

  m_triangles[t.top_left().profit_1]->locked = true;
} // base_second_phase::triangle_set::lock()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a triangle is locked.
 */
bool base_second_phase::triangle_set::locked( const triangle& t ) const
{
  assert( m_triangles.find( t.top_left().profit_1 ) != m_triangles.end() );

  return m_triangles.find( t.top_left().profit_1 )->second->locked;
} // base_second_phase::triangle_set::locked()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a point in the adequate triangle.
 * \param s The point to add.
 */
void base_second_phase::triangle_set::insert( const bi_profit& s )
{
  assert( !m_triangles.empty() );

  point_set_map::iterator it;

  it = m_triangles.lower_bound( s.profit_1 );

  if ( it != m_triangles.begin() )
    {
      if ( it == m_triangles.end() )
	--it;
      else if ( it->first != s.profit_1 )
	{
	  assert( it->first > s.profit_1 );
	  --it;
	}

      if ( !it->second->locked )
	if ( s.profit_1 > it->second->z1.profit_1 )
	  if ( s.profit_2 > it->second->z2.profit_2 )
	    it->second->point_set.insert(s);
    }
} // base_second_phase::triangle_set::insert()

/*----------------------------------------------------------------------------*/
void base_second_phase::triangle_set::tighten()
{
  point_set_map::const_iterator it;

  for ( it=m_triangles.begin(); it!=m_triangles.end(); ++it )
    if ( !it->second->locked )
      it->second->compute_lower_bound();
} // base_second_phase::triangle_set::tighten()

/*----------------------------------------------------------------------------*/
real_type
base_second_phase::triangle_set::strip_width( const triangle& t ) const
{
  assert( m_triangles.find( t.top_left().profit_1 ) != m_triangles.end() );

  local_point_set* s = m_triangles.find( t.top_left().profit_1 )->second;

  return s->upper_bound - s->lower_bound;
} // base_second_phase::triangle_set::strip_width()

/*----------------------------------------------------------------------------*/
real_type
base_second_phase::triangle_set::get_lower_bound( const triangle& t ) const
{
  assert( m_triangles.find( t.top_left().profit_1 ) != m_triangles.end() );

  return m_triangles.find( t.top_left().profit_1 )->second->lower_bound;
} // base_second_phase::triangle_set::get_lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find points that dominate as most already seen triangles as possible.
 * \param ub (out) The points dominating the triangles.
 */
void
base_second_phase::triangle_set::large_cut( std::list<bi_profit>& ub ) const
{
  point_set_map::const_iterator it1;

  for ( it1=m_triangles.begin(); it1!=m_triangles.end(); ++it1 )
    {
      point_set_map::const_iterator it2(it1);
      bool stop = false;

      while (!stop)
	if ( it2 == m_triangles.end() )
	  stop = true;
	else if ( ! it2->second->locked )
	  stop = true;
	else
	  ++it2;

      if ( it2 != it1 )
	{
	  --it2;

	  ub.push_front
	    ( bi_profit( it2->second->z2.profit_1, it1->second->z1.profit_2 ) );

	  it1 = it2;
	}
    }
} // base_second_phase::triangle_set::large_cut()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
base_second_phase::triangle::triangle()
  : m_z_left(NULL), m_z_right(NULL)
{

} // base_second_phase::triangle::triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param z_left Value of the top-left point.
 * \param z_right Value of the bottom-right point..
 */
base_second_phase::triangle::triangle
( const bi_profit& z_left, const bi_profit& z_right )
  : m_z_left(&z_left), m_z_right(&z_right)
{
  assert( z_left.profit_1 < z_right.profit_1 );
  assert( z_right.profit_2 < z_left.profit_2 );
} // base_second_phase::triangle::triangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the top-left solution.
 */
const bi_profit& base_second_phase::triangle::top_left() const
{
  assert( m_z_left != NULL );
  return *m_z_left;
} // base_second_phase::triangle::top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bottom-right solution.
 */
const bi_profit& base_second_phase::triangle::bottom_right() const
{
  assert( m_z_right != NULL );
  return *m_z_right;
} // base_second_phase::triangle::bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the triangle.
 */
real_type base_second_phase::triangle::width() const
{
  assert( m_z_left != NULL );
  assert( m_z_right != NULL );

  return m_z_right->profit_1 - m_z_left->profit_1;
} // base_second_phase::triangle::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the triangle.
 */
real_type base_second_phase::triangle::height() const
{
  assert( m_z_left != NULL );
  assert( m_z_right != NULL );

  return m_z_left->profit_2 - m_z_right->profit_2;
} // base_second_phase::triangle::height()




/*----------------------------------------------------------------------------*/
bool base_second_phase::area_lesser::operator()
( const triangle& t1, const triangle& t2 ) const
{
  return t1.width() * t1.height() < t2.width() * t2.height();
} // base_second_phase::area_lesser::operator()()
