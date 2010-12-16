#include <algorithm>
#include <numeric>
#include <functional>
#include <cassert>

#include "HackTM.h"
#include "Space.h"
#include "rnd_normal.h"

using namespace HackTM;

/*
 * Space
 */

Space::Space(const Vector &max)
  : __maxCoordinates(max), __dimensions(max.size())
{
  __size = std::accumulate(max.begin(), max.end(), 1, std::multiplies<coord_t>());

  // get a vector [ 1 MaxX MaxY MaxZ ... ]
  Vector tmp;
  tmp.push_back(1);
  tmp.insert(tmp.end(), max.begin(), max.end());
  tmp.pop_back();

  // Finally set idProjector as: [ 1 MaxX MaxY*MaxX MaxZ*MaxY*MaxX ... ]
  __idProjector = Vector(__dimensions);
  std::partial_sum(tmp.begin(), tmp.end(), __idProjector.begin(),
		   std::multiplies<coord_t>());
}

Vector &
Space::setVectorFromId(id_t id, Vector &v) const
{
  assert ( contains(id) );
  assert ( v.size() == __dimensions );
  for ( unsigned i = 0; i < __dimensions; i++ ) {
    v[i] =  (id / __idProjector[i]) % __maxCoordinates[i];
  }
  return v;
}

/*
 * SubSpace.
 */

SubSpace::SubSpace(const Space *space, id_t center, scalar_t radius)
  : __space(space)
{
  __maxSub = Vector(__space->getDimensions(), 0);
  __minSub = Vector(__space->getDimensions(), 0);
  __center = Vector(__space->getDimensions(), 0);
  __space->setVectorFromId(center, __center);

  resize(radius);
}

void
SubSpace::resize(scalar_t radius)
{
  for ( unsigned i = 0; i < __center.size(); i++ ) {
    __minSub[i] = std::max(__center[i] - (int)radius, 0);
    // +1 because we check for max which is over the end.
    __maxSub[i] = std::min(__center[i] + (int)radius + 1,
			   __space->getMaxCoord(i));
  }

  /* Some points might have been cut off by the min/max
     checks. Rescan to calculate. */
  __recalculateSize();
}

void
SubSpace::__recalculateSize()
{
  /* Recalculate minId */
  __minId = __space->getIdFromVector(__minSub);

  /* Recalculate radius with actual points inserted. */
  scalar_t radius = 0;
  for ( unsigned i = 0; i < __space->getDimensions(); i++ )
    radius += __maxSub[i] - __minSub[i];
  radius /= 2 * __space->getDimensions();
  __radius = radius;
}

/*
 * NormalRandomGenerator
 */

id_t 
NormalRandomGenerator::operator()() const
{
  id_t id = 0;
  for ( unsigned i = 0; i < __space->getDimensions(); i++ ) {
    coord_t x;
    do { 
      x = rnd_normal(__center[i], __r);
    } while ( x < 0 || x >= __space->getMaxCoord(i) );
    id += x * __space->getIdProjectorValue(i);
  }
  assert ( __space->contains(id) );
  return id;
}

/*
 * SpaceTransform
 */

SpaceTransform::SpaceTransform(const Space *x, const Space *y)
  : __inputSpace(x), __outputSpace(y), __inOutRatios(x->getDimensions())
{
  assert ( x->getDimensions() == y->getDimensions() );
  assert ( x > y && "Time to generalize this class. It's easy." );
  
  for ( unsigned i = 0; i < x->getDimensions(); i++ )
    __inOutRatios[i] = x->getMaxCoord(i) / y->getMaxCoord(i);
  
  __maxRatio = *std::max_element(__inOutRatios.begin(), __inOutRatios.end());
}

id_t
SpaceTransform::transformIdForward(id_t iid) const {
  id_t oid = 0;
  for ( unsigned i = 0; i < __inputSpace->getDimensions(); i++ ) {
    coord_t icoord = __inputSpace->getCoord(iid, i);
    coord_t ocoord = icoord / __inOutRatios[i];
    oid += ocoord * __outputSpace->getIdProjectorValue(i);
  }
  assert ( __outputSpace->contains(oid) );
  return oid;
}

id_t 
SpaceTransform::transformIdBackward(id_t oid) const
{
  id_t iid = 0;
  for ( unsigned i = 0; i < __inputSpace->getDimensions(); i++ ) {
    coord_t ocoord = __outputSpace->getCoord(oid, i);
    // Add 1/2 of the ratio to be at the center of the interval.
    coord_t icoord = ocoord * __inOutRatios[i] + (__inOutRatios[i] / 2);
    iid += icoord * __inputSpace->getIdProjectorValue(i);
  }
  assert (__inputSpace->contains(iid) );
  return iid;
}
