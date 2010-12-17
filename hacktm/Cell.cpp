#include "HackTM.h"
#include "Cell.h"
#include "DendriteSegment.h"

#include <algorithm>

using namespace HackTM {

  DendriteSegment *Cell::getActiveSegments(htmtime_t t, segmentstate_t state);
  {
    DendriteSegment *s = NULL;
    dendrite_iterator it;
    for ( it = __segments.begin(); it != __segments.end(); it++ ) {

      if ( (*it)->getState(t) == inactiveState )
	continue;

      if ( (s == NULL)
	   || ((*it)->getState(t) > s->getState(t))
	   || ((*it)->getState(t) == s->getState(t) 
	       && (*it)->getActivity(t) > s->getActivity(t)) ) {
	s = *it;
      }
    }
    return s;
  }
    
  
    
}
