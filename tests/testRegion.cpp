
#include "Region.h"

using namespace metric;
using namespace HackTM;

main()
{
  Vector input(1000,2);
  Vector col(2);
  col[0] = 20;
  col[1] = 30;

  MetricSpace inputSpace(&input), columnSpace(&col);
  Region r(&inputSpace, &columnSpace);

  for ( Region::column_iterator it = r.columns.begin(); it != r.columns.end(); it++ ) {
    Column *c = *it;
    std::list<struct synapse *>::iterator synit;

    for ( synit = c->proximalDendrite->potentialBegin(); 
	  synit != c->proximalDendrite->potentialEnd();
	  synit++ ) {
      Vector v = inputSpace.getVectorFromId((*synit)->id);
      std::cout << v[0] << " " << v[1] << " " << (*synit)->perm << std::endl;
    }

    std::cout << std::endl << std::endl;
    
  }
}

