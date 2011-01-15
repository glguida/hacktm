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

#ifndef __HACKTM_CELL_H__
#define __HACKTM_CELL_H__

/*
 * Class for the actual "Cell" that belongs to a column and connects
 * other cells via its distal dendrites. 
 */

#include "HackTM.h"
#include "DendriteSegment.h"

namespace hacktm {

  struct segmentUpdate {
    struct DendriteSegment *s;
    bool sequenceSegment;
    std::list<id_t> activeSynapses;
    std::list<id_t> newSynapses;
  };

  class Cell {
  public:
    void setup(id_t cid, CellsState *cellstate);
    DendriteSegment *getActiveSegment(htmtime_t t, segmentstate_t state);
    DendriteSegment *getBestMatchingSegment(htmtime_t t, unsigned int &maxactive);
    DendriteSegment *getBestMatchingSequenceSegment(htmtime_t t, unsigned &active);
    
    void getSegmentActiveSynapses(segmentUpdate *sUpdate,
				  const htmtime_t t,
				  bool newSynapses = false);
    bool getPredictiveState(htmtime_t cur, htmtime_t prev);
    void adaptSegments(bool positiveReinforcements);

    inline void addSegmentUpdateList(segmentUpdate *sUpdate) {
      __segmentUpdateList.push_front(sUpdate);
    }
    void deleteSegmentUpdateList() {
      __segmentUpdateList.clear();
    }
    inline unsigned getNumSegments() const
    {
      return __segments.size();
    }
    inline id_t getId() const {
      return __id;
    }

    typedef std::list<DendriteSegment *>::iterator segment_iterator;
    friend class Introspection;
  private:
    id_t                         __id;
    CellsState                  *__cellstate;
    std::list<DendriteSegment *> __segments;
    std::list<segmentUpdate *>   __segmentUpdateList;
  };

} // namespace
#endif
