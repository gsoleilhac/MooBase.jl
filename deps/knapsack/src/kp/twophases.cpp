#include <kp/phase_1.hpp>
#include <kp/phase_2.hpp>
#include <kp/twophases.hpp>

#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
void twophases(int nb_obj, int sz, float *obj, float *weigths ,float capacity )
{
  kp::knapsack_instance inst( nb_obj, sz, obj, weigths, capacity );
  kp::phase_1<> phase_1(inst);

  phase_1.run();
  std::cout << "phase 1: done" << std::endl;

  std::vector<kp::knapsack_solution> XSEM
    ( phase_1.get_XSEM().begin(), phase_1.get_XSEM().end() );
  std::vector<kp::knapsack_solution> XSEm
    ( phase_1.get_XSEm().begin(), phase_1.get_XSEm().end() );

  std::cout << XSEm.size() << " supported extreme points." << std::endl;
  std::cout << XSEM.size() << " supported solutions." << std::endl;

  std::sort( XSEM.begin(), XSEM.end() );
  std::sort( XSEm.begin(), XSEm.end() );
  std::ofstream supported("supported.coord");
  std::ofstream supported_min("supported_min.coord");

  for (unsigned int i=0; i!=XSEM.size(); ++i)
    supported << XSEM[i] << '\n';

  for (unsigned int i=0; i!=XSEm.size(); ++i)
    supported_min << XSEm[i] << '\n';

  std::vector<kp::knapsack_solution> XNSEM;

  std::cout << "Checking XSEM" << std::endl;

  for( unsigned int i=0; i!=XSEM.size(); ++i )
    inst.check(XSEM[i]);

  // 2nd phase : ranking
  if ( XSEM.size() >= 2 )
    {
      kp::phase_2<> phase_2( inst, phase_1.get_XSEM(), phase_1.get_hull() );

      phase_2.run();

      XNSEM.insert
        ( XNSEM.begin(),
          phase_2.get_XNSEM().begin(), phase_2.get_XNSEM().end() );
      std::cout << XNSEM.size() << " not supported points." << std::endl;
    }
  else
    {
      std::cout << 0 << " not supported points." << std::endl;
      XNSEM = XSEM;
    }

  std::cout << "Checking XNSEM" << std::endl;

  for( unsigned int i=0; i!=XNSEM.size(); ++i )
    inst.check(XNSEM[i]);
  std::sort( XNSEM.begin(), XNSEM.end() );
  std::ofstream not_supported("efficient_solutions.txt");
  for (unsigned int i=0; i!=XNSEM.size(); ++i)
    not_supported << XNSEM[i] << '\n';
}
