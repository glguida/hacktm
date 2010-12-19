#ifndef __HACKTM_SPACE_H__
#define __HACKTM_SPACE_H__

#include <cassert>
#include <vector>
#include <numeric>

#include "HackTM.h"
#include "Introspection.h"

namespace HackTM {

  /*
   * Space.
   *
   * This class defines a space, which can be of any dimension, and
   * most importantly provides function to map this euclidean,
   * n-dimensional space to a one-dimensional space (read bitmap)
   * defined by id_t.
   * The advantage of this class is that it lets the client to handle
   * object in multiple dimension without the hassle of handling
   * vectors. Whose vectors, btw, would tend to easily stress the
   * already sensible C++ allocator, so this implementation gives also
   * a noticeable performance gain.
   */

  class Space {
  public:
    Space(const Vector &max);

    inline bool contains(id_t id) const { return id < __size; }
    inline bool contains(Vector v) const
    {
      for (unsigned i = 0; i < __dimension; i++ )
	if ( v[i] < 0 || v[i] >= __maxCoordinates[i] )
	  return false;
      return true;
    }

    inline coord_t getCoord(id_t id, unsigned i) const
    {
      return ((id/__idProjector[i]) % __maxCoordinates[i]);
    }


    inline id_t getIdFromVector(Vector &v) const
    {
      return std::inner_product(v.begin(), v.end(),
				__idProjector.begin(), 0);
    }
    Vector &setVectorFromId(id_t id, Vector &v) const;
    scalar_t getDistance(id_t id1, id_t id2) const;
    
    inline const unsigned getSize() const { return __size; }
    inline const scalar_t getMaxSide() const { return __maxSide; }
    inline const unsigned getDimension() const { return __dimension; }
    inline coord_t getMaxCoord(int i) const { return __maxCoordinates[i]; }
    inline coord_t getIdProjectorValue(int i) const  { return __idProjector[i]; }

    friend class Introspection;
  private:
    unsigned       __size;
    scalar_t       __maxSide;
    Vector         __idProjector;
    Vector         __maxCoordinates;
    const unsigned __dimension;
  };


  /*
   * SubSpace class.
   *
   * This class provides functions to operate on a subspace (defined
   * by a center and a radius) of a space. It actually provides you
   * the n-Cube, so the radius is the minimal distance between the
   * center and the sides, for the euclidean minds. If you accept the
   * Infinite Norm distance, though, you can assume it's a
   * sphere. :-) 
   */

  class SubSpace {
  public:
    SubSpace(const Space *space, id_t center, scalar_t radius);
    void resize(scalar_t r);

    inline scalar_t getRadius() const { return __radius; };
    inline id_t getMinId() const { return __minId; }
    inline coord_t getMinCoord(int i) const { return __minSub[i]; }
    inline coord_t getMaxCoord(int i) const { return __maxSub[i]; }
    inline const Space *getSpace() const { return __space; }

    /*
     * Apply:
     *
     * Applies function "F(id_t) to every Id of the subspace.
     */

    template <class F>
    void apply(F &func)
    {
      __apply(getMinId(), getSpace()->getDimension() - 1, func);
    }

    template <class F>
    void __apply(id_t id, const unsigned dim, F &func)
    {
      for ( int i = getMinCoord(dim); i < getMaxCoord(dim); i++ ) {
	if ( dim == 0 )
	  func(id);
	else
	  __apply(id, dim - 1, func);
	id += getSpace()->getIdProjectorValue(dim);
      }
    }

    /* collect
     *
     * Given a set of Ids (through an iterator and a function
     * that returns Ids from the iterator value) it sets the current
     * subspace as the smallest subspace that contains all the points
     * collected until now yet not smaller than the current size. 
     */

    template <class T, class F>
    scalar_t collect(T begin, T end, F &func)
    {
      /* Recalculate Max and Min. */
      for ( T it = begin; it != end; it++ )
	for ( unsigned i = 0; i < __space->getDimension(); i++ ) {
	  coord_t coord = __space->getCoord(func(*it), i);
	  __minSub[i] = std::min(coord, __minSub[i]);
	  // +1 because we check for max which is over the end.
	  __maxSub[i] = std::max(coord + 1, __maxSub[i]);
	}

      /* Some points might have been cut off by the min/max
	 checks. Rescan to calculate. */
      __recalculateSize();

      return __radius;
    }

  private:
    void __recalculateSize();

    id_t __minId;
    Vector __minSub;
    Vector __maxSub;
    Vector __center;
    scalar_t __radius;
    const Space *__space;
  };

  /*
   * Normal Random Generator class.
   *
   * This class is a container for Pseudo -- look at rnd_normal.h and
   * laugh for yourself -- Normal Random vectors (in form of Ids) that
   * range in a certain radius from a center.
   */

  class NormalRandomGenerator {
  public:
    NormalRandomGenerator(const Space *space, id_t center, scalar_t radius)
      : __r(radius), __center(space->getDimension()), __space(space)
    { 
      __space->setVectorFromId(center, __center); 
    }
    id_t operator()() const;
  private:
    scalar_t __r;
    Vector __center;
    const Space *__space;
  };


  /*
   * Space Transformation class.
   *
   * This function creates a mapping (with some easily fixable limits)
   * between two different spaces, and provides functions to map
   * vectors (in form of Ids) and scalars into each other.
   */

  class SpaceTransform {
  public:
    SpaceTransform(const Space *x, const Space *y);

    id_t transformIdForward(id_t iid) const;
    id_t transformIdBackward(id_t oid) const;

    inline scalar_t transformScalarForward(scalar_t ival) const
    {
      return ival / __maxRatio;
    }
    scalar_t transformScalarBackward(scalar_t oval) const
    {
      return oval * __maxRatio;
    }

  private:
    const Space *__inputSpace;
    const Space *__outputSpace;
    Vector __inOutRatios;
    unsigned __maxRatio;
  };

}

#endif
