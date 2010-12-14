#ifndef __METRIC_DISTANCE_H
#define __METRIC_DISTANCE_H

#include <cmath>
#include <algorithm>
#include "Vector.h"

namespace metric {

  class Distance {
  public:
    unsigned getDistance(const Vector &x, const Vector &y) const {
      return __dist(x,y);
    }

    // Hint to give a range of id that contains (do not match) the spere.
    /* XXX: Since only the metric space knows about the mapping
       between the vector space and the ID-space, we are here making
       an assumpion on the order of __idProjector. */
    void getSphericalRange(const Vector &c, unsigned radius,
			     Vector &first, Vector &last) const {
      return __idrange(c, radius, first, last);
    }
  private:
    virtual unsigned __dist(const Vector &x, const Vector &y) const = 0;
    virtual void __idrange(const Vector &c, unsigned radius, 
			   Vector &first, Vector &last) const = 0;
  };

  class OneNormDist : public Distance {
    virtual unsigned __dist(const Vector &x, const Vector &y) const {
      return (abs(x - y)).sum();
    }
    virtual void __idrange(const Vector &c, unsigned radius,
			   Vector &first, Vector &last) const {
      /* Not too hard to understand this once you see the shape. */
      first = c; first[c.size() - 1] = first[c.size() - 1] - (int)radius;
      last = c; last[c.size() - 1] = last[c.size() - 1] + (int)radius;
    }
  };
    
  class EuclideanDist : public Distance {
    virtual unsigned __dist(const Vector &x, const Vector &y) const {
      Vector diff = x - y;
      return (unsigned)sqrtf((diff * diff).sum());
    }
    virtual void __idrange(const Vector &c, unsigned radius,
			   Vector &first, Vector &last) const {
      /* We can approximate to the cubic edge here. */
      first = c; first -= (int)radius;
      last = c; last += (int)radius;
    }
  };

  class InfNormDist : public Distance {
    virtual unsigned __dist(const Vector &x, const Vector &y) const {
      return (abs(x - y)).max();
    }
    virtual void __idrange(const Vector &c, unsigned radius,
			   Vector &first, Vector &last) const {
      first = c; first -= (int)radius;
      last = c; last += (int)radius;
    }
  };
}

#endif
