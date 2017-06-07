/**
 * \file lexicographic_solver.cpp
 * \brief Implementation of the kp::lexicographic_solver class.
 * \author Julien Jorge
 */
#include <bikp/problem/mono_problem.hpp>
#include <bikp/reduction/mtr_variable_fixing.hpp>
#include <bikp/solver/dynamic_programming_single.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the lexicographic optimal solutions.
 * \param inst The instance to solve.
 * \param X The computed solutions.
 */
template<typename View>
void kp::lexicographic_solver<View>::solve
( const view_type& inst, std::list<knapsack_solution>& X ) const
{
  std::vector<knapsack_solution> ref
    ( inst.get_nb_objectives(), knapsack_solution(inst) );

  cost_vector coeff( inst.get_nb_objectives(), 0 );
  cost_vector ideal( compute_ideal(inst, ref) );
  std::vector<std::size_t> order( inst.get_nb_objectives() );

  // 3-criteria only
  for (unsigned int i=0; i!=3; ++i)
    for (unsigned int j=0; j!=3; ++j)
      if ( j!=i )
        for (unsigned int k=0; k!=3; ++k)
          if ( (k!=i) && (k!=j) )
            {
              order[0] = i;
              order[1] = j;
              order[2] = k;

              compute_coeff( order, ideal, ref[i].image(), coeff );

              solve_mono( inst, coeff, ref[i] );

              bool insert = true;
              std::list<knapsack_solution>::const_iterator it;

              for (it=X.begin(); insert && (it!=X.end()); ++it)
                insert = it->image() != ref[i].image();

              if ( insert )
                X.push_back(ref[i]);
            }
} // lexicographic_solver::solve()

/*----------------------------------------------------------------------------*/
template<typename View>
void kp::lexicographic_solver<View>::compute_coeff
( const std::vector<std::size_t>& order, const cost_vector& ideal,
  const cost_vector& ref, cost_vector& coeff,
  unsigned int index ) const
{
  if ( index+1 == order.size() )
    coeff[ order[index] ] = 1;
  else
    {
      compute_coeff( order, ideal, ref, coeff, index+1 );
      coeff[ order[index] ] =
        coeff[ order[index+1] ]
        * ( ideal[ order[index+1] ] /*- ref[ order[index+1] ]*/ + 1 );
    }
} // lexicographic_solver::compute_coeff()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the ideal point.
 * \param inst The instance to solve.
 * \param sols (out) The solution found on each objective (sols[i] is the
 *        solution found for the ith objective).
 */
template<typename View>
kp::cost_vector kp::lexicographic_solver<View>::compute_ideal
( const view_type& inst,
  std::vector<knapsack_solution>& sols ) const
{
  assert( sols.size() == inst.get_nb_objectives() );

  cost_vector result( inst.get_nb_objectives() );
  cost_vector ref( inst.get_nb_objectives(), 0 );
  cost_vector coeff( inst.get_nb_objectives(), 0 );
  coeff[0] = 1;

  solve_mono(inst, coeff, sols[0] );
  result[0] = sols[0].image()[0];

  for ( unsigned int j=1; j!=inst.get_nb_objectives(); ++j )
    {
      coeff[j-1] = 0;
      coeff[j] = 1;

      sols[j] = sols[0];

      for (unsigned int i=0; i!=j; ++i)
        if ( sols[i].image()[j] > sols[j].image()[j] )
          sols[j] = sols[i];
      
      solve_mono(inst, coeff, sols[j]);
      result[j] = sols[j].image()[j];
    }

  return result;
} // lexicographic_solver::compute_ideal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Solve a linear combination of the problem.
 * \param inst The instance to solve.
 * \param coeff The coefficient to apply to the objectives.
 */
template<typename View>
void kp::lexicographic_solver<View>::solve_mono
( const view_type& inst, const cost_vector& coeff,
  knapsack_solution& result ) const
{
  
  cost_vector lb(result.image());

  // build the problem
  mono_problem mono( inst.get_size(), inst.get_capacity() );

  for ( unsigned int i=0; i!=inst.get_size(); ++i )
    mono.set_variable( i, inst.get_variable(i).cost.scalar_product(coeff),
                       inst.get_variable(i).weight );

  mono.sort_by_decreasing_efficiency();
  mono_solution sol(mono);

  // reduce the problem
  mtr_variable_fixing red(mono, sol);
  const mono_problem& sub_p( red.get_sub_problem() );

  // get the best solution of lb and the solution built by the reduction
  // procedure
  if ( sol.objective_value() > coeff.scalar_product(lb) )
    {
      for (unsigned int i=0; i!=mono.size(); ++i)
        if ( sol.is_set(i) )
          result.set( inst.get_variable(mono.real_index(i)) );
        else
          result.unset( inst.get_variable(mono.real_index(i)) );

      lb = result.image();
    }
  if ( sub_p.size() > 0 )
    {
      // solve the reduced problem
      dynamic_programming_single<mono_problem>::region_type
        region( coeff.scalar_product(lb) - red.guaranteed_profit(), false );
      dynamic_programming_single<mono_problem> solver(sub_p, region);

      mono_solution sub_s(sub_p);
      solver.run(sub_s);

      if ( sub_s.objective_value() > 0 )
        {
          red.restore_solution(sub_s, sol);

          for (unsigned int i=0; i!=mono.size(); ++i)
            if ( sol.is_set(i) )
              result.set( inst.get_variable(mono.real_index(i)) );
            else
              result.unset( inst.get_variable(mono.real_index(i)) );
        }
    }
} // lexicographic_solver::solve_mono()
