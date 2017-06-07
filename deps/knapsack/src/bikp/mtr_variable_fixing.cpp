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
 * \file mtr_variable_fixing.cpp
 * \brief Implementation of the mtr_variable_fixing class.
 * \author Julien Jorge
 */
#include "bikp/tool/algorithm.hpp"
#include "bikp/reduction/mtr_variable_fixing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The lower bound to copy from.
 */
mtr_variable_fixing::lower_bound_type::lower_bound_type
( const lower_bound_type& that )
  : m_problem_size(that.m_problem_size), m_solution(NULL),
    m_profit(that.m_profit), m_split_item(that.m_split_item),
    m_add_item(that.m_add_item), m_remove_item(that.m_remove_item)
{
  if ( that.m_solution )
    m_solution = new mono_solution( *that.m_solution );
} // mtr_variable_fixing::lower_bound_type::lower_bound_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param problem_size The size of the problem for which we are a solution.
 */
mtr_variable_fixing::lower_bound_type::lower_bound_type
( unsigned int problem_size )
  : m_problem_size(problem_size), m_solution(NULL),
    m_split_item(m_problem_size), m_add_item(m_problem_size),
    m_remove_item(m_problem_size)
{
  m_profit = 0;
} // mtr_variable_fixing::lower_bound_type::lower_bound_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param problem_size The size of the problem.
 * \param sol The solution that we represent.
 * \pre sol.size() == problem_size.
 * \remark We could do without problem size but, then, we have a problem when
 *         mono_solution is of type 'real_type': the above constructor will
 *         be called.
 */
mtr_variable_fixing::lower_bound_type::lower_bound_type
( unsigned int problem_size, const mono_solution& sol )
  : m_problem_size(sol.size()), m_solution(NULL),
    m_profit(sol.objective_value()), m_add_item(m_problem_size),
    m_remove_item(m_problem_size)
{
  assert( sol.size() == problem_size );
  assert( sol.is_feasible() );

  m_solution = new mono_solution(sol);
} // mtr_variable_fixing::lower_bound_type::lower_bound_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
mtr_variable_fixing::lower_bound_type::~lower_bound_type()
{
  if (m_solution)
    delete m_solution;
} // mtr_variable_fixing::lower_bound_type::~lower_bound_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the method for building the solution.
 * \param profit The value of the solution.
 * \param split_item The first item that doesn't fit in the solution.
 * \param index The index to add in the solution.
 * \pre (index > split_item) && (m_solution == NULL).
 */
void mtr_variable_fixing::lower_bound_type::set_add
( real_type profit, unsigned int split_item, unsigned int index )
{
  assert(index > split_item);
  assert(m_solution == NULL);

  m_profit = profit;
  m_split_item = split_item;
  m_add_item = index;
  m_remove_item = m_problem_size;
} // mtr_variable_fixing::lower_bound_type::set_add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the method for building the solution.
 * \param profit The value of the solution.
 * \param split_item The first item that doesn't fit in the solution.
 * \param index The index to remove of the solution.
 * \pre (index < split_item) && (m_solution == NULL).
 */
void mtr_variable_fixing::lower_bound_type::set_remove
( real_type profit, unsigned int split_item, unsigned int index )
{
  assert(index < split_item);
  assert(m_solution == NULL);

  m_profit = profit;
  m_split_item = split_item;
  m_add_item = m_problem_size;
  m_remove_item = index;
} // mtr_variable_fixing::lower_bound_type::set_remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the profit. No solution will be buildable.
 * \param profit The value of the solution.
 * \pre (m_split_item == m_problem_size) && (m_add_item == m_problem_size) &&
 *      (m_remove_item == m_problem_size) && (m_solution == NULL).
 */
void mtr_variable_fixing::lower_bound_type::set_profit( real_type profit )
{
  assert(m_split_item == m_problem_size);
  assert(m_add_item == m_problem_size);
  assert(m_remove_item == m_problem_size);
  assert(m_solution == NULL);

  m_profit = profit;
} // mtr_variable_fixing::lower_bound_type::set_remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solution corresponding to the bound.
 * \param sol (out) The solution to build.
 */
