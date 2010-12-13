#include "HackTMConfig.h"
#include "Column.h"
#include "BitVector.h"


namespace HackTM {

  Column::~Column()
  {
    if ( proximalDendrite != NULL )
      delete proximalDendrite;
  }

  void Column::updateOverlap(const BitVector &input)
  {
      __overlap = proximalDendrite->getOverlap(input);
  }

}
