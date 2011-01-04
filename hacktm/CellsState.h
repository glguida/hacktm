/*
 * Copyright (C) 2010 Gianluca Guida <glguida@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
