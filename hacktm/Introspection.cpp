#include "HackTM.h"
#include "ProximalDendrite.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"
#include "Space.h"
#include "Introspection.h"

#include <iostream>

namespace HackTM {
  namespace IntrospectionLib {
    void
    dumpPotentialSynapses(const SpatialPooler *sp, unsigned column)
    {
      Introspection is;
      const ProximalDendrite *p = &(is.getSpatialPoolerProximalDendrites(sp)[column]);
      const Space *inputSpace = is.getProximalDendriteInputSpace(p);
      std::list<synapse*>::const_iterator it;
      for ( it = is.getProximalDendritePotentialSynapses(p).begin();
	    it != is.getProximalDendritePotentialSynapses(p).end(); it++ ) {
	for ( unsigned i = 0; i < inputSpace->getDimension(); i++ )
	  std::cout << inputSpace->getCoord((*it)->id, i) << " ";
	std::cout << (*it)->perm << std::endl;
      }
      std::cout << std::endl << std::endl;
    }
    
    void 
    dumpAllPotentialSynapses(const SpatialPooler *sp)
    {
      Introspection is;
      unsigned columns = is.getSpatialPoolerColumnSpace(sp)->getSize();
      for ( unsigned i = 0; i < columns; i++ )
	dumpPotentialSynapses(sp, i);
    }

    void
    dumpActiveCells(const TemporalPooler *tp, htmtime_t t)
    {
      Introspection is;
      const CellsState *cs = is.getTemporalPoolerCellsState(tp);
      
      std::cout << "Active State: " << std::endl;
      std::cout << *(is.getCellsStateActiveState(cs, t)) << std::endl;
    }

    void
    dumpPredictiveCells(const TemporalPooler *tp, htmtime_t t)
    {
      Introspection is;
      const CellsState *cs = is.getTemporalPoolerCellsState(tp);
      
      std::cout << "Predictive State: " << std::endl;
      std::cout << *(is.getCellsStatePredictiveState(cs, t)) << std::endl;
    }

    void
    dumpLearnCells_bitmap(const TemporalPooler *tp)
    {
      Introspection is;
      const CellsState *cs = is.getTemporalPoolerCellsState(tp);
      
      std::cout << "Learn State: " << std::endl;
      std::cout << *(is.getCellsStateLearnState(cs)) << std::endl;
    }
    void
    dumpLearnCells(const TemporalPooler *tp, htmtime_t t)
    {
      Introspection is;
      const CellsState *cs = is.getTemporalPoolerCellsState(tp);
      std::vector<id_t>::const_iterator it;

      std::cout << "Learn Cells:\n";
      for ( it = is.getCellsStateLearnCells(cs, t).begin();
	    it != is.getCellsStateLearnCells(cs, t).end(); it++ )
	std::cout << *it << " ";
      std::cout << std::endl;
    }
  }
}
