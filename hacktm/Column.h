#ifndef __HACKTM_COLUMN__
#define __HACKTM_COLUMN__

#include "HackTM.h"
#include "ProximalDendrite.h"

namespace HackTM {

  class Column {
  public:
    Column(id_t id)
      : __id(id) {}
    ~Column();

    inline id_t getId() const { return __id; }

    void updateOverlap(const BitVector &input);
    inline unsigned getOverlap() const { return __overlap; }

    ProximalDendrite *proximalDendrite;    

  private:
    id_t __id;
    unsigned __overlap;
  };
}
#endif
