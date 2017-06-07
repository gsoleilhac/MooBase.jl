/**
 * \file moo/imple/weight_set.tpp
 * \brief Implementation of the moo::weight_set class.
 * \author Julien Jorge
 */
#include <cassert>
#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename T>
moo::weight_set<T>::weight_set()
  : m_empty(false)
{
  vector<T> c(2, 0), zero(2, 0), x1(2, 0), y1(2, 0);

  x1[0] = 1;
  y1[1] = 1;

  c[0] = 1;
  m_constraint.push_front
    ( facet_type( constraint_type( c, constraint_type::greater_equal, 0 ),
		  zero, y1 ) );

  c[1] = 1;
  m_constraint.push_front
    ( facet_type( constraint_type( c, constraint_type::less_equal, 1 ),
		  y1, x1 ) );

  c[0] = 0;
  m_constraint.push_front
    ( facet_type( constraint_type( c, constraint_type::greater_equal, 0 ),
		  x1, zero ) );
} // weight_set::weight_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint to the weight set.
 * \param c The constraint to add.
 */
template<typename T>
void moo::weight_set<T>::add_constraint( const constraint_type& c )
{
  if ( !m_empty )
    {
      if ( conflicting(c) )
	m_empty = true;
      else if ( !redundant(c) )
	insert_and_update( c );
    }
} // weight_set::add_constraint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a point in inside the weight_set.
 * \param v The point to check.
 */
template<typename T>
bool moo::weight_set<T>::check( const vector_type& v ) const
{
  bool result = !m_empty;
  typename std::list<facet_type>::const_iterator it;

  for (it=m_constraint.begin(); result && (it!=m_constraint.end()); ++it)
    result = it->get_constraint().check(v);

  return result;
} // weight_set::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief ell if the weight set is empty.
 */
template<typename T>
bool moo::weight_set<T>::empty() const
{
  return m_empty;
} // weight_set::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a weight_set as text in an output stream.
 * \param os The stream to write in.
 */
template<typename T>
std::ostream& moo::weight_set<T>::output( std::ostream& os ) const
{
  if (m_empty)
    return os << "(empty)";
  else
    {
      typename std::list<facet_type>::const_iterator it;

      for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
        os << *it << '\n';

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
template<typename T>
void moo::weight_set<T>::gnuplot
( std::ostream& gp, std::ostream& data, const std::string& data_name ) const
{
  if ( !m_empty )
    {
      typename std::list<facet_type>::const_iterator it;

      gp << "set xrange [0:1.385]\n";
      gp << "set yrange [0:1]\n";

      gp << "plot \\\n";

      gp << "  1-x with lines 2, \\\n";

      for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
	{
	  gp << "  ";
	  it->get_constraint().gnuplot(gp);
	  gp << ", \\\n";
	  data << it->p[0] << '\n' << it->p[1] << "\n\n";
	}

      gp << "  \"" << data_name << "\" with linespoints ls -1\n";
    }
} // weight_set::gnuplot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 */
template<typename T>
bool moo::weight_set<T>::conflicting( const constraint_type& c ) const
{
  typename std::list<facet_type>::const_iterator it;
  bool found = false;

  // check that at least one vertex of the polygon is on the good side of the
  // constraint
  for (it=m_constraint.begin(); !found && (it!=m_constraint.end()); ++it)
    found = c.check(it->p[0]) || c.check(it->p[1]);

  return !found;
} // weight_set::conflicting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 */
template<typename T>
bool moo::weight_set<T>::redundant( const constraint_type& c ) const
{
  typename std::list<facet_type>::const_iterator it;
  bool found = false;

  // check that at least one vertex of the polygon is on the out side of the
  // constraint
  for (it=m_constraint.begin(); !found && (it!=m_constraint.end()); ++it)
    found = !c.check(it->p[0]) || !c.check(it->p[1]);

  return !found;
} // weight_set::redundant()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a constraint conflicts with an other one.
 * \param c The constraint to add.
 */
template<typename T>
void moo::weight_set<T>::insert_and_update( const constraint_type& c )
{
  typedef typename std::list<facet_type>::iterator list_iterator;

  std::vector< vector<T> > inter; // intersection of c with the weight_set
  std::vector<list_iterator> constraints; // the intersected constraints
  list_iterator it;

  // find the intersection of the new constraint with the weight_set
  for (it=m_constraint.begin(); it!=m_constraint.end(); ++it)
    if ( !c.parallel(it->get_constraint()) )
      {
	vector_type p = c.intersection(it->get_constraint());

	if ( check(p) )
	  {
	    inter.push_back(p);
	    constraints.push_back(it);
	  }
      }
  
  if ( !((inter.size() >= 2) && (inter.size() <= 4)) )
    {
      std::ofstream gp("fail.gp");
      std::ofstream data("fail.data");

      gnuplot( gp, data, "fail.data" );
      gp.close();
      data.close();

      output(std::cerr) << std::endl;
      std::cerr << "adding " << c << " -> " << inter.size() << std::endl;
      assert( (inter.size() >= 2) && (inter.size() <= 4) );
    }

  // update the bounds of the intersected facets
  for (unsigned int i=0; i!=constraints.size(); ++i)
    if ( !c.check( constraints[i]->p[0] ) )
      constraints[i]->p[0] = inter[i];
    else if ( !c.check( constraints[i]->p[1] ) )
      constraints[i]->p[1] = inter[i];

  // then remove the facets that became useless
  for ( it=m_constraint.begin(); it!=m_constraint.end(); )
    if ( (it->p[0] == it->p[1])              /* the constraint became a point */
	 || (!c.check(it->p[0]) && !c.check(it->p[1])) ) /* or is now outside */
      {
	list_iterator tmp(it);
	++it;
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

  m_constraint.push_front( facet_type(c, p1, p2) );
} // weight_set::insert_and_update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a weight_set as text in an output stream.
 * \param os The stream to write in.
 * \param p The weight_set to output.
 * \return os.
 */
template<typename T>
std::ostream& operator<<( std::ostream& os, const moo::weight_set<T>& p )
{
  return p.output(os);
} // operator<<()
