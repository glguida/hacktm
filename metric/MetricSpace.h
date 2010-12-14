#ifndef __METRIC_METRICSPACE_H__
#define __METRIC_METRICSPACE_H__

#include <cassert>
#include "Vector.h"
#include "Distance.h"

namespace metric {
  class MetricSpace {

  public:
    MetricSpace(const Vector *max, const Distance &dist);

    inline const unsigned size() const { return __size; }
    inline const unsigned dimensions() const { return __dim; }

    inline bool contains(unsigned id) const {
      return id < __size;
    }
    inline bool contains(const Vector &v) const {
      return (v >= 0).min() && (v < __max).min();
    }

    inline unsigned getIdFromVector(const Vector &v) const {
      assert ( contains(v) );
      return std::inner_product(VectorConstBegin(v), VectorConstEnd(v), 
				VectorConstBegin(__idProjector), 0);
    }
    inline Vector getVectorFromId(unsigned id) const {
      assert ( contains(id) );
      Vector tmp(id, __dim);
      tmp /= __idProjector;
      tmp %= __max;
      return tmp;
    }

    void accumulateMinCoordinates(Vector &acc, unsigned id) const;
    void accumulateMaxCoordinates(Vector &acc, unsigned id) const;

    Vector getNormalRandomVector(const Vector &c, unsigned r) const;
    unsigned getNormalRandomId(unsigned c, unsigned r) const;

    unsigned getDistance(unsigned id1, unsigned id2) const;
    unsigned getDistance(Vector &v1, Vector &v2) const;

    void getIdRangeContainingSphere(unsigned c, unsigned r,
				     unsigned &first, unsigned &last) const;

    inline bool operator>(const MetricSpace &LHS) const {
      return (this->__max > LHS.__max).min();
    }
    inline bool operator>=(const MetricSpace &LHS) const {
      return (this->__max >= LHS.__max).min();
    }
    inline bool operator<=(const MetricSpace &LHS) const {
      return (this->__max <= LHS.__max).min();
    }
    inline bool operator<(const MetricSpace &LHS) const {
      return (this->__max < LHS.__max).min();
    }
    inline bool operator==(const MetricSpace &LHS) const {
      return (this->__max == LHS.__max).min();
    }
    inline bool operator!=(const MetricSpace &LHS) const {
      return (this->__max != LHS.__max).min();
    }

    inline Vector operator/(const MetricSpace &LHS) const {
      return (this->__max / LHS.__max);
    }

  private:
    Vector __idProjector;
    Vector __max;
    unsigned __dim;
    unsigned __size;
    const Distance &__distance;
  };

}

#endif
