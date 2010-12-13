#ifndef __HACKTM_REGION__
#define __HACKTM_REGION__

#include <list>
#include <vector>
#include "metric/MappingFunction.h"
#include "metric/MetricSpace.h"
#include "metric/Sphere.h"
#include "Column.h"

namespace HackTM {

  class Region {
  public:
    Region(const metric::MetricSpace *inputspace, const metric::MetricSpace *columnspace);
    ~Region();

    void updateColumnsOverlap(const BitVector &input);
    void inhibitColumns();
    void adjustProximalSynapses(const BitVector &inpu);

    inline void setInhibitionRadius(unsigned r) { __inhibitionRadius = r; };

    inline const MetricSpace *getInputSpace() const {
      return __inputSpace;
    }
    inline const MetricSpace *getColumnSpace() const {
      return __columnSpace;
    }
    inline unsigned getColumnInputCenter(Column *c) const {
      return __inputToColumn->getInputIdFromOutput(c->getId());
    }
    inline unsigned scaleRadiusFromColumnSpace(unsigned value) const { 
      return __inputToColumn->scaleOutputValueWithMaxRatio(value);
    }
    inline unsigned scaleRadiusToColumnSpace(unsigned value) const {
      return __inputToColumn->scaleInputValueWithMaxRatio(value);
    }

    typedef std::vector<Column *>::iterator column_iterator;
    std::vector<Column *>columns;
    typedef std::list<Column *>::iterator activecol_iterator;
    std::list<Column *>activeColumns;

  private:
    void __connectColumnsToInput();
    unsigned kthScore(metric::Sphere::iterator, metric::Sphere::iterator, unsigned);

    unsigned __inhibitionRadius;
    const metric::MetricSpace *__inputSpace;
    const metric::MetricSpace *__columnSpace;
    const metric::MappingFunction *__inputToColumn;
  };

};

#endif
