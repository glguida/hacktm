#ifndef __METRIC_SPHERE_H__
#define __METRIC_SPHERE_H__

#include "BitVector.h"
#include "MetricSpace.h"

namespace metric {

  class Sphere {
  private:
    class SphereRandomIterator {
    public:
      inline SphereRandomIterator& operator++() {
	__getNextRandom();
	return *this;
      }
      inline SphereRandomIterator& operator++(int) {
	SphereRandomIterator &tmp = *this;
	++*this;
	return tmp;
      }
      inline unsigned operator*() {
	return __curId;
      }
      inline bool operator==(const SphereRandomIterator &RHS) {
	return (__curId == RHS.__curId);
      }
      inline bool operator!=(const SphereRandomIterator &RHS) {
	return !(*this == RHS);
      }
      
      SphereRandomIterator(const Sphere *s)
	: __sphere(s), __bitmap(BitVector(s->__metricSpace->size()))
      {
	__getNextRandom();
      }
      
    private:
      unsigned __curId;
      const Sphere *__sphere;
      BitVector __bitmap;
      
      void __getNextRandom()
      {
	unsigned random;
	do {
	  random = __sphere->getNormalRandomPoint();
	} while( __bitmap.test(random) == true );
	__bitmap.set(random);
	__curId = random;
      }
    };
    
    class SphereIterator {
    public:
      inline SphereIterator& operator++() {
	__incCurId();
	__advanceToNext();
	return *this;
      }
      inline SphereIterator operator++(int) {
	SphereIterator tmp = *this;
	++*this;
	return tmp;
      }
      inline unsigned operator*() const {
	return __curId;
      }
      inline bool operator==(const SphereIterator &RHS) {
	if ( __atEnd && RHS.__atEnd )
	  return true;
	return ( __atEnd == RHS.__atEnd && __curId == RHS.__curId );
      }
      inline bool operator!=(const SphereIterator &RHS) {
	return !(*this == RHS);
      }

      inline SphereIterator& operator=(const SphereIterator &RHS) {
	__sphere = RHS.__sphere;
	__curId = RHS.__curId;
	__maxId = RHS.__maxId;
	__atEnd = RHS.__atEnd;
	return *this;
      }

      SphereIterator() : __sphere(NULL) {}

      SphereIterator(const Sphere *RHS, bool end = false)
	: __sphere(RHS)
      {
	__atEnd = end;
	if ( __atEnd )
	  return;
	/*
	  We have to scan now the space to find elements that are in
	  the radius of the sphere. The most generic approach for
	  unusual distances is to sweep the whole space. This is of
	  course inefficient. Ask the sphere, that knows more about
	  the geometry of the space in what interval to search.
	*/
	/*
	  For standard norms, we could just check for the square
	  between <min(x),min<y>,...> and <max(x),max(y),..>. This is
	  by far the best engineering solution, but being this my pet
	  project I want to honour my Lobacevski reading nights and my
	  geometry-freak days and get the searching edges based on the
	  distance implemented.
	*/
	__sphere->__metricSpace->getIdRangeContainingSphere(__sphere->__center,
							    __sphere->__radius,
							    __curId, __maxId);
	__advanceToNext();
      }
      
    private:
      bool __atEnd;
      unsigned __curId;
      unsigned __maxId;
      const Sphere *__sphere;

      inline void __incCurId()
      {
	if ( ++__curId >= __sphere->__metricSpace->size()
	     || __curId >= __maxId )
	  __atEnd = true;
      }
      inline void __advanceToNext()
      {
	while ( !__atEnd && 
		__sphere->getDistance(__curId) > __sphere->getRadius() )
	  __incCurId();
      }
    };
    
    unsigned __center;
    unsigned __radius;
    const MetricSpace *__metricSpace;

  public:
    Sphere() : __metricSpace(NULL) {}
    Sphere(const MetricSpace *space, unsigned center, unsigned radius)
      : __metricSpace(space), __center(center), __radius(radius) {}

    inline unsigned getDistance(unsigned id) const {
      return __metricSpace->getDistance(id, __center);
    }
    inline unsigned getRadius() const {
      return __radius;
    }
    inline unsigned getCenter() const {
      return __center;
    }

    inline unsigned getNormalRandomPoint() const {
      return __metricSpace->getNormalRandomId(__center, __radius);
    }

    typedef SphereIterator iterator;
    inline iterator begin() const {
      return SphereIterator(this);
    }
    inline iterator end() const {
      return SphereIterator(this, true);
    }

    typedef SphereRandomIterator random_iterator;
    inline random_iterator getNormalDistributionIterator() const {
      return SphereRandomIterator(this);
    }
  };
}

#endif