void mtr_variable_fixing::lower_bound_type::build( mono_solution& sol ) const
{
  assert( sol.size() == m_problem_size );
  assert( (m_add_item == m_problem_size) ^ (m_remove_item == m_problem_size)
	  || ( (m_add_item == m_problem_size)
	       && (m_remove_item == m_problem_size) && (m_solution != NULL) ) );

  sol.clear();

  if ( m_solution != NULL )
    sol = *m_solution;
  else if (m_add_item == m_problem_size)
    build_suppression( sol );
  else if (m_remove_item == m_problem_size)
    build_insertion( sol );

  assert( sol.is_feasible() );
  assert( sol.is_full() );
} // mtr_variable_fixing::lower_bound_type::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the bound.
 */
real_type mtr_variable_fixing::lower_bound_type::to_profit() const
{
  return m_profit;
} // mtr_variable_fixing::lower_bound_type::to_profit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solution by inserting an item.
 * \param sol (out) The solution to build.
 */
void mtr_variable_fixing::lower_bound_type::build_insertion
( mono_solution& sol ) const
{
  assert( m_add_item != m_problem_size );
  assert( m_remove_item == m_problem_size );
  
  for (unsigned int i=0; i!=m_split_item; ++i)
    sol.set(i);

  sol.set( m_add_item );

  for (unsigned int i=m_split_item; i!=m_add_item; ++i)
    sol.unset(i);

  for (unsigned int i=m_add_item+1; i!=sol.size(); ++i)
    sol.unset(i);

  assert( sol.is_feasible() );
  assert( sol.is_full() );
  assert( sol.objective_value() == m_profit );
} // mtr_variable_fixing::lower_bound_type::build_insertion()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solution by removing an item.
 * \param sol (out) The solution to build.
 */
void mtr_variable_fixing::lower_bound_type::build_suppression
( mono_solution& sol ) const
{
  assert( m_add_item == m_problem_size );
  assert( m_remove_item != m_problem_size );
  
  for (unsigned int i=0; i!=m_remove_item; ++i)
    sol.set(i);

  sol.unset( m_remove_item );

  for (unsigned int i=m_remove_item+1; i!=m_split_item; ++i)
    sol.set(i);

  for (unsigned int i=m_split_item; i!=sol.size(); ++i)
    sol.unset(i);

  assert( sol.is_feasible() );
  assert( sol.is_full() );
  assert( sol.objective_value() == m_profit );
} // mtr_variable_fixing::lower_bound_type::build_suppression()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two lower bounds in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if and only if \a a is lesser than \a b.
 */
bool mtr_variable_fixing::lower_bound_traits::less::operator()
( const lower_bound_type& a, const lower_bound_type& b ) const
{
  profit_traits_type::less comp;

  return comp(a.to_profit(), b.to_profit());
} // lower_bound_traits::less::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a lower_bound dominates an other.
 * \param a Left operand.
 * \param b Right operand.
 * \return True if \a a dominates \a b.
 */
bool mtr_variable_fixing::lower_bound_traits::better
( const lower_bound_type& a, const lower_bound_type& b )
{
  return profit_traits_type::better( a.to_profit(), b.to_profit() );
} // lower_bound_traits::better_than()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two lower_bounds are of same quality.
 * \param a Left operand.
 * \param b Right operand.
 */
bool mtr_variable_fixing::lower_bound_traits::equal
( const lower_bound_type& a, const lower_bound_type& b )
{
  return profit_traits_type::equal( a.to_profit(), b.to_profit() );
} // lower_bound_traits::equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a lower_bound dominates an other or if the two lower_bounds
 *        are of same quality.
 * \param a Left operand.
 * \param b Right operand.
 */
bool mtr_variable_fixing::lower_bound_traits::better_equal
( const lower_bound_type& a, const lower_bound_type& b )
{
  return better(a, b) || equal(a, b);
} // lower_bound_traits::equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two lower_bounds are exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
bool mtr_variable_fixing::lower_bound_traits::same
( const lower_bound_type& a, const lower_bound_type& b )
{
  return false;
} // lower_bound_traits::same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a lower bound dominates an other or if the two lower bounds
 *        are exactly the same.
 * \param a Left operand.
 * \param b Right operand.
 */
bool mtr_variable_fixing::lower_bound_traits::better_same
( const lower_bound_type& a, const lower_bound_type& b )
{
  return better(a, b) || same(a, b);
} // lower_bound_traits::same()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two lower_bounds in lexicographic order.
 * \param a Left operand.
 * \param b Right operand.
 * \return less(a, b).
 */
