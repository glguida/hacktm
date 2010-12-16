#ifndef __HTM_FUNCTIONS_H__
#define __HTM_FUNCTIONS_H__

#include "HackTM.h"

namespace HackTM {
  void spatialPooler(Region *r, const BitVector &input);
  void spatialPoolerInit(Region *r);
}

#endif
