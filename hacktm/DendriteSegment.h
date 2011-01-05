/*
 * Copyright (C) 2010 Gianluca Guida <glguida@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    learnState         /* activeState + learnState cell in active synapses. */
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

    friend class Introspection;
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
