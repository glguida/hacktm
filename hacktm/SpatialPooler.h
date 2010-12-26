#ifndef __HACKTM_SPATIALPOOLER__
#define __HACKTM_SPATIALPOOLER__

#include <list>
#include <vector>
#include <iostream>

#include "HackTM.h"
#include "Space.h"
#include "ProximalDendrite.h"
#include "BitVector.h"

namespace HackTM {

  class SpatialPooler {
  public:
    SpatialPooler(const Space *inputspace, const Space *columnspace);
    ~SpatialPooler();

    void run(const BitVector &input, std::list<id_t> &actColumns);
    void updateColumnsOverlap(const BitVector&);
    void inhibitColumns(const BitVector&, std::list<id_t> &);

    friend class Introspection;
  private:
    void     __initDendrites();
    scalar_t __avgReceptiveFieldSize();
    id_t     __getColumnInputCenter(id_t id) const;
    scalar_t __scaleRadiusFromColumnSpace(scalar_t value) const;
    scalar_t __scaleRadiusToColumnSpace(scalar_t value) const;
    unsigned __kthScore(SubSpace &neighbors, unsigned);

    scalar_t              __inhibitionRadius;
    unsigned              *__columnsOverlap;
    ProximalDendrite      *__dendrites;    
    const Space           *__inputSpace;
    const Space           *__columnSpace;
    const SpaceTransform  *__inputToColumn;
  };

};

#endif
