#include <cmath>
#include <algorithm>
#include <valarray>
#include "Vector.h"
#include "DistanceFunctions.h"

using namespace std;

namespace metric {

  unsigned Norm1(const Vector &x, const Vector &y)
  {
    return (abs(x - y)).sum();
  }

  unsigned Euclidean(const Vector &x, const Vector &y)
  {
    Vector diff = x - y;
    return (unsigned)sqrtf((diff * diff).sum());
  }

  unsigned NormInf(const Vector &x, const Vector &y)
  {
    return (abs(x - y)).max();
  }

}
