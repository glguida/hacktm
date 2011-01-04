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

#ifndef __HACKTM_TEMPORALPOOLER_H__
#define __HACKTM_TEMPORALPOOLER_H__

#include "HackTM.h"
#include "Space.h"
#include "HackTMTime.h"
#include "CISpace.h"
#include "Cell.h"
#include "CellsState.h"
#include "BitVector.h"

namespace hacktm {

  class TemporalPooler {
  public:
    TemporalPooler(Space *columnspace, unsigned cellspercol);
    ~TemporalPooler();

    void run(htmtime_t cur, htmtime_t prev, const std::list<id_t> &actColumns, BitVector &output);
    void calculateActiveState(htmtime_t cur, htmtime_t prev, const std::list<id_t> &actColumns);
    void calculatePredictiveState(htmtime_t cur, htmtime_t prev);
    void learn(htmtime_t cur, htmtime_t prev);
    void getOutput(htmtime_t cur, BitVector &output);
    
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
