#ifndef __HACKTM_COLUMN__
#define __HACKTM_COLUMN__

#include "ProximalDendrite.h"

namespace HackTM {

  class Column {
  public:
    Column(unsigned id)
      : __id(id) {}
    ~Column();

    inline unsigned getId() const { return __id; }

    void updateOverlap(const BitVector &input);
    inline unsigned getOverlap() const { return __overlap; }

    ProximalDendrite    *proximalDendrite;    

  private:
    unsigned             __id;
    unsigned             __overlap;
  };
}
#endif
