#include <functional>
#include <algorithm>
#include "Vector.h"
#include "MetricSpace.h"
#include "rnd_normal.h"

/* TMP */
#include <iostream>

namespace metric {

  MetricSpace::MetricSpace(const Vector *max, const DistanceFunction &dist)
    : __distance(dist)
  {
    __dim = max->size();
    __max = new Vector(*max);
    __idProjector = new Vector(__dim);
    __size = VectorArea(*__max);
    //    __size = VectorArea(max);
    //    std::cout << __max[0] << " " << __max[1] << std::endl;

    std::cout << " size is " << __size << std::endl;
    std::cout << " size is " << VectorArea(*__max) << std::endl;
    std::cout << " size is " << VectorArea(*max) << std::endl;

    /*
     * Calculate __idProjector;
     */

    // get a vector [ 1 MaxX MaxY MaxZ ... ]
    Vector tmp = __max->shift(-1);
    tmp[0] = 1;

    // get a vector [ 1 MaxX MaxY*MaxX MaxZ*MaxY*MaxX ... ]
    std::partial_sum(VectorBegin(tmp), VectorEnd(tmp),
		     VectorBegin(*__idProjector),
		     std::multiplies<unsigned>());

    std::cout << "[";
    for ( unsigned i = 0; i < __idProjector->size(); i++ )
      std::cout << (*__idProjector)[i] << " ";
    std::cout << "]\n";

    std::cout << "Initializing a " << __dim << "-dimensional space.\n";

  }
  
  struct functor_normrnd {
    functor_normrnd(unsigned radius) : __r(radius) {}
    int operator()(int center)
    {
      return rnd_normal(center, __r);
    }
  private:
    int __r;
  };

  Vector
  MetricSpace::getNormalRandomVector(const Vector &c, unsigned r) const
  {
    Vector random(0, __dim);
    /* Note, the current function doesn't actually garantess that the
       random point is in the radius, only very close. (Actually, it's
       guaraneed in case you're using the normInf distance :-)) */
    do {
      std::transform(VectorConstBegin(c), VectorConstEnd(c), VectorBegin(random), functor_normrnd(r));
    } while ( !contains(random) );
    return random;
  }

  unsigned 
  MetricSpace::getNormalRandomId(unsigned c, unsigned r) const
  {
    Vector center = getVectorFromId(c);
    Vector random = getNormalRandomVector(center, r);
    return getIdFromVector(random);
  }

  unsigned
  MetricSpace::getDistance(unsigned id1, unsigned id2) const
  {
    return __distance(getVectorFromId(id1), getVectorFromId(id2));
  }
  unsigned
  MetricSpace::getDistance(Vector v1, Vector v2) const
  {
    return __distance(v1, v2);
  }

  struct functor_minmax {
    functor_minmax(bool max) : __isMax(max) {}
    int operator()(int a,int b) { return __isMax?std::max(a,b):std::min(a,b); }
  private:
    bool __isMax;
  };

  void
  MetricSpace::accumulateMinCoordinates(Vector &acc, unsigned id) const
  {
    Vector v = getVectorFromId(id);

    if ( acc.size() == 0 ) {
      acc.resize(__dim);
      acc = getVectorFromId(id);
      return;
    }

    std::transform(VectorBegin(acc), VectorEnd(acc), 
		   VectorBegin(v), VectorBegin(acc), functor_minmax(false));
  }

  void
  MetricSpace::accumulateMaxCoordinates(Vector &acc, unsigned id) const
  {
    Vector v = getVectorFromId(id);

    if ( acc.size() == 0 ) {
      acc.resize(__dim);
      acc = getVectorFromId(id);
      return;
    }

    std::transform(VectorBegin(acc), VectorEnd(acc), 
		   VectorBegin(v), VectorBegin(acc), functor_minmax(true));
  }

}

