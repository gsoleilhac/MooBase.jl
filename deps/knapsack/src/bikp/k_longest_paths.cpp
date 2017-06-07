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
 * \file k_longest_paths.cpp
 * \brief Implementation of the k_longest_paths class.
 * \author Julien Jorge
 */
#include "bikp/solver/k_longest_paths.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy Constructor.
 * \param that the set to copy from.
 * \param v The vertex to be added to \a that.
 * \remark \a that must be dynamically allocated.
 */
k_longest_paths::vertex_set::vertex_set
( const vertex_set* that, const vertex_type& v )
  : m_links(1), m_parent(that), m_turning(v)
{
  if (m_parent)
    const_cast<vertex_set*>(m_parent)->add_link();
} // k_longest_paths::vertex_set::vertex_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Desctructor.
 */
k_longest_paths::vertex_set::~vertex_set()
{
  assert( m_links == 0 );

  if (m_parent)
    const_cast<vertex_set*>(m_parent)->remove_link();
} // k_longest_paths::vertex_set::~vertex_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the vertices in the set.
 * \param vertices (out) The vertices in the set.
 * \post vertices is sorted in decreasing order of vertex index.
 */
void k_longest_paths::vertex_set::get_vertices( set_type& vertices ) const
{
  assert( vertices.empty() ? true
	  : m_turning.index() > vertices.front()->index() );

  vertices.push_front( &m_turning );

  if ( m_parent )
    m_parent->get_vertices( vertices );
} // k_longest_paths::vertex_set::get_vertices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the last vertex in the set.
 */
const k_longest_paths::vertex_type&
k_longest_paths::vertex_set::last_vertex() const
{
  return m_turning;
} // k_longest_paths::vertex_set::last_vertex()

/*----------------------------------------------------------------------------*/
/**
 * \brief Release this instance.
 */
void k_longest_paths::vertex_set::release()
{
  remove_link();
} // k_longest_paths::vertex_set::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a link to this set.
 */
void k_longest_paths::vertex_set::add_link()
{
  ++m_links;
} // k_longest_paths::vertex_set::add_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a link to this set.
 */
void k_longest_paths::vertex_set::remove_link()
{
  assert( m_links > 0 );

  --m_links;

  if (m_links == 0)
    delete this;
} // k_longest_paths::vertex_set::remove_link()




// counters of instances of the path class.
unsigned int k_longest_paths::path::inst_count;
unsigned int k_longest_paths::path::inst_count_max;
unsigned int k_longest_paths::path::current;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The root of the path.
 *
 * Paths made with this constructor don't have out-vertices. They represent
 * optimal paths starting from the vertex \a r.
 */
k_longest_paths::path::path( const vertex_type& r )
  : m_vertices(NULL), m_root(r), m_profit( m_root.profit() )
{

} // k_longest_paths::path::path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param that The parent path.
 * \param t The out-vertex to take to quit the path.
 * \param p The profit of this path.
 *
 * Paths made with this constructor have one more out-vertex compared to their
 * parent path. They represent non-optimal paths starting from the vertex \a r.
 *
 * \pre that.make() has been called.
 */
k_longest_paths::path::path
( const path& that, const vertex_type& t, const combined_profit& p )
  : m_vertices(new vertex_set(that.m_vertices, t)), m_root(that.m_root),
    m_profit(p)
{
  /*if ( ++inst_count > inst_count_max )
    inst_count_max = inst_count;

    if ( inst_count % 100 == 0 )
    std::cout << inst_count << " (max " << inst_count_max << ") "
    << current << std::endl;*/
} // k_longest_paths::path::path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
k_longest_paths::path::~path()
{
  //--inst_count;

  if (m_vertices)
    m_vertices->release();
} // k_longest_paths::path::~path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the vertices in the path.
 * \param vertices (out) The vertices in the path.
 */
