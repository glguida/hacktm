/* HackTM - C++ implementation of Numenta Cortical Learning Algorithm.
 * Copyright (c) 2010-2011 Gianluca Guida <glguida@gmail.com>
 *  
 * This software is released under the Numenta License for
 * Non-Commercial Use. You should have received a copy of the Numenta
 * License for Non-Commercial Use with this software (usually
 * contained in a file called LICENSE.TXT). If you don't, you can
 * download it from 
 * http://numenta.com/about-numenta/Numenta_Non-Commercial_License.txt.
 */

#ifndef __HACKTM_CELLSSTATE_H__
#define __HACKTM_CELLSSTATE_H__

#include "HackTM.h"
#include "HackTMTime.h"
#include "Space.h"
#include "BitVector.h"

#include <list>

namespace hacktm {

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
    inline void copyActiveState(htmtime_t time, BitVector &cp) const
    {
      cp = *__activeState[time];
    }
    inline void orPredictiveState(htmtime_t time, BitVector &base) const
    {
      base |= *__predictiveState[time];
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

} // namespace
#endif
