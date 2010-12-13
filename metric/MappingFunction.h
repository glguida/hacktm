#ifndef __METRIC_MAPPINGFUNCTION_H__
#define __METRIC_MAPPINGFUNCTION_H__

#include "Vector.h"
#include "MetricSpace.h"

namespace metric {

  class MappingFunction
  {
  public:
    MappingFunction(const MetricSpace *x, const MetricSpace *y)
      : __inputSpace(x), __outputSpace(y), __inOutRatios(x->dimensions())
    {
      assert ( x->dimensions() == y->dimensions() );
      assert ( x > y && "Time to generalize this class. It's easy." );
      __inOutRatios = *x / *y;
      __maxRatio = __inOutRatios.max();
    }

    inline Vector getOutputVectorFromInput(const Vector &iv) const {
      return iv / __inOutRatios;
    }

    inline unsigned getOutputIdFromInput(unsigned iid) const {
      Vector iv = __inputSpace->getVectorFromId(iid);
      Vector ov = getOutputVectorFromInput(iv);
      return __outputSpace->getIdFromVector(iv);
    }

    inline Vector getInputVectorFromOutput(const Vector &ov) const {
      // (ov + 1/2) * In/Out Ratio, to center the resulting vector
      return (ov * __inOutRatios) + (__inOutRatios / 2);
    }

    inline unsigned getInputIdFromOutput(unsigned oid) const {
      Vector ov = __outputSpace->getVectorFromId(oid);
      Vector iv = getInputVectorFromOutput(ov);
      return __inputSpace->getIdFromVector(iv);
    }

    inline unsigned scaleInputValueWithMaxRatio(unsigned n) const {
      return n / __maxRatio;
    }
    inline unsigned scaleOutputValueWithMaxRatio(unsigned n) const {
      return n * __maxRatio;
    }

  private:
    const MetricSpace *__inputSpace;
    const MetricSpace *__outputSpace;
    Vector __inOutRatios;
    unsigned __maxRatio;
  };

};

#endif
