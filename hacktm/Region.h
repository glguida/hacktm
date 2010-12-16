#ifndef __HACKTM_REGION__
#define __HACKTM_REGION__

#include <list>
#include <vector>
#include "Space.h"
#include "Column.h"

namespace HackTM {

  class Region {
  public:
    Region(const Space *inputspace, const Space *columnspace);
    ~Region();

    void updateColumnsOverlap(const BitVector &input);
    void inhibitColumns();
    void adjustProximalSynapses(const BitVector &inpu);

    inline void setInhibitionRadius(unsigned r) { __inhibitionRadius = r; };

    inline const Space *getInputSpace() const {
      return __inputSpace;
    }
    inline const Space *getColumnSpace() const {
      return __columnSpace;
    }
    inline unsigned getColumnInputCenter(Column *c) const {
      return __inputToColumn->transformIdBackward(c->getId());
    }
    inline unsigned scaleRadiusFromColumnSpace(unsigned value) const { 
      return __inputToColumn->transformScalarBackward(value);
    }
    inline unsigned scaleRadiusToColumnSpace(unsigned value) const {
      return __inputToColumn->transformScalarForward(value);
    }

    typedef std::vector<Column *>::iterator column_iterator;
    std::vector<Column *>columns;
    typedef std::list<Column *>::iterator activecol_iterator;
    std::list<Column *>activeColumns;

  private:
    void __connectColumnsToInput();
    unsigned kthScore(SubSpace &neighbors, unsigned);

    unsigned __inhibitionRadius;
    const Space *__inputSpace;
    const Space *__columnSpace;
    const SpaceTransform *__inputToColumn;
  };

};

#endif
