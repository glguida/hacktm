/* HackTM - C++ implementation of Numenta Cortical Learning Algorithm.
 * Copyright (c) 2010-2011 Gianluca Guida <glguida@gmail.com>
 *  
 * This software is released under the Numenta License for
 * Non-Commercial Use. You should have received a copy of the Numenta
 * License for Non-Commercial Use with this software (usually
 * contained in a file called LICENSE.TXT). If you don't, you can
 * download it from 
 * http://numenta.com/about-numenta/Numenta_Non-Commercial_License.txt.
 */

#ifndef __HACKTM_CISPACE_H__
#define __HACKTM_CISPACE_H__

#include "HackTM.h"
#include "Space.h"
#include "BitVector.h"

/*
 * Column-IndexOfCell Spaces.
 */

namespace hacktm {

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

} // namespace

#endif
