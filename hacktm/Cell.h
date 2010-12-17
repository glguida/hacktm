#ifndef __HACKTM_CELL_H__
#define __HACKTM_CELL_H__

/*
 * Class for the actual "Cell" that belongs to a column and connects
 * other cells via its distal dendrites. 
 */

#include "HackTM.h"
#nclude "Space.h"

namespace HackTM {

  class Cell {
  public:
    Cell(Space *cellSpace, id_t cellSpaceId, scalar_t learningRadius = htmconfig::learningRadius)
      : __space(cellSpace), __id(cellSpaceId), __radius(learningRadius) {}

    inline id_t getId() { return __id };
    DendriteSegment *getActiveSegments(htmtime_t t, segmentstate_t state);

    typedef std::list<DendriteSegment *>::iterator dendrite_iterator;
  private:
    const id_t __id;
    const Space *__space;
    const scalar_t __radius;
    std::list<DendriteSegment *> __segments;
  };

}
#endif
