#include "HackTMConfig.h"
#include "HTMFunctions.h"

#include <cassert>
#include <algorithm>
#include <functional>

using namespace htmconfig;

namespace HackTM {
  unsigned averageReceptiveFieldSize(Region *r)
  {
    unsigned long long avg = 0; // No overflow check.
    for ( Region::column_iterator it = r->columns.begin(); it != r->columns.end(); it++ )
      avg += (*it)->proximalDendrite->getReceptiveFieldSize();
    return avg / r->getColumnSpace()->getSize();
  }

  void
  spatialPoolerInit(Region *r)
  {
    r->setInhibitionRadius(averageReceptiveFieldSize(r));
  }

  void
  spatialPooler(Region *r, const BitVector &input)
  {
    Region::column_iterator it;
    Region::activecol_iterator actit;

    // Phase 0: Reset.
    r->activeColumns.clear();

    // Phase 1: Overlap.
    r->updateColumnsOverlap(input);
    
    // Phase 2: Inhibition.
    r->inhibitColumns();

    // Phase 3: Learning
    r->adjustProximalSynapses(input);
    /* Boost Missing */
    r->setInhibitionRadius(averageReceptiveFieldSize(r));

    if ( 1 || hacktmdebug::Debug_PrintOverlappingColumns ) {
      Region::column_iterator it;
      for ( it = r->columns.begin(); it != r->columns.end(); it++ )
	std::cout << (*it)->getOverlap() << " ";
      std::cout << std::endl;
    }

    if ( 1 || hacktmdebug::Debug_PrintWinningColumns ) {
      Region::activecol_iterator it;
      std::cout << "And the winners are:\n";
      for ( it = r->activeColumns.begin(); it != r->activeColumns.end(); it++ ) {
	std::cout << (*it)->getId() << std::endl;
      }
      std::cout << std::endl;
    }
  }
}







