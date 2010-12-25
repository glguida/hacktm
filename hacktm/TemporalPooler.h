#ifndef __HACKTM_TEMPORALPOOLER_H__
#define __HACKTM_TEMPORALPOOLER_H__

#include "HackTM.h"
#include "Space.h"
#include "HackTMTime.h"
#include "CISpace.h"
#include "Cell.h"
#include "CellsState.h"
#include "BitVector.h"

namespace HackTM {

  class TemporalPooler {
  public:
    TemporalPooler(Space *columnspace, unsigned cellspercol);
    ~TemporalPooler();

    void run(const std::list<id_t> &actColumns, BitVector &output);
    void calculateActiveState(htmtime_t cur, htmtime_t prev, std::list<id_t> &actColumns);
    void calculatePredictiveState(htmtime_t cur, htmtime_t prev);
    void learn(htmtime_t cur, htmtime_t prev);
    
    friend class Introspection;
  private:
    inline id_t __getIdFromCI(id_t c, unsigned i)
    {
      return __ciSpace->getIdFromCI(c, i);
    }
    inline bool __predictiveState(id_t c, unsigned i, htmtime_t t)
    {
      return __cellsState->predictiveState(__getIdFromCI(c, i), t);
    }
    inline bool __activeState(id_t c, unsigned i, htmtime_t t)
    {
      return __cellsState->activeState(__getIdFromCI(c, i), t);
    }
    inline void __setActiveState(id_t c, unsigned i, htmtime_t t)
    {
      __cellsState->setActiveState(__getIdFromCI(c, i), t);
    }
    inline bool __learnState(id_t c, unsigned i)
    {
      return __cellsState->learnState(__getIdFromCI(c, i));
    }
    inline void __setLearnState(id_t c, unsigned i, htmtime_t t)
    {
      __cellsState->setLearnState(__getIdFromCI(c, i), t);
    }
    inline Cell *__getCell(id_t c, unsigned i)
    {
      return &__cells[__getIdFromCI(c, i)];
    }

    Cell *__getBestMatchingCell(id_t c, htmtime_t t, DendriteSegment *&segment);


    CISpace    *__ciSpace;
    Cell       *__cells;
    CellsState *__cellsState;
    unsigned    __cellsPerColumn;
  };

}

#endif