bool mtr_variable_fixing::lower_bound_traits::lex_less
( const lower_bound_type& a, const lower_bound_type& b )
{
  less comp;

  return comp(a, b);
} // lower_bound_traits::lex_less()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param sol (in/out) The best solution found.
 *
 * If the size of the resulting problem is zero, then \a sol is an optimal
 * solution.
 *
 * This constructor builds a reduction keeping only one solution. If you want
 * equivalent optimal solutions, use the other constructor.
 *
 * \pre The variables of \a p are sorted by decreasing efficiency.
 */
mtr_variable_fixing::mtr_variable_fixing
( const mono_problem& p, mono_solution& sol )
  : super(p)
{
  assert( p.size() > 0 );

  optimal_solution_set sols;
  sols.insert(sol);
  
  run( sols );
  
#ifndef NDEBUG
  if ( sol != *sols.begin() )
    assert( sols.begin()->is_full() );
#endif

  sol = *sols.begin();
} // mtr_variable_fixing::mtr_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The problem to reduce.
 * \param sols (in/out) The bests solutions found.
 *
 * If the size of the resulting problem is zero, then \a sol is an optimal
 * solution.
 *
 * \pre The variables of \a p are sorted by decreasing efficiency.
 */
mtr_variable_fixing::mtr_variable_fixing
( const mono_problem& p, optimal_solution_set& sols )
  : super(p)
{
  assert( p.size() > 0 );

  run( sols );
} // mtr_variable_fixing::mtr_variable_fixing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the problem reduction.
 * \param sols (in/out) The best solutions found.
 */
void mtr_variable_fixing::run( optimal_solution_set& sols )
{
  const unsigned int problem_size = this->m_problem.size();

  std::vector<real_type> weight_sum(problem_size);
  std::vector<real_type> profit_sum(problem_size);
  
  calculate_sums( weight_sum, profit_sum );
  
  const real_type capacity = this->m_problem.capacity();
  const unsigned int split_item = find_index( capacity, weight_sum );
  
  if (split_item < problem_size)
    find_sets( weight_sum, profit_sum, split_item, sols );
  else
    {
      mono_solution sol(this->m_problem);

      for (unsigned int i=0; i!=problem_size; ++i)
        {
          this->m_set_variables.insert(i);
          sol.set(i);
        }

      sols.insert(sol);
    }

  this->create_sub_problem();
} // mtr_variable_fixing::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the sets of fixed variables.
 * \param weight_sum Table such that weight_sum[i] is the sum of the weight of
 *        the items j, j <= i.
 * \param profit_sum Table such that profit_sum[i] is the sum of the profit of
 *        the items j, j <= i.
 * \param split_item Index of the split item.
 * \param sols (in/out) The best known solutions.
 */
void mtr_variable_fixing::find_sets
( const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  unsigned int split_item, optimal_solution_set& sols )
{
  const unsigned int problem_size = this->m_problem.size();
  lower_bound_set lower_bound( sols.is_multi_set() );

  greedy( weight_sum, profit_sum, sols );
  build_lower_bound( sols, lower_bound );

  std::vector<real_type> upper_bound_zero(split_item+1);
  std::vector<real_type> upper_bound_one(problem_size - split_item);
  
  calculate_upper_bound_zero( upper_bound_zero, weight_sum, profit_sum,
			      lower_bound );
  calculate_upper_bound_one( upper_bound_one, weight_sum, profit_sum,
			     lower_bound, split_item );

  build_solutions( lower_bound, sols );
  fix_variables( lower_bound, upper_bound_zero, upper_bound_one, split_item );
  fix_more();
} // mtr_variable_fixing::find_sets()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the index of the first item in the table such that
 *        table[i-1] <= val < table[i].
 * \param val The value to find.
 * \param table The table to search.
 */
unsigned int mtr_variable_fixing::find_index
( real_type val, const std::vector<real_type>& table ) const
{
  assert( table.size() > 0 );

  return find_first_greater( val, table.begin(), table.end() ) - table.begin();
} // mtr_variable_fixing::find_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the tables of cumulates weights and profits.
 * \param weight_sum (out) The table of cumulate weights.
 * \param profit_sum (out) The table of cumulate profits.
 */
void mtr_variable_fixing::calculate_sums
( std::vector<real_type>& weight_sum,
  std::vector<real_type>& profit_sum ) const
{
  weight_sum[0] = this->m_problem.weight(0);
  profit_sum[0] = this->m_problem.profit(0);

  for (unsigned int i=1; i!=this->m_problem.size(); ++i)
    {
      weight_sum[i] = weight_sum[i-1] + this->m_problem.weight(i);
      profit_sum[i] = profit_sum[i-1] + this->m_problem.profit(i);
    }
} // mtr_variable_fixing::calculate_sums()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a lower bound using a greedy algorithm taking profit of the
 *        cumulate weights and profits tables.
 * \param weight_sum The table of cumulate weights.
 * \param profit_sum The table of cumulate profits.
 * \param sols (in/out) The best known solutions.
 */