void k_longest_paths::path::get_vertices
( std::list<const_vertex_ptr>& vertices ) const
{
  vertices.clear();

  if ( m_vertices )
    m_vertices->get_vertices( vertices );
} // k_longest_paths::path::get_out_vertices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the profit of this path.
 */
const combined_profit& k_longest_paths::path::profit() const
{
  return m_profit;
} // k_longest_paths::path::profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the root vertex of this path.
 */
const k_longest_paths::vertex_type& k_longest_paths::path::root() const
{
  return m_root;
} // k_longest_paths::path::root()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the last vertex in the path.
 */
const k_longest_paths::vertex_type& k_longest_paths::path::last_vertex() const
{
  if ( !m_vertices )
    return m_root;
  else
    return m_vertices->last_vertex();
} // k_longest_paths::path::last_vertex()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to solve.
 * \param search_region (in) The region to explore, (out) a tightened region
 *        containing the solutions/
 * \param sols (in/out) The solutions found.
 * \param out_profits (in/out) The set of solutions outside of the region.
 * \param ub An upper bound front.
 */
k_longest_paths::k_longest_paths
( const combined_problem& p, combined_triangle_front& search_region,
  std::list<combined_solution>& sols, std::list<combined_profit>& out_profits )
  : m_problem(p), m_solutions(sols), m_graph(NULL),
    m_paths_region(search_region)
{
  m_out_profits.insert( out_profits.begin(), out_profits.end() );
  //path::inst_count = path::inst_count_max = 0;

  build_graph();
  //std::cout << search_region.data_file_prefix() << std::endl;
  //m_graph->output_dot( p, search_region.data_file_prefix() + ".dot" );
  build_solutions();
  search_region = m_paths_region;
  out_profits.clear();
  out_profits.insert
    ( out_profits.begin(), m_out_profits.begin(), m_out_profits.end() );
} // k_longest_paths::k_longest_paths()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
k_longest_paths::~k_longest_paths()
{
  quality_table_type::iterator it_1;
  std::size_t i(0);
  for (it_1=m_quality_table.begin(); it_1!=m_quality_table.end(); ++it_1)
    {
      quality_table_value_type::iterator it;
      
      for ( it=it_1->second.begin(); it!=it_1->second.end(); ++it,++i )
	delete *it;
    }

  if (m_graph != NULL)
    delete m_graph;
} // k_longest_paths::~k_longest_paths()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the graph representing solutions of the knapsack problem.
 */
void k_longest_paths::build_graph()
{
  assert( !m_graph );

  m_graph = new graph_type( m_problem, m_paths_region );

  const graph_type::layer_type& layer = m_graph->get_layer(m_problem.size());
  const real_type min_profit( (real_type)m_paths_region.min_profit() );

  for ( unsigned int i=0; i!=layer.size(); ++i )
    if ( (real_type)layer[i]->profit() >= min_profit )
      {
	m_quality_table[ layer[i]->profit() ].push_back( new path(*layer[i]) );

	if ( !m_paths_region.contains(layer[i]->profit()) )
	  m_out_profits.insert(layer[i]->profit());
      }
} // k_longest_paths::build_graph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solutions.
 * \pre The graph is built.
 */
