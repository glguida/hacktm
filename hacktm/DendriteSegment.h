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

namespace HackTM {

  typedef enum {
    inactiveState = 0, /* Below the threshold. */
    activeState,       /* Over the threshold. */
    learnState,        /* activeState + learnState cell in active synapses. */
  } segmentstate_t;

  class DendriteSegment {
  public:
    inline segmentstate_t getState(htmtime_t t) { return __state(t); }
    inline unsigned getActivity(htmtime_t t) { return __activity(t); }

    inline bool isSequenceSegment() { return __sequence; }
    inline bool isSegmentActive(htmtime_t t, segmentstate_t state) {
      return (getState(t) >= state);
    }

    void computeState(htmtime_t t);
    void update(segmentUpdate *su, bool positiveReinforcement);
  private:
    bool __sequence = false; /* This segment is a "sequence segment", meaning
				that it is believed that when it is active the
				cell will be active at next input. */
    
    enum segmentStatus __state[HTMTIME_MAX];
    unsigned __activity[HTMTIME_MAX];
    
    std::list<struct synapse *> __connectedSynapses;
    std::list<struct synapse *> __potentialSynapses;
  }
}
#endif
