#ifndef __METRIC_VECTOR_H__
#define __METRIC_VECTOR_H__

#include <valarray>
#include <functional>


namespace metric {

  typedef std::valarray<int> Vector;

  inline const int *VectorConstBegin(const Vector &v) {
    return &(v[0]);
  }
  inline const int *VectorConstEnd(const Vector &v) {
    return &(v[v.size()]);
  }
  inline int *VectorBegin(Vector &v) {
    return &(v[0]);
  }
  inline int *VectorEnd(Vector &v) {
    return &(v[v.size()]);
  }
  inline int VectorArea(const Vector &v)
  { 
    std::accumulate(&v[0], &v[v.size()], 1, std::multiplies<int>());
  }
}

#endif