void mtr_variable_fixing::greedy
( const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  optimal_solution_set& sols ) const
{
  real_type remaining_capacity = this->m_problem.capacity();
  mono_solution local_sol( this->m_problem );

  for (unsigned int i=0; i<this->m_problem.size(); ++i)
    if ( this->m_problem.weight(i) <= remaining_capacity )
      {
	local_sol.set(i);
	remaining_capacity -= this->m_problem.weight(i);
      }
    else
      local_sol.unset(i);

  sols.insert(local_sol);
} // mtr_variable_fixing::greedy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the upper bound by setting the value of each item in range
 *        [1,s] to zero and update the lower bound.
 * \param upper_bound (out) The upper bounds computed.
 * \param weight_sum The table of cumulate weights.
 * \param profit_sum The table of cumulate profits.
 * \param lower_bound (in/out) The current lower bound on the optimal solutions.
 */
void mtr_variable_fixing::calculate_upper_bound_zero
( std::vector<real_type>& upper_bound,
  const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  lower_bound_set& lower_bound ) const
{
  const real_type capacity = this->m_problem.capacity();

  for (unsigned int i=0; i!=upper_bound.size(); ++i)
    {
      const real_type item_profit = this->m_problem.profit(i); 
      const real_type item_weight = this->m_problem.weight(i);
      const unsigned int new_split =
	find_index( capacity + item_weight, weight_sum );

      const real_type remaining_capacity =
	capacity + item_weight - weight_sum[new_split-1];
      const real_type new_profit = profit_sum[new_split - 1] - item_profit;

      upper_bound[i] = relax(new_split, remaining_capacity, new_profit);

      lower_bound_type lb( this->m_problem.size() );
      lb.set_remove( new_profit, new_split, i );
      lower_bound.insert( lb );
    }
} // mtr_variable_fixing::calculate_upper_bound_zero()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the upper bound by setting the value of each item in range
 *        [s,n] to one and update the lower bound.
 * \param upper_bound (out) The upper bounds computed.
 * \param weight_sum The table of cumulate weights.
 * \param profit_sum The table of cumulate profits.
 * \param lower_bound (in/out) The current lower bound on the optimal solutions.
 * \param split_item Index of the split item.
 */
void mtr_variable_fixing::calculate_upper_bound_one
( std::vector<real_type>& upper_bound,
  const std::vector<real_type>& weight_sum,
  const std::vector<real_type>& profit_sum,
  lower_bound_set& lower_bound, unsigned int split_item ) const
{
  const real_type capacity = this->m_problem.capacity();

  for (unsigned int i=0; i!=upper_bound.size(); ++i)
    {
      const unsigned int index = split_item + i;
      const real_type item_profit = this->m_problem.profit(index); 
      const real_type item_weight = this->m_problem.weight(index);
      const unsigned int new_split =
	find_index( capacity - item_weight, weight_sum );

      real_type weights;
      real_type profits;
      
      if (new_split > 0)
	{
	  weights = weight_sum[new_split-1];
	  profits = profit_sum[new_split-1];
	}
      else
	{
	  weights = 0;
	  profits = 0;
	}

      const real_type remaining_capacity = capacity - item_weight - weights;
      const real_type new_profit = profits + item_profit;

      upper_bound[i] = relax(new_split, remaining_capacity, new_profit);

      lower_bound_type lb( this->m_problem.size() );
      lb.set_add( new_profit, new_split, index );
      lower_bound.insert( lb );
    }
} // mtr_variable_fixing::calculate_upper_bound_one()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the relaxation.
 * \param split_item Index of the split item.
 * \param capacity Remaining capacity.
 * \param current_profit Profit to add to the calculated bound.
 */
real_type mtr_variable_fixing::relax
( unsigned int split_item, real_type capacity,
  real_type current_profit ) const
{
  assert( capacity >= 0 );

  relaxation_type rl;
  relaxation_type::result_type result;

  result = rl.calculate(this->m_problem, split_item, current_profit, capacity);

  return result.objective_value;
} // mtr_variable_fixing::relax()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the lower bound from the best known solutions.
 * \param sols The best knows solutions.
 * \param lower_bound (in/out) The lower bound.
 */
