#ifndef __HACKTM_SPACE_H__
#define __HACKTM_SPACE_H__

#include <cassert>
#include <vector>
#include <numeric>

namespace HackTM {

  typedef int coord_t;
  typedef unsigned id_t;
  typedef unsigned scalar_t;
  typedef std::vector<coord_t> Vector;

  class Space {
  public:
    Space(const Vector &max);

    inline bool contains(id_t id) const { return id < __size; }
    inline bool contains(Vector v) const
    {
      for (unsigned i = 0; i < __dimensions; i++ )
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
    
    inline const unsigned getSize() const { return __size; }
    inline const unsigned getDimensions() const { return __dimensions; }
    inline coord_t getMaxCoord(int i) const { return __maxCoordinates[i]; }
    inline coord_t getIdProjectorValue(int i) const  { return __idProjector[i]; }

  private:
    unsigned  __size;
    Vector __idProjector;
    Vector __maxCoordinates;
    const unsigned __dimensions;
  };

  class SubSpace {
  public:
    SubSpace(const Space *space, id_t center, scalar_t radius);
    void resize(scalar_t r);

    inline scalar_t getRadius() const { return __radius; };
    inline id_t getMinId() const { return __minId; }
    inline unsigned getMinCoord(int i) const { return __minSub[i]; }
    inline unsigned getMaxCoord(int i) const { return __maxSub[i]; }
    inline const Space *getSpace() const { return __space; }


    template <class F>
    void apply(F &func)
    {
      __apply(getMinId(), getSpace()->getDimensions() - 1, func);
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

    template <class T, class F>
    scalar_t collect(T begin, T end, F &func)
    {
      /* Recalculate Max and Min. */
      for ( T it = begin; it != end; it++ )
	for ( unsigned i = 0; i < __space->getDimensions(); i++ ) {
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

  class NormalRandomGenerator {
  public:
    NormalRandomGenerator(const Space *space, id_t center, scalar_t radius)
      : __r(radius), __space(space), __center(space->getDimensions())
    { 
      __center = Vector(space->getDimensions());
      __space->setVectorFromId(center, __center); 
    }
    id_t operator()() const;
  private:
    scalar_t __r;
    Vector __center;
    const Space *__space;
  };

  class SpaceTransform {
  public:
    SpaceTransform(const Space *x, const Space *y)
      : __inputSpace(x), __outputSpace(y), __inOutRatios(x->getDimensions())
    {
      assert ( x->getDimensions() == y->getDimensions() );
      assert ( x > y && "Time to generalize this class. It's easy." );
 
      for ( unsigned i = 0; i < x->getDimensions(); i++ )
	__inOutRatios[i] = x->getMaxCoord(i) / y->getMaxCoord(i);
      
      __maxRatio = *std::max_element(__inOutRatios.begin(), __inOutRatios.end());
    }

    //    Vector &transformVectorForward(const Vector &iv, Vector &ov);
    //    Vector &transformVectorBackward(const Vector &iv, Vector &ov);

    id_t transformIdForward(id_t iid) const {
      unsigned oid = 0;
      for ( unsigned i = 0; i < __inputSpace->getDimensions(); i++ ) {
	coord_t icoord = __inputSpace->getCoord(iid, i);
	coord_t ocoord = icoord / __inOutRatios[i];
	oid += ocoord * __outputSpace->getIdProjectorValue(i);
      }
      assert ( __outputSpace->contains(oid) );
      return oid;
    }

    id_t transformIdBackward(id_t oid) const
    {
      unsigned iid = 0;
      for ( unsigned i = 0; i < __inputSpace->getDimensions(); i++ ) {
	coord_t ocoord = __outputSpace->getCoord(oid, i);
	// Add 1/2 of the ratio to be at the center of the interval.
	coord_t icoord = ocoord * __inOutRatios[i] + (__inOutRatios[i] / 2);
	iid += icoord * __inputSpace->getIdProjectorValue(i);
      }
      assert (__inputSpace->contains(iid) );
      return iid;
    }

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
