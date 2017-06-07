/**
 * \file weight_set.cpp
 * \brief Implementation of the weight_set class.
 * \author Julien Jorge
 */
#include <cassert>
#include <fstream>
#include <sstream>
#include <kp/weight_set.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param y The point optimal for the weight in this polytope.
 */
kp::weight_set::weight_set( const vector_type& y )
  : m_point(y), m_empty(false)
{
  vector_type c(2, 0), zero(2, 0), x1(2, 0), y1(2, 0);

  x1[0] = 1;
  y1[1] = 1;

  c[0] = 1;
  m_constraint.push_front
    ( neighbor_type
      ( NULL,
	facet_type( constraint_type( c, constraint_type::greater_equal, 0 ),
		    zero, y1 ) ) );

  c[1] = 1;
  m_constraint.push_front
    ( neighbor_type
      ( NULL,
	facet_type( constraint_type( c, constraint_type::less_equal, 1 ),
		    y1, x1 ) ) );

  c[0] = 0;
  m_constraint.push_front
    (neighbor_type
      ( NULL,
	facet_type( constraint_type( c, constraint_type::greater_equal, 0 ),
		    x1, zero ) ) );
} // weight_set::weight_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
kp::weight_set::~weight_set()
{
  facet_list_type::iterator it;

  for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
    if ( it->first != NULL )
      delete it->first;
} // weight_set::~weight_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point optimal for these weights.
 */
const kp::weight_set::vector_type& kp::weight_set::get_point() const
{
  return m_point;
} // weight_set::get_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first facet.
 */
kp::weight_set::const_facet_iterator kp::weight_set::facets_begin() const
{
  return m_constraint.begin();
} // weight_set::facets_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the facets.
 */
kp::weight_set::const_facet_iterator kp::weight_set::facets_end() const
{
  return m_constraint.end();
} // weight_set::facets_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint to the weight set.
 * \param adj The (potentially) adjacent point we must be better than.
 */
void kp::weight_set::add_constraint( const vector_type& adj )
{
  if ( !m_empty )
    {
      vector_type coeff(2);
      coeff[0] = m_point[0] - adj[0] - m_point[2] + adj[2];
      coeff[1] = m_point[1] - adj[1] - m_point[2] + adj[2];

      constraint_type c
	( coeff, constraint_type::greater_equal, adj[2] - m_point[2] );

      if ( conflicting(c) )
	m_empty = true;
      else if ( !redundant(c) )
	insert_and_update( adj, c );
    }
} // weight_set::add_constraint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a point in inside the weight_set.
 * \param v The point to check.
 */
bool kp::weight_set::check( const vector_type& v ) const
{
  bool result = !m_empty;
  facet_list_type::const_iterator it;

  for (it=m_constraint.begin(); result && (it!=m_constraint.end()); ++it)
    result = it->second.get_constraint().check(v);

  return result;
} // weight_set::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief ell if the weight set is empty.
 */
bool kp::weight_set::empty() const
{
  return m_empty;
} // weight_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a weight_set as text in an output stream.
 * \param os The stream to write in.
 */
std::ostream& kp::weight_set::output( std::ostream& os ) const
{
  if (m_empty)
    return os << "(empty)";
  else
    {
      facet_list_type::const_iterator it;

      for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
        os << it->second << '\n';

      return os;
    }
} // weight_set::output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the weight set in a format understood by gnuplot.
 * \param gp The stream in which we write gnuplot instructions.
 * \param data The stream in which we write the coordinates of the vertices.
 * \param data_name The name of the file constaining the vertices.
 */
void kp::weight_set::gnuplot
( std::ostream& gp, std::ostream& data, const std::string& data_name ) const
{
  if ( !m_empty )
    {
      facet_list_type::const_iterator it;

      gp << "set xrange [0:1.385]\n";
      gp << "set yrange [0:1]\n";

      gp << "plot \\\n";

      gp << "  1-x with lines 2, \\\n";

      for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
	{
	  gp << "  ";
	  it->second.get_constraint().gnuplot(gp);
	  gp << ", \\\n";
	  data << it->second.p[0] << '\n' << it->second.p[1] << "\n\n";
	}

      gp << "  \"" << data_name << "\" with linespoints ls -1\n";
    }
} // weight_set::gnuplot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 */
bool kp::weight_set::conflicting( const constraint_type& c ) const
{
  facet_list_type::const_iterator it;
  bool found = false;

  // check that at least one vertex of the polygon is on the good side of the
  // constraint
  for (it=m_constraint.begin(); !found && (it!=m_constraint.end()); ++it)
    found = c.check(it->second.p[0]) || c.check(it->second.p[1]);

  return !found;
} // weight_set::conflicting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 */
bool kp::weight_set::redundant( const constraint_type& c ) const
{
  facet_list_type::const_iterator it;
  bool found = false;

  // check that at least one vertex of the polygon is on the out side of the
  // constraint
  for (it=m_constraint.begin(); !found && (it!=m_constraint.end()); ++it)
    found = !c.check(it->second.p[0]) || !c.check(it->second.p[1]);

  return !found;
} // weight_set::redundant()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 * \param adj The (potentially) adjacent point we must be better than.
 */
void kp::weight_set::insert_and_update
( const vector_type& adj, const constraint_type& c )
{
  typedef facet_list_type::iterator list_iterator;

  // intersection of c with the weight_set
  std::vector<vector_type> inter;
  std::vector<list_iterator> constraints; // the intersected constraints
  list_iterator it;

  // find the intersection of the new constraint with the weight_set
  for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
    if ( !c.parallel(it->second.get_constraint()) )
      {
	vector_type p = c.intersection(it->second.get_constraint());

	if ( check(p) )
	  {
	    inter.push_back(p);
	    constraints.push_back(it);
	  }
      }

  // update the bounds of the intersected facets
  for (unsigned int i=0; i!=constraints.size(); ++i)
    if ( !c.check( constraints[i]->second.p[0] ) )
      constraints[i]->second.p[0] = inter[i];
    else if ( !c.check( constraints[i]->second.p[1] ) )
      constraints[i]->second.p[1] = inter[i];

  // then remove the facets that became useless :
  //  - the constraint became a point
  //  - or is now outside
  for ( it=m_constraint.begin(); it!=m_constraint.end(); )
    if ( (it->second.p[0] == it->second.p[1]) 
	 || (!c.check(it->second.p[0]) && !c.check(it->second.p[1])) )
      {
	list_iterator tmp(it);
	++it;

	if ( tmp->first != NULL )
	  delete tmp->first;

	m_constraint.erase(tmp);
      }
    else
      ++it;

  // insert the constraint
  vector_type p1 = inter[0];
  vector_type p2 = inter[1];

  for (unsigned int i=2; (p1==p2) && (i!=inter.size()); ++i)
    if ( inter[i] != p2 )
      p2 = inter[i];

  m_constraint.push_front
    ( neighbor_type( new vector_type(adj), facet_type(c, p1, p2) ) );
} // weight_set::insert_and_update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a weight_set as text in an output stream.
 * \param os The stream to write in.
 * \param p The weight_set to output.
 * \return os.
 */
std::ostream& operator<<( std::ostream& os, const kp::weight_set& p )
{
  return p.output(os);
} // operator<<()
