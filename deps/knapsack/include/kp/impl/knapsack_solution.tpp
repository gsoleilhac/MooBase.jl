/**
 * \file knapsack_solution.tpp
 * \brief Implementation of the template methods of the kp::knapsack_solution
 *        class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param inst An instance of the knapsack problem.
 * \param s The status of the variables in the solution.
 */
template<typename View>
kp::knapsack_solution::knapsack_solution
( const View& inst, const moo::binary_solution& s )
  : m_solution(s), m_value(inst.get_nb_objectives())
{
  const unsigned int n = inst.get_size();

  for (unsigned int i=0; i!=n; ++i)
    {
      const variable& v = inst.get_variable(i);

      if ( m_solution[v.index] == moo::bin_set )
	{
	  m_value.cost += v.cost;
	  m_value.weight += v.weight;
	}
    }
} // knapsack_solution::knapsack_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param inst The problem from which we get the size and the number of
 *        objectives.
 */
template<typename View>
kp::knapsack_solution::knapsack_solution( const View& inst )
  : m_solution(inst.get_size()), m_value(inst.get_nb_objectives())
{

} // knapsack_solution::knapsack_solution()

