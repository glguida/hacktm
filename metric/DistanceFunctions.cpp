#include <cmath>
#include <algorithm>
#include <valarray>
#include "Vector.h"
#include "DistanceFunctions.h"

using namespace std;

namespace metric {

  unsigned Norm1(Vector x, Vector y)
  {
    return (abs(x - y)).sum();
  }

  unsigned Euclidean(Vector x, Vector y)
  {
    Vector diff = x - y;
    return (unsigned)sqrtf((diff * diff).sum());
  }

  unsigned NormInf(Vector x, Vector y)
  {
    return (abs(x - y)).max();
  }

}
