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
