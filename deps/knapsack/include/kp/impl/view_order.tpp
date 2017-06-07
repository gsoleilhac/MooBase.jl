/**
 * \file kp/impl/view_order.tpp
 * \brief Implementation of the kp::view_order class.
 * \author Julien Jorge
 */
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

#include <numeric>
#include <algorithm>
#include <fstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The view on which we have a view.
 * \param w The weight applied to the cost of the variables.
 */
template<typename View>
kp::view_order<View>::efficiency_greater::efficiency_greater
( const view_type& v, const cost_vector& w )
  : m_view(v), m_weight(w)
{

} // view_order::efficiency_greater::efficiency_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two items on their efficiency.
 * \param i The first item.
 * \param j The second item.
 * \return m_view->get_variable(i).efficiency()[m_obj] >
 *         m_view->get_variable(j).efficiency()[m_obj].
 */
template<typename View>
bool kp::view_order<View>::efficiency_greater::operator()
  (unsigned int i, unsigned int j) const
{
  return ( m_view.get_variable(i).cost.scalar_product(m_weight)
           / m_view.get_variable(i).weight )
    > ( m_view.get_variable(j).cost.scalar_product(m_weight)
        / m_view.get_variable(j).weight );
} // view_order::efficiency_greater::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param freq The frequency associated to the variables.
 */
template<typename View>
kp::view_order<View>::frequency_greater::frequency_greater
( const std::vector<std::size_t>& freq )
  : m_frequency(freq)
{

} // view_order::frequency_greater::frequency_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two items on their frequency.
 * \param i The first item.
 * \param j The second item.
 * \return m_value[i] < m_value[j].
 */
template<typename View>
bool kp::view_order<View>::frequency_greater::operator()
  ( unsigned int i, unsigned int j ) const
{
  return m_frequency[i] > m_frequency[j];
} // view_order::frequency_greater::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The values associated to the variables.
 */
template<typename View>
kp::view_order<View>::valued_min_order::valued_min_order
( const std::vector<moo::real_type>& v )
  : m_value(v)
{

} // view_order::valued_min_order::valued_min_order()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two items on their values.
 * \param i The first item.
 * \param j The second item.
 * \return m_value[i] < m_value[j].
 */
template<typename View>
bool kp::view_order<View>::valued_min_order::operator()
  ( unsigned int i, unsigned int j ) const
{
  return m_value[i] < m_value[j];
} // view_order::valued_min_order::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The view on which we have a view.
 */
template<typename View>
kp::view_order<View>::view_order( const View& v )
  : super(v), m_order(v.get_size())
{
  unsigned int s=v.get_size();

  for (unsigned int i=0; i!=s; ++i)
    m_order[i] = i;
} // view_order::view_order()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a variable of the problem.
 * \param i The index of the variables.
 */
template<typename View>
const kp::variable& kp::view_order<View>::get_variable( unsigned int i ) const
{
  return this->m_view.get_variable(m_order[i]);
} // view_order::get_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by decreasing efficiency on a given objective.
 * \param obj The objective on which we compare the efficiency.
 */
template<typename View>
void kp::view_order<View>::decreasing_efficiency( unsigned int obj )
{
  cost_vector w( this->get_nb_objectives(), 0 );
  w[obj] = 1;

  decreasing_efficiency(w);
} // view_order::decreasing_efficiency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by decreasing efficiency according to a weighted
 *        sum of their costs.
 * \param w The weights applied to the costs.
 */
template<typename View>
void kp::view_order<View>::decreasing_efficiency( const cost_vector& w )
{
  std::sort( m_order.begin(), m_order.end(),
             efficiency_greater(this->m_view, w) );
} // view_order::decreasing_efficiency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply a topological sort on the variables such that if e_i dominates
 *        e_j then i is before j in the order.
 */
template<typename View>
void kp::view_order<View>::topological_efficiency()
{
  typedef moo::vector<moo::real_type> float_vector;
  typedef moo::compare_unidirectional_max<moo::real_type> float_compare;

  const unsigned int s = this->m_view.get_size();
  std::vector<float_vector> eff(s);
    
  get_efficiencies(eff);

  typedef boost::adjacency_list<> graph_type;
  
  graph_type g;
  float_compare compare;

  for (unsigned int i=0; i!=s; ++i)
    for (unsigned int j=i+1; j!=s; ++j)
      if ( compare(eff[i], eff[j]) & moo::general_dominance )
        boost::add_edge(i, j, g);
      else if ( compare(eff[j], eff[i]) & moo::general_dominance )
        boost::add_edge(j, i, g);

  boost::topological_sort(g, m_order.rbegin());
} // view_order::topological_efficiency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by increasing order of their maximum rank.
 */
template<typename View>
void kp::view_order<View>::max_rank()
{
  const unsigned int n = this->get_size();
  const unsigned int p = this->get_nb_objectives();

  std::vector< std::vector<std::size_t> > rank
    ( n, std::vector<std::size_t>(p, 0) );
  std::vector<moo::real_type> r(n);

  compute_rank(rank);

  for (unsigned int i=0; i!=n; ++i)
    {
      moo::real_type rp( *std::max_element(rank[i].begin(), rank[i].end()) );
      moo::real_type sum_r(std::accumulate(rank[i].begin(), rank[i].end(), 0));

      r[i] = rp + sum_r / moo::real_type(p*n);
    }

  std::sort( m_order.begin(), m_order.end(), valued_min_order(r) );
} // view_order::max_rank()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by increasing order of their minimum rank.
 */
