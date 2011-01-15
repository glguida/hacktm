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

#ifndef __HACKTM_INTROSPECTION_H__
#define __HACKTM_INTROSPECTION_H__

/*
 * This class is used to gather statistics and display private
 * information of the system.
 */

#include "HackTM.h"
#include "Space.h"

#include "SpatialPooler.h"
#include "ProximalDendrite.h"
#include "TemporalPooler.h"

namespace hacktm {

  class Introspection {
  public:

    inline const Space *
    getProximalDendriteInputSpace(const ProximalDendrite *p) const
    {
      return p->__inputSpace;
    }
    
    inline const std::list<synapse *>&
    getProximalDendriteConnectedSynapses(const ProximalDendrite *p) const
    {
      return p->__connectedSynapses;
    }
    
    inline const std::list<synapse *>&
    getProximalDendritePotentialSynapses(const ProximalDendrite *p) const
    {
      return p->__potentialSynapses;
    }
    
    inline const scalar_t
    getSpatialPoolerInhibitionRadius(const SpatialPooler *sp) const
    {
      return sp->__inhibitionRadius;
    }
    
    inline const unsigned *
    getSpatialPoolerColumnsOverlaps(const SpatialPooler *sp) const
    {
      return sp->__columnsOverlap;
    }
    
    inline const ProximalDendrite *
    getSpatialPoolerProximalDendrites(const SpatialPooler *sp) const
    {
      return sp->__dendrites;
    }
    
    inline const Space *
    getSpatialPoolerInputSpace(const SpatialPooler *sp) const
    {
      return sp->__inputSpace;
    }
    
    inline const Space *
    getSpatialPoolerColumnSpace(const SpatialPooler *sp) const
    {
      return sp->__columnSpace;
    }
    
    inline const SpaceTransform *
    getSpatialPoolerInputToColumnTransform(const SpatialPooler *sp) const
    {
      return sp->__inputToColumn;
    }

    inline const CISpace *
    getTemporalPoolerCISpace(const TemporalPooler *tp) const
    {
      return tp->__ciSpace;
    }

    inline const Cell *
    getTemporalPoolerCells(const TemporalPooler *tp) const 
    {
      return tp->__cells;
    }

    inline const CellsState *
    getTemporalPoolerCellsState(const TemporalPooler *tp) const
    {
      return tp->__cellsState;
    }

    inline const std::list<DendriteSegment *>& 
    getCellDendriteSegments(const Cell *c) const
    {
      return c->__segments;
    }

    inline const BitVector *
    getCellsStateActiveState(const CellsState *cs, htmtime_t t) const
    {
      return cs->__activeState[t];
    }

    inline const BitVector *
    getCellsStatePredictiveState(const CellsState *cs, htmtime_t t) const
    {
      return cs->__predictiveState[t];
    }

    inline const BitVector *
    getCellsStateLearnState(const CellsState *cs) const
    {
      return cs->__learnState;
    }

    inline const std::vector<id_t>&
    getCellsStateLearnCells(const CellsState *cs, htmtime_t t) const
    {
      return cs->__learnCells[t];
    }

    inline const std::list<struct synapse *> &
    getDendriteSegmentPotentialSynapses(const DendriteSegment *s) const
    {
      return s->__potentialSynapses;
    }
  };

  namespace IntrospectionLib {
    void dumpPotentialSynapses(const SpatialPooler *sp, unsigned column);
    void dumpAllPotentialSynapses(const SpatialPooler *sp);
    void dumpActiveCells(const TemporalPooler *tp, htmtime_t t);
    void dumpPredictiveCells(const TemporalPooler *tp, htmtime_t t);
    void dumpLearnCells(const TemporalPooler *tp, htmtime_t t);
    void dumpLearnCells_bitmap(const TemporalPooler *tp);
  }

} // namespace

#endif
