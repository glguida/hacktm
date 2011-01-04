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
