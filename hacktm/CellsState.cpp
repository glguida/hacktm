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

#include "HackTM.h"
#include "Space.h"
#include "CellsState.h"

#include <algorithm>

namespace hacktm {

CellsState::CellsState(Space *cellspace)
  : __cellSpace(cellspace)
{
  for ( unsigned i = 0; i < HTMTIME_MAX; i++ ) {
    __activeState[i] = new BitVector(cellspace->getSize());
    __predictiveState[i] = new BitVector(cellspace->getSize());
  }
  __learnState = new BitVector(cellspace->getSize());
}

CellsState::~CellsState()
{
  delete __learnState;
  for ( unsigned i = 0; i < HTMTIME_MAX; i++ ) {
    delete __activeState[i];
    delete __predictiveState[i];
  }
}

/* DIFF: learningRadius unused! */
void
CellsState::getRandomLearnCells(htmtime_t time, unsigned num, std::list<id_t> &list)
{
  std::random_shuffle(__learnCells[time].begin(),__learnCells[time].end());

  if ( __learnCells[time].size() > num )
    list.insert(list.begin(), __learnCells[time].begin(), __learnCells[time].begin() + num);
  else
    list.insert(list.begin(), __learnCells[time].begin(), __learnCells[time].end());
}

void
CellsState::resetState(htmtime_t time)
{
  __learnState->reset();
  __activeState[time]->reset();
  __predictiveState[time]->reset();
  __learnCells[time].clear();
}

}
