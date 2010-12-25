#ifndef __HACKTM_CELLSSTATE_H__
#define __HACKTM_CELLSSTATE_H__

#include "HackTM.h"
#include "HackTMTime.h"
#include "Space.h"
#include "BitVector.h"

#include <list>

namespace HackTM {
  class CellsState {
  public:
    CellsState(Space *cellspace);
    ~CellsState();

    inline bool activeState(id_t cellid, htmtime_t time) const
    {
      return __activeState[time]->test(cellid);
    }
    inline void setActiveState(id_t cellid, htmtime_t time) {
      __activeState[time]->set(cellid);
    }
    inline bool predictiveState(id_t cellid, htmtime_t time) const 
    {
      return __predictiveState[time]->test(cellid);
    }
    inline void setPredictiveState(id_t cellid, htmtime_t time) {
      __predictiveState[time]->set(cellid);
    }
    inline bool learnState(id_t cellid) const
    {
      return __learnState->test(cellid);
    }
    inline void setLearnState(id_t cellid, htmtime_t time) {
      __learnState->set(cellid);
      __learnCells[time].push_back(cellid);
    }

    inline void dumpActiveState(htmtime_t time) {
      std::cout << "Active State: " << std::endl;
      std::cout << *__activeState[time] << std::endl;
    }
    inline void dumpPredictiveState(htmtime_t time) {
      std::cout << "Predictive State: " << std::endl;
      std::cout << *__predictiveState[time] << std::endl;
    }
    inline void dumpLearnState() {
      std::cout << "Learn State: " << std::endl;
      std::cout << *__learnState << std::endl;
    }
    inline void _TMP_dumpOutput(htmtime_t time) {
      (*__learnState) = *__predictiveState[time];
      (*__learnState) |= *__activeState[time];
      std::cout << *__learnState << std::endl;
    }

    void getRandomLearnCells(htmtime_t time, unsigned num, std::list<id_t> &list);
    void resetState(htmtime_t time);
    
    friend class Introspection;
  private:
    BitVector *__predictiveState[HTMTIME_MAX];
    BitVector *__activeState[HTMTIME_MAX];
    BitVector *__learnState;
    std::vector<id_t> __learnCells[HTMTIME_MAX];
    Space     *__cellSpace;
  };
}
#endif