void k_longest_paths::build_solutions()
{
  quality_table_type::iterator it;
  bool stop = false;

#if 0
  time_t ref_date( time(NULL) );
#endif

  while ( !stop && !m_quality_table.empty() )
    {
      it = m_quality_table.end();
      --it;

      stop = it->first < (real_type)m_paths_region.min_profit();

      if (!stop)
	{
	  while ( !it->second.empty() )
	    {
#if 0
              time_t now = time(NULL);
              if ( now - ref_date >= 5 ) // 5 seconds interval log
                {
                  std::size_t n(0);
                  quality_table_type::iterator itq
                    (m_quality_table.begin());

                  for ( ; itq!=m_quality_table.end(); ++itq )
                    n += it->second.size();

                  std::cout << it->first << " far from "
                            << m_paths_region.min_profit()
                            << " remaining paths=" << n << " values="
                            << m_quality_table.size()
                            << std::endl;
                  ref_date = now;
                }
#endif
	      path* p = it->second.front();
	      it->second.pop_front();

	      if ( !m_paths_region.contains( p->profit() ) )
                build_next_path(*p);
	      else
		{
		  combined_solution sol(m_problem);
		  build_solution( *p, sol );
		  add_solution( sol );
		}
	    
	      delete p;
	    }

	  m_quality_table.erase(it);

          clear_table();
	}
    }
} // k_longest_paths::build_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a solution from a path.
 * \param p The path to follow.
 * \param sol (out) The solution.
 *
 * New paths differing from p in one more out-vertex are added in
 * m_quality_table.
 */
void k_longest_paths::build_solution( const path& p, combined_solution& sol )
{
  std::list<const_vertex_ptr> vertices;
  p.get_vertices( vertices );

  std::list<const_vertex_ptr>::const_iterator it = vertices.begin();
  const_vertex_ptr v = &p.root();

  while ( it != vertices.end() )
    if ( (*it)->index() == v->index() - 1 )
      {
	set_solution_variable
	  ( (*it)->index(), sol, (*it)->weight() != v->weight() );
	v = *it;
	++it;
      }
    else
      build_optimal_solution_step( p, v, sol );

  build_optimal_solution( p, v, sol );
  assert( p.profit() == sol.objective_value() );
} // k_longest_paths::build_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the optimal solution starting from a given vertex.
 * \param p The path that bring us to \a v.
 * \param v The vertex from which we start.
 * \param sol (in/out) The solution.
 *
 * New paths differing from p in one more out-vertex are added in
 * m_quality_table.
 */
void k_longest_paths::build_optimal_solution
( const path& p, const_vertex_ptr &v, combined_solution& sol )
{
  while ( v->index() != 0 )
    build_optimal_solution_step_add( p, v, sol );
} // k_longest_paths::build_optimal_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a variable in a solution by taking the best in-edge
 *        of the current vertex.
 * \param p The path that bring us to \a v.
 * \param v The vertex where we are.
 * \param sol (in/out) The solution.
 *
 * \post !sol.is_free(v.var_index()-1)
 *
 * \return <true, t> if \a t is a parent for \a v and \a t has not been choosen,
 *         <false, NULL> otherwise.
 */
std::pair< bool, k_longest_paths::const_vertex_ptr >
k_longest_paths::build_optimal_solution_step
( const path& p, const_vertex_ptr &v, combined_solution& sol )
{
  assert( (v->in_degree() == 1) || (v->in_degree() == 2) );

  std::pair<bool, const_vertex_ptr> result(false, NULL);

  if ( v->in_degree() == 1 )
    {
      const vertex_type& parent = v->parent();
      set_solution_variable( parent.index(), sol,
			     parent.weight() != v->weight() );
      v = &parent;
    }
  else
    {
      result.first = true;

      const vertex_type& parent_zero = v->parent_zero();
      const vertex_type& parent_one = v->parent_one();

      if ( v->profit() == parent_zero.profit() )
	{
	  result.second = &parent_one;
	  set_solution_variable( parent_zero.index(), sol, false );
	  v = &parent_zero;
	}
      else
	{
	  assert( v->profit() - m_problem.profit( parent_one.index() )
		  == parent_one.profit() );

	  result.second = &parent_zero;
	  set_solution_variable( parent_one.index(), sol, true );
	  v = &parent_one;
	}
    }

  return result;
} // k_longest_paths::build_optimal_solution_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a variable in a solution by taking the best in-edge
 *        of the current vertex and add a possible new path in m_quality_table.
 * \param p The path that bring us to \a v.
 * \param v The vertex where we are.
 * \param sol (in/out) The solution.
 *
 * \post !sol.is_free(v.var_index-1)
 */
