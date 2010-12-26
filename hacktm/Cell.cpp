#include "HackTM.h"
#include "Cell.h"
#include "DendriteSegment.h"

#include <algorithm>

namespace HackTM {

  void
  Cell::setup(id_t id, CellsState *cstate)
  {
    __id = id;
    __cellstate = cstate;
  }

  DendriteSegment *
  Cell::getActiveSegment(htmtime_t t, segmentstate_t state)
  {
    DendriteSegment *s = NULL;
    segment_iterator it;
    for ( it = __segments.begin(); it != __segments.end(); it++ ) {

      if ( (*it)->getState(t) == inactiveState )
	continue;

      /* Early exit if we find a sequence segment. */
      if ( (*it)->isSequenceSegment() ) {
	s = *it;
	return s;
      }

      if ( (s == NULL)
	   || ((*it)->getState(t) >= activeState
	       && (*it)->getActivity(t) > s->getActivity(t)) ) {
	s = *it;
      }
    }
    return s;
  }

  DendriteSegment*
  Cell::getBestMatchingSegment(htmtime_t t, unsigned &active)
  {
    DendriteSegment *s = NULL;
    unsigned maxactive = 0;
    segment_iterator it;
    for ( it = __segments.begin(); it != __segments.end(); it++ ) {
      unsigned cur = (*it)->getMatchingSynapses(t);
      if ( cur > maxactive ) {
	maxactive = cur;
	s = (*it);
      }
    }
    active = maxactive;
    return s;
  }

  void 
  Cell::getSegmentActiveSynapses(segmentUpdate *sUpdate,
				 const htmtime_t t,
				 bool newSynapses)				 
  {
    DendriteSegment *s = sUpdate->s;
    assert ( s != NULL || newSynapses );
    if ( s != NULL )
	s->getActiveSynapses(t, sUpdate->activeSynapses);

    if ( newSynapses ) {
      int newsyns = htmconfig::newSynapseCount - sUpdate->activeSynapses.size();
      if ( newsyns > 0 )
	__cellstate->getRandomLearnCells(t, newsyns, sUpdate->newSynapses);
    }
  }

  void
  Cell::adaptSegments(bool positiveReinforcement)
  {
    std::list<segmentUpdate *>::iterator it;
    for (it = __segmentUpdateList.begin(); it != __segmentUpdateList.end(); it++ ) {
      segmentUpdate *sup = *it; 
      if ( sup->s == NULL ) {
	if ( !positiveReinforcement ) {
	  delete sup;
	  continue;
	}
	sup->s = new DendriteSegment(__cellstate);
	__segments.push_front(sup->s);
      }
      sup->s->synapseReinforcement(sup->activeSynapses, sup->sequenceSegment, positiveReinforcement);
      if ( positiveReinforcement ) {
	sup->newSynapses.sort();
	sup->s->addSynapses(sup->newSynapses);
      }
      delete sup;
    }
    __segmentUpdateList.clear();
  }
  
  bool
  Cell::getPredictiveState(htmtime_t cur, htmtime_t prev)
  {
    bool ret = false;
    segment_iterator it;
    for ( it = __segments.begin(); it != __segments.end(); it++ ) {
      DendriteSegment *s = *it;
      s->computeState(cur);
      if ( s->isSegmentActive(cur, activeState) ) {
	segmentUpdate *sUpdate = new segmentUpdate;
	sUpdate->s = s;
	sUpdate->sequenceSegment = false;
	getSegmentActiveSynapses(sUpdate, cur, false);
	addSegmentUpdateList(sUpdate);

	unsigned unused;
	segmentUpdate *predUpdate = new segmentUpdate;
	predUpdate->s = getBestMatchingSegment(prev, unused);
	predUpdate->sequenceSegment = false;
	getSegmentActiveSynapses(predUpdate, prev, true);
	addSegmentUpdateList(predUpdate);

	ret = true;
      }
    }

    return ret;
  }
}