void mtr_variable_fixing::build_lower_bound
( const optimal_solution_set& sols, lower_bound_set& lower_bound ) const
{
  optimal_solution_set::const_iterator it;

  for ( it=sols.begin(); it!=sols.end(); ++it )
    lower_bound.insert( lower_bound_type( this->m_problem.size(), *it ) );
} // mtr_variable_fixing::build_lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the solutions from the lower bound.
 * \param lower_bound The lower bound.
 * \param sols (in/out) The best knows solutions.
 */
void mtr_variable_fixing::build_solutions
( const lower_bound_set& lower_bound, optimal_solution_set& sols ) const
{
  sols.clear();

  lower_bound_set::const_iterator it;
  mono_solution local_sol(this->m_problem);

  for ( it=lower_bound.begin(); it!=lower_bound.end(); ++it )
    {
      it->build( local_sol );
      sols.insert( local_sol );
    }
} // mtr_variable_fixing::build_solutions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fix the value of the variables.
 * \param lower_bound The ower frontier.
 * \param upper_bound_zero The values of the upper bound for each item from
 *        zero to \a split_item.
 * \param upper_bound_one The values of the upper bound for each item from
 *        \a split_item to the last.
 * \param split_item Index of the split item.
 */
void mtr_variable_fixing::fix_variables
( const lower_bound_set& lower_bound,
  const std::vector<real_type>& upper_bound_zero,
  const std::vector<real_type>& upper_bound_one, unsigned int split_item )
{
  assert( upper_bound_zero.size() == split_item + 1 );
  assert( upper_bound_one.size() == this->m_problem.size() - split_item );

  lower_bound_type low(this->m_problem.size());

  for (unsigned int i=0; i<=split_item; ++i)
    {
      low.set_profit(upper_bound_zero[i]);
      
      if ( lower_bound.is_dominated( low ) )
	this->m_set_variables.insert( i );
    }

  for (unsigned int i=split_item; i!=this->m_problem.size(); ++i)
    {
      unsigned int index = i-split_item;

      low.set_profit(upper_bound_one[index]);
      
      if ( lower_bound.is_dominated( low ) )
	this->m_unset_variables.insert( i );
    }
} // mtr_variable_fixing::fix_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Try to fix more variables by removing the ones that doesn't fit in the
 *        remaining capacity and by fixing all remaining variables if the sum
 *        of their weight fit in the remaining capacity.
 */
void mtr_variable_fixing::fix_more()
{
  real_type needed_capacity = 0;
  fixed_set_type::const_iterator it;

  for (it=this->m_set_variables.begin(); it!=this->m_set_variables.end(); ++it)
    {
      needed_capacity += this->m_problem.weight( *it );  

#ifndef NDEBUG
      if (m_unset_variables.find(*it) != m_unset_variables.end() )
        assert( m_set_variables.size() + m_unset_variables.size()
                >= m_problem.size() );
#endif
    }

  // infeasibility ==> lower bound is optimal
  if ( needed_capacity > this->m_problem.capacity() )
    {
      this->m_set_variables.clear();
      
      for (unsigned int i=0; i!=this->m_problem.size(); ++i)
	this->m_unset_variables.insert(i);
    }
  else
    {
      real_type free_weight_sum = 0;
      std::vector<std::size_t> free_items;
      unsigned int more(0);
      const real_type remaining_capacity =
	this->m_problem.capacity() - needed_capacity;

      free_items.reserve( this->m_problem.size() - 
			  std::max( this->m_set_variables.size(),
				    this->m_unset_variables.size() ) );

      for (unsigned int i=0; i!=this->m_problem.size(); ++i)
	if ((this->m_set_variables.find(i) == this->m_set_variables.end()) &&
	    (this->m_unset_variables.find(i) == this->m_unset_variables.end()))
	  {
	    if ( this->m_problem.weight(i) > remaining_capacity )
	      {
		++more;
		this->m_unset_variables.insert(i);
	      }
	    else
	      {
		free_items.push_back(i);
		free_weight_sum += this->m_problem.weight(i);
	      }
	  }
      
      if ( free_weight_sum <= remaining_capacity )
	{
	  more += free_items.size();
	  
	  for (unsigned int i=0; i!=free_items.size(); ++i)
	    this->m_set_variables.insert( free_items[i] );
	}
    }
} // mtr_variable_fixing::fix_more()

