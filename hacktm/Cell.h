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