template<typename View>
void kp::view_order<View>::min_rank()
{
  const unsigned int n = this->get_size();
  const unsigned int p = this->get_nb_objectives();

  std::vector< std::vector<std::size_t> > rank
    ( n, std::vector<std::size_t>(p, 0) );
  std::vector<moo::real_type> r(n);

  compute_rank(rank);

  for (unsigned int i=0; i!=n; ++i)
    {
      moo::real_type rp( *std::min_element(rank[i].begin(), rank[i].end()) );
      moo::real_type sum_r(std::accumulate(rank[i].begin(), rank[i].end(), 0));

      r[i] = rp + sum_r / moo::real_type(p*n);
    }

  std::sort( m_order.begin(), m_order.end(), valued_min_order(r) );
} // view_order::min_rank()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by increasing order of the sum of their rank.
 */
template<typename View>
void kp::view_order<View>::sum_rank()
{
  const unsigned int n = this->get_size();
  const unsigned int p = this->get_nb_objectives();

  std::vector< std::vector<std::size_t> > rank
    ( n, std::vector<std::size_t>(p, 0) );
  std::vector<moo::real_type> r(n);

  compute_rank(rank);

  for (unsigned int i=0; i!=n; ++i)
    r[i] = std::accumulate( rank[i].begin(), rank[i].end(), 0 );

  std::sort( m_order.begin(), m_order.end(), valued_min_order(r) );
} // view_order::sum_rank()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by decreasing frequency of in a set of solutions.
 * \param X The set from which we compute the frequency.
 */
template<typename View>
void kp::view_order<View>::frequency( const maximum_set& X )
{
  const std::size_t n = this->get_size();

  std::vector<std::size_t> freq(n, 0);
  maximum_set::const_iterator it;

  for (it=X.begin(); it!=X.end(); ++it)
    for (std::size_t i=0; i!=n; ++i)
      if ( it->status_of( get_variable(i) ) == moo::bin_set )
	++freq[i];

  std::sort( m_order.begin(), m_order.end(), frequency_greater(freq) );
} // view_order::frequency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Randomize the order on the variables.
 */
template<typename View>
void kp::view_order<View>::random()
{
  std::random_shuffle( m_order.begin(), m_order.end() );
} // view_order::random()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by their efficiency domination rank.
 */
template<typename View>
void kp::view_order<View>::domination_rank()
{
  typedef moo::vector<moo::real_type> float_vector;
  typedef moo::compare_unidirectional_max<moo::real_type> float_compare;

  const std::size_t s = this->m_view.get_size();
  std::vector<float_vector> eff(s);
    
  get_efficiencies(eff);

  float_compare compare;
  std::list<std::size_t> pending(m_order.begin(), m_order.end());
  m_order.clear();

  while ( !pending.empty() )
    {
      std::vector<std::size_t> dom_count(s, 0);
      typename std::list<std::size_t>::iterator it;
      typename std::list<std::size_t>::iterator it2;

      for (it=pending.begin(); it!=pending.end(); ++it)
        {
          it2=it;
          for (++it2; it2!=pending.end(); ++it2 )
            if ( compare(eff[*it2], eff[*it]) & moo::general_dominance )
              ++dom_count[*it];
            else if ( compare(eff[*it], eff[*it2]) & moo::general_dominance )
              ++dom_count[*it2];
        }

      for( it=pending.begin(); it!=pending.end(); )
        if ( dom_count[*it] == 0 )
          {
            m_order.push_back(*it);
            it2=it;
            ++it;
            pending.erase(it2);
          }
        else
          ++it;
    }
} // view_order::domination_rank()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the variables by their efficiency domination rank.
 */
template<typename View>
void kp::view_order<View>::domination_count()
{
  typedef moo::vector<moo::real_type> float_vector;
  typedef moo::compare_unidirectional_max<moo::real_type> float_compare;

  const std::size_t s = this->m_view.get_size();
  std::vector<float_vector> eff(s);
    
  get_efficiencies(eff);

  float_compare compare;
  std::vector<moo::real_type> dom_count(s, 0);

  for (std::size_t i=0; i!=s; ++i)
    for (std::size_t j=i+1; j!=s; ++j )
      if ( compare(eff[i], eff[j]) & moo::general_dominance )
        dom_count[j] += 1;
      else if ( compare(eff[j], eff[i]) & moo::general_dominance )
        dom_count[i] += 1;

  std::sort( m_order.begin(), m_order.end(), valued_min_order(dom_count) );
} // view_order::domination_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the rank of each variable for each objective, when sorted by
 *        decreasing order of efficiency.
 * \param r (out) Table of get_size() cells of get_nb_objectives() cells,
 *        containing the rank of each variable on each objective. This table
 *        must be large enough.
 */
template<typename View>
void kp::view_order<View>::compute_rank
( std::vector< std::vector<std::size_t> >& r )
{
  const unsigned int n = this->get_size();
  const unsigned int p = this->get_nb_objectives();

  for (unsigned int j=0; j!=p; ++j)
    {
      decreasing_efficiency(j);
      
      for (unsigned int i=0; i!=n; ++i)
        r[ m_order[i] ][j] = i;
    }
} // view_order::compute_rank()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the efficiency of the items.
 * \param eff (out) The efficiency of each item. This vector must be large
 *        enough.
 */
template<typename View>
void kp::view_order<View>::get_efficiencies
( std::vector< moo::vector<moo::real_type> >& eff ) const
{
  const unsigned int s = this->m_view.get_size();

  for (unsigned int i=0; i!=s; ++i)
    eff[i] = this->m_view.get_variable(i).efficiency();
} // view_order::get_efficiencies()
