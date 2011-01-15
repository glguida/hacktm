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

#include "HackTM.h"
#include "Space.h"
#include "CISpace.h"

namespace hacktm {

CISpace::CISpace(Space *columnspace, unsigned cells)
{
  unsigned coldim = columnspace->getDimension();
  /* create a space of dimensions (dim(ColumnSpace) + 1) */
  Vector max;
  for ( unsigned i = 0; i < coldim; i++ )
    max.push_back(columnspace->getMaxCoord(i));
  max.push_back(cells);
  __space = new Space(max);

  /* ProjectorValue of the cell coord (that is, last one). */
  __cellIdProjectorValue = __space->getIdProjectorValue(max.size() - 1);
}

CISpace::~CISpace()
{
  delete __space;
}

} // namespace
