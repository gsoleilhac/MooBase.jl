/**
 * \file dynamic_programming.cpp
 * \brief Implementation of the kp::dynamic_programming class.
 * \author Julien Jorge
 */
#include "kp/dynamic_programming.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the efficient points for a given instance.
 * \param inst The instance to solve.
 * \param Z The efficient points computed.
 */
void kp::dynamic_programming::solve
( const knapsack_instance& inst, pareto_set& Z ) const
{
  const unsigned int capacity = (unsigned int)inst.get_capacity();
  const unsigned int layer_size = capacity + 1;

  std::vector<pareto_set*> layer(layer_size, NULL);
  unsigned int w;

  layer[0] = new pareto_set;
  layer[0]->insert( cost_vector(inst.get_nb_objectives(), 0) );

  for ( unsigned int i=0; i!=inst.get_size(); ++i )
    {
      const variable& v( inst.get_variable(i) );

      std::vector<pareto_set*> new_layer(layer);

      for ( w=layer_size; w != 0; )
	{
	  --w;
	  unsigned int new_w = w + (unsigned int)v.weight;

	  if ( layer[w] != NULL )
	    if ( new_w <= capacity )
	      {
		if ( new_layer[new_w] == NULL )
		  new_layer[new_w] = new pareto_set;

		pareto_set::const_iterator it;

		for ( it=layer[w]->begin(); it!=layer[w]->end(); ++it )
		  new_layer[new_w]->insert( *it + v.cost );
	      }
	}

      std::swap(layer, new_layer);
    }

  for ( w=0; w!=layer_size; ++w )
    if ( layer[w] != NULL )
      {
	Z.insert( layer[w]->begin(), layer[w]->end() );
	delete layer[w];
      }
} // dynamic_programming::solve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute a minimum complete set for a given instance.
 * \param inst The instance to solve.
 * \param Xm The solutions computed.
 */
void kp::dynamic_programming::solve
( const knapsack_instance& inst, minimum_set& Xm ) const
{
  const unsigned int capacity = (unsigned int)inst.get_capacity();
  const unsigned int layer_size = capacity + 1;

  std::vector<minimum_set*> layer(layer_size, NULL);
  unsigned int w;

  layer[0] = new minimum_set;

  knapsack_solution sol(inst);
  for (unsigned int i=0; i!=inst.get_size(); ++i)
    sol.unset( inst.get_variable(i) );

  layer[0]->insert( sol );

  for ( unsigned int i=0; i!=inst.get_size(); ++i )
    {
      const unsigned int v_weight( (unsigned int)inst.get_variable(i).weight );

      std::vector<minimum_set*> new_layer(layer);

      for ( w=layer_size; w != 0; )
	{
	  --w;
	  const unsigned int new_w = w + v_weight;

	  if ( layer[w] != NULL )
	    if ( new_w <= capacity )
	      {
		if ( new_layer[new_w] == NULL )
		  new_layer[new_w] = new minimum_set;

		minimum_set::const_iterator it;

		for ( it=layer[w]->begin(); it!=layer[w]->end(); ++it )
		  {
		    sol = *it;
		    sol.set( inst.get_variable(i) );
		    new_layer[new_w]->insert( sol );
		  }
	      }
	}

      std::swap(layer, new_layer);
    }

  for ( w=0; w!=layer_size; ++w )
    if ( layer[w] != NULL )
      {
	Xm.insert( layer[w]->begin(), layer[w]->end() );
	delete layer[w];
      }
} // dynamic_programming::solve()
