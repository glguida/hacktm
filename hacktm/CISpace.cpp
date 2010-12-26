#include "HackTM.h"
#include "Space.h"
#include "CISpace.h"

using namespace HackTM;

CISpace::CISpace(Space *columnspace, unsigned cells)
{
  unsigned coldim = columnspace->getDimension();
  /* create a space of dimensions (dim(ColumnSpace) + 1) */
  Vector max;
  for ( unsigned i = 0; i < coldim; i++ )
    max.push_back(columnspace->getMaxCoord(i));
  max.push_back(cells);
  __space = new Space(max);

  for ( unsigned i = 0; i < __space->getDimension(); i++ )
    std::cout << "["<<i<<"] "<< __space->getIdProjectorValue(i) << " ";
  std::cout << std::endl;

  /* ProjectorValue of the cell coord (that is, last one). */
  __cellIdProjectorValue = __space->getIdProjectorValue(max.size() - 1);
  std::cout << " idprojv = " << __cellIdProjectorValue << " size: " << __space->getSize() << std::endl;
}

CISpace::~CISpace()
{
  delete __space;
}

