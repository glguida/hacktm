#ifndef __HACKTM_CELL_H__
#define __HACKTM_CELL_H__

/*
 * Class for the actual "Cell" that belongs to a column and connects
 * other cells via its distal dendrites. 
 */

#include "HackTM.h"
#include "DendriteSegment.h"

namespace HackTM {

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
  private:
    id_t                         __id;
    CellsState                  *__cellstate;
    std::list<DendriteSegment *> __segments;
    std::list<segmentUpdate *>   __segmentUpdateList;
  };

}
#endif
