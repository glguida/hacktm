/* HackTM - C++ implementation of Numenta Cortical Learning Algorithm.
 * Copyright (c) 2010-2011 Gianluca Guida <glguida@gmail.com>
 *  
 * This software is released under the Numenta License for
 * Non-Commercial Use. You should have received a copy of the Numenta
 * License for Non-Commercial Use with this software (usually
 * contained in a file called LICENSE.TXT). If you don't, you can
 * download it from 
 * http://numenta.com/about-numenta/Numenta_Non-Commercial_License.txt.
 */

#include "HackTM.h"
#include "ProximalDendrite.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"
#include "Space.h"
#include "Introspection.h"

#include <iostream>

namespace hacktm {

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
