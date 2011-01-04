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
