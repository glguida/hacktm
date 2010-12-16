#include "Region.h"
#include "Space.h"

using namespace HackTM;

main()
{
  Vector input(2, 1000);
  Vector col(2);
  col[0] = 20;
  col[1] = 30;

  Space inputSpace(input), columnSpace(col);
  Region r(&inputSpace, &columnSpace);

  for ( Region::column_iterator it = r.columns.begin(); it != r.columns.end(); it++ ) {
    Column *c = *it;
    std::list<struct synapse *>::iterator synit;

    for ( synit = c->proximalDendrite->potentialBegin(); 
	  synit != c->proximalDendrite->potentialEnd();
	  synit++ ) {
      Vector v( inputSpace.getDimensions() );
      inputSpace.setVectorFromId((*synit)->id, v);
      std::cout << v[0] << " " << v[1] << " " << (*synit)->perm << std::endl;
    }

    std::cout << std::endl << std::endl;
    
  }
}