void k_longest_paths::build_optimal_solution_step_add
( const path& p, const_vertex_ptr &v, combined_solution& sol )
{
  const_vertex_ptr t = v;

  std::pair<bool, const_vertex_ptr> vertex =
    build_optimal_solution_step(p, v, sol);

  if (vertex.first)
    add_path(p, vertex.second, t);
} // k_longest_paths::build_optimal_solution_step_add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the next interesting paths.
 * \param p The initial path.
 */
void k_longest_paths::build_next_path( const path& p )
{
  const_vertex_ptr v = &p.last_vertex();

  while ( v->index() != 0 )
    if ( v->in_degree() == 1 )
      v = &v->parent();
    else
      {
        const vertex_type& parent_zero = v->parent_zero();
        const vertex_type& parent_one = v->parent_one();
	
        if ( v->profit() == parent_zero.profit() )
          {
            add_path(p, &parent_one, v);
            v = &parent_zero;
          }
        else
          {
            assert( v->profit() - m_problem.profit( parent_one.index() )
                    == parent_one.profit() );

            add_path(p, &parent_zero, v);
            v = &parent_one;
          }
      }
} // k_longest_paths::build_next_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the next interesting path in m_quality_table (if any).
 * \param p The reference path.
 * \param s The vertex to follow after having followed \a p.
 * \param t The vertex where we are when we must pass to \a s.
 */
void k_longest_paths::add_path
( const path& p, const_vertex_ptr s, const_vertex_ptr t )
{
  const real_type region_min_profit
    ( (real_type)m_paths_region.min_profit() );

  combined_profit max_profit = p.profit() + s->max_profit();
  combined_profit profit = p.profit() + s->profit();

  if ( s->weight() != t->weight() )
    {
      profit += m_problem.profit( s->index() );
      max_profit += m_problem.profit( s->index() );
    }

  profit -= t->profit();
  max_profit -= t->profit();

  if ( (real_type)profit >= region_min_profit )
    {
      if ( m_paths_region.contains(profit) )
        {
          path* new_path = new path(p, *s, profit);
          m_paths_region.insert(profit);
          m_quality_table[profit].push_front( new_path );

          if ( (real_type)m_paths_region.min_profit() > region_min_profit )
            clear_table();
        }
      else if ( m_paths_region.contains(max_profit) )
        {
          path* new_path = new path(p, *s, profit);
          m_out_profits.insert(profit);
          build_next_path(*new_path);
          delete new_path;
        }
      else
        m_out_profits.insert(profit);
    }

} // k_longest_paths::add_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the useless path in the quality table.
 */
void k_longest_paths::clear_table()
{
  quality_table_type::iterator it;
  const quality_table_type::iterator it_end( m_quality_table.end() );
  bool stop(false);
  const real_type lower_bound( m_paths_region.min_profit() );

  for (it=m_quality_table.begin(); !stop && (it!=it_end); ++it)
    if ( it->first >= lower_bound )
      stop = true;
    else
      for ( ; !it->second.empty(); it->second.pop_front() )
        delete it->second.front();
} // k_longest_paths::clear_table()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a variable in the solution.
 * \param i The index of the variable.
 * \param sol The solution.
 * \param in Tell if the variable must be set in the solution (not out).
 */
void k_longest_paths::set_solution_variable
( unsigned int i, combined_solution& sol, bool in ) const
{
  if ( in )
    sol.set( i );
  else
    sol.unset( i );
} // k_longest_paths::set_solution_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a solution in the set of optimal solutions and update the lower
 *        bound.
 * \param sol The solution to add.
 */
void k_longest_paths::add_solution( const combined_solution& sol )
{
  assert( sol.is_full() );
  assert( sol.is_feasible() );
  assert( m_paths_region.contains(sol.objective_value()) );

  m_solutions.push_front(sol);
} // k_longest_paths::add_solution()
