#include "HackTM.h"
#include "ProximalDendrite.h"
#include "SpatialPooler.h"
#include "Space.h"
#include "Introspection.h"

#include <iostream>

namespace HackTM {

  Introspection::Introspection(SpatialPooler *sp)
    : spatialPooler(sp),
      dendrites(sp->__dendrites) {}

  void 
  Introspection::dumpSynapse(ProximalDendrite *p, struct synapse *syn)
  {
    unsigned dimension = p->__inputSpace->getDimension();
    for ( unsigned i = 0; i < dimension; i++ )
      std::cout << p->__inputSpace->getCoord(syn->id, i) << " ";
    std::cout << syn->perm << std::endl;
  }

  void
  Introspection::dumpPotentialSynapses(unsigned column)
  {
    ProximalDendrite *p = &spatialPooler->__dendrites[column];
    ProximalDendrite::synapse_iterator it;
    for ( it = p->__potentialSynapses.begin();
	  it != p->__potentialSynapses.end(); it++ )
      dumpSynapse(p, *it);
    std::cout << std::endl << std::endl;

  }

  void 
  Introspection::dumpAllPotentialSynapses()
  {
    unsigned columns = spatialPooler->__columnSpace->getSize();
    for ( unsigned i = 0; i < columns; i++ )
      dumpPotentialSynapses(i);
  }

}
