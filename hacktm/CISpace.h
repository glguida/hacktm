#ifndef __HACKTM_CITSTATE_H__
#define __HACKTM_CITSTATE_H__

#include "HackTM.h"
#include "Space.h"
#include "BitVector.h"

/*
 * Column-IndexOfCell-Time States.
 * 
 * The Column-Cell-Time triade forms a three dimensional space (that
 * we can handle with the Space class) with an associated (hopefully
 * sparse, TODO) bitmap. There will be three states:
 *
 * - activeState(c,i,t)
 * - predictive(c,i,t)
 * - learnState(c,i,t)
 *
 * each with its own Space class.  */

namespace HackTM {

  enum time {
    tm1 = 0,
    t  = 1,
  };

  class CITState {
    CITState( unsigned columns, unsigned cells, unsigned time );
    ~CITState();

    inline id_t getIdFromCIT(unsigned c, unsigned i, unsigned t)
    {
      return (id_t)(c * __space->getIdProjectorValue(0) + 
		    i * __space->getIdProjectorValue(1) +
		    t * __space->getIdProjectorValue(2) );
    }
    inline void setCITFromId(id_t id, unsigned &c, unsigned &i, unsigned &t)
    {
      c = __space->getCoord(id, 0);
      i = __space->getCoord(id, 1);
      t = __space->getCoord(id, 2);
    }
    inline bool getState(id_t id) { return __state.test(id); }
    inline bool getState(unsigned c, unsigned i, unsigned t) 
    { 
      return getState(getIdFromCIT(c, i, t));
    }
    inline void setState(id_t id, bool val) 
    { 
      val ? __state.set(id) : __state.reset(id);
    }
    inline bool setState(unsigned c, unsigned i, unsigned t, bool val)
    {
      setState(getIdFromCIT(c, i, t), val);
    }

  private:
    const unsigned cols;
    const unsigned cell;
    const unsigned time;
    BitVector __state;
    Space *__space;
  };
  

}

#endif
