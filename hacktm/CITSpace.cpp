#include "HackTM.h"
#include "Space.h"
#include "CITSpace.h"

using namespace HackTM;

CITState::CITState(unsigned columns, unsigned cells, unsigned time)
  : __state(columns * cells * time), __cols(columns),
    __cells(cells), __time(time)
{
  Vector max(3);
  max.push_back(columns);
  max.push_back(cells);
  max.push_back(time);

  __space = new Space(max);
}

CITState::~CITState()
{
  delete __space;
}

