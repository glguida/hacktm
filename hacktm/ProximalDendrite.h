#ifndef __HACKTM_PROXIMAL_DENDRITES_H__
#define __HACKTM_PROXIMAL_DENDRITES_H__

#include <list>
#include "HackTM.h"
#include "Space.h"
#include "BitVector.h"

namespace hacktm {

  class ProximalDendrite {
  public:
    ProximalDendrite() {}
    ~ProximalDendrite();

    void populateSynapses(unsigned synapses, const Space *inputspace, id_t center);
    void adjustSynapses(const BitVector &input);
    unsigned getOverlap(const BitVector &input);

    inline scalar_t getReceptiveFieldSize() const { return __receptiveFieldSize; }

    friend class Introspection;
  private:
    scalar_t __receptiveFieldSize;
    void __updateReceptiveFieldSize();
    void __addSynapse(id_t id, float perm);
    void __incPotentialSynapse(struct synapse *syn);
    void __decPotentialSynapse(struct synapse *syn);

    const Space *__inputSpace;
    std::list<synapse *> __connectedSynapses;
    std::list<synapse *> __potentialSynapses;
  };

} // namespace
  
#endif
