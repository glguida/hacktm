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

#include "HackTM.h"
#include "TemporalPooler.h"

#include "Introspection.h"

#include <cassert>
#include <limits>
#include <numeric>

namespace hacktm {

  TemporalPooler::TemporalPooler(Space *columnspace, unsigned cellspercol)
  {
    unsigned cellno = columnspace->getSize() * cellspercol;
    __ciSpace = new CISpace(columnspace, cellspercol);

    __cellsState = new CellsState(__ciSpace->getSpace());

    __cells = new Cell[cellno];
    for ( unsigned i = 0; i < cellno; i++ )
      __cells[i].setup(i, __cellsState);

    __cellsPerColumn = cellspercol;
  }

  TemporalPooler::~TemporalPooler()
  {
    delete __ciSpace;
    delete __cellsState;
    delete [] __cells;
  }

  void TemporalPooler::run(htmtime_t cur, htmtime_t prev, const std::list<id_t> &actColumns, BitVector &output)
  {
    calculateActiveState(cur, prev, actColumns);
    calculatePredictiveState(cur, prev);
    learn(cur, prev);
    getOutput(cur, output);
  }

  void
  TemporalPooler::calculateActiveState(htmtime_t cur, htmtime_t prev, const std::list<id_t> &actColumns)
  {
    __cellsState->resetState(cur);

    std::list<id_t>::const_iterator it;
    for ( it = actColumns.begin(); it != actColumns.end(); it++ ) {
      bool buPredicted = false;
      bool lcChosen = false;
      id_t c = *it;

      for ( unsigned i = 0; i < __cellsPerColumn; i++ )
	if ( __predictiveState(c, i, prev) ) {
	  DendriteSegment *s = __getCell(c,i)->getActiveSegment(prev, activeState);
	  assert ( s != NULL 
		   && "Cell predictive but no active segments found!" );
	  if ( s->isSequenceSegment() ) {
	    buPredicted = true;
	    __setActiveState(c, i, cur);

	    if ( s->isSegmentActive(prev, learnState) ) {
	      lcChosen = true;
	      __setLearnState(c, i, cur);
	    }
	  }
	}

      if ( !buPredicted )
	for ( unsigned i = 0; i < __cellsPerColumn; i++ )
	  __setActiveState(c, i, cur);

      if ( !lcChosen  ) {
	segmentUpdate *sUpdate = new segmentUpdate;
	Cell *cell = __getBestMatchingCell(c, prev, sUpdate->s);
	__cellsState->setLearnState(cell->getId(), cur);
	cell->getSegmentActiveSynapses(sUpdate, prev, true);
	sUpdate->sequenceSegment = true;
	cell->addSegmentUpdateList(sUpdate);
      }
    }
  }

  void
  TemporalPooler::calculatePredictiveState(htmtime_t cur, htmtime_t prev)
  {
    for ( id_t cid = 0; cid < __ciSpace->getSpace()->getSize(); cid++ ) {
      if ( __cellsState->activeState(cid, cur) )
	continue;
      if ( __cells[cid].getPredictiveState(cur, prev) )
	__cellsState->setPredictiveState(cid, cur);
    }
  }

  void
  TemporalPooler::learn(htmtime_t cur, htmtime_t prev)
  {
    for ( id_t cid = 0; cid < __ciSpace->getSpace()->getSize(); cid++ ) {
      if ( __cellsState->learnState(cid) )
	__cells[cid].adaptSegments(true);
      else if ( !__cellsState->predictiveState(cid, cur)
		&& __cellsState->predictiveState(cid, prev) ) {
	__cells[cid].adaptSegments(false);
      }
    }
  }

  void
  TemporalPooler::getOutput(htmtime_t cur, BitVector &output)
  {
    __cellsState->copyActiveState(cur, output);
    __cellsState->orPredictiveState(cur, output);
  }

  Cell *
  TemporalPooler::__getBestMatchingCell(id_t c, htmtime_t t, DendriteSegment *&segment)
  {
    Cell *bestcell = NULL, *cell;
    unsigned bestactive = 0, active;
    DendriteSegment *bestsegm = NULL, *s;
    for ( unsigned i = 0; i < __cellsPerColumn; i++ ) {
      cell = __getCell(c, i);
      s = cell->getBestMatchingSequenceSegment(t, active);
      if ( s != NULL && active > bestactive ) {
	bestsegm = s;
	bestcell = cell;
	bestactive = active;
      }
    }

    if ( bestsegm != NULL ) {
      segment = bestsegm;
      assert ( bestcell != NULL );
      return bestcell;
    }

    unsigned minsegments = std::numeric_limits<unsigned>::max(), numsegments;
    for ( unsigned i = 0; i < __cellsPerColumn; i++ ) {
      cell = __getCell(c, i);
      numsegments = cell->getNumSegments();
      if ( numsegments < minsegments ) {
	minsegments = numsegments;
	bestcell = cell;
      }
    }
    assert ( bestcell != NULL );
    segment = NULL;
    return bestcell;
  }

}
