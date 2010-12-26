#ifndef __HACKTM_CISPACE_H__
#define __HACKTM_CISPACE_H__

#include "HackTM.h"
#include "Space.h"
#include "BitVector.h"

/*
 * Column-IndexOfCell Spaces.
 */

namespace HackTM {

  class CISpace {
  public:
    CISpace(Space *columnspace, unsigned cells);
    ~CISpace();

    inline id_t getIdFromCI(id_t column, unsigned i) const
    {
      /* The following is explained by the fact that 'column' is
	 aready an id for the first n-1 dimension of this space. */
      return column + i * __cellIdProjectorValue;
    }

    inline Space *getSpace() const {
      return __space;
    }

  private:
    unsigned __cellIdProjectorValue;
    Space *__space;
  };

}

#endif
