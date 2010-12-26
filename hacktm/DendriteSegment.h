#ifndef __HACKTM_DENDRITE_SEGMENT_H__
#define __HACKTM_DENDRITE_SEGMENT_H__

/*
 * Distal Dendrites Segments, or simply, "Segments".
 *
 * In this implementation, pretty much a synapse container with a
 * state.
 */

#include "HackTM.h"
#include "HackTMTime.h"
#include "Space.h"
#include "CellsState.h"

#include <list>

namespace hacktm {

  typedef enum {
    inactiveState = 0, /* Below the threshold. */
    activeState,       /* Over the threshold. */
    learnState,        /* activeState + learnState cell in active synapses. */
  } segmentstate_t;

  class DendriteSegment {
  public:
    DendriteSegment(CellsState *cstate) : __cellstate(cstate) {}
    inline segmentstate_t getState(htmtime_t t) const { return __state[t]; }
    inline unsigned getActivity(htmtime_t t) const { return __activity[t]; }

    inline bool isSequenceSegment() const { return __sequence; }
    inline bool isSegmentActive(htmtime_t t, segmentstate_t state) const {
      return (getState(t) >= state);
    }

    void computeState(htmtime_t t);
    unsigned getMatchingSynapses(htmtime_t t) const;
    void getActiveSynapses(htmtime_t t, std::list<id_t> &activeSynapses) const;
    void synapseReinforcement(std::list<id_t> &activeSynapses, bool, bool);
    void addSynapses(std::list<id_t> &newSynapses);
    //    void update(segmentUpdate *su, bool positiveReinforcement);
  private:
    bool __sequence; /* This segment is a "sequence segment", meaning
		       	that it is believed that when it is active the
		       	column will be activated by the next input. */
    
    segmentstate_t    __state[HTMTIME_MAX];
    unsigned          __activity[HTMTIME_MAX]; /* XXX?: Is activity really needed? */
    const CellsState *__cellstate;
    
    std::list<struct synapse *> __connectedSynapses;
    std::list<struct synapse *> __potentialSynapses;
  };

} // namespace
#endif
