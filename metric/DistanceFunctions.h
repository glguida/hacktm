#ifndef __METRIC_DISTANCEFUNCTIONS_H__
#define __METRIC_DISTANCEFUNCTIONS_H__

#include "Vector.h"

namespace metric {

  typedef unsigned (DistanceFunction)(const Vector &x, const Vector &y);

  DistanceFunction Norm1;
  DistanceFunction Euclidean;
  DistanceFunction NormInf;

};

#endif
