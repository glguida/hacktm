#ifndef __HACKTM_PROXIMAL_DENDRITES_H__
#define __HACKTM_PROXIMAL_DENDRITES_H__

#include <list>
#include "HackTM.h"
#include "Space.h"
#include "Introspection.h"
#include "BitVector.h"

namespace HackTM {

  struct synapse {
    float perm;
    id_t id;
  };

  class ProximalDendrite {
  public:
    ProximalDendrite() {}
    ~ProximalDendrite();

    void populateSynapses(unsigned synapses, const Space *inputspace, id_t center);
    void adjustSynapses(const BitVector &input);
    unsigned getOverlap(const BitVector &input);

    inline scalar_t getReceptiveFieldSize() const { return __receptiveFieldSize; }

    typedef std::list<struct synapse *>::iterator synapse_iterator;
    friend class Introspection;
  private:
    scalar_t __receptiveFieldSize;
    void __updateReceptiveFieldSize();

    const Space *__inputSpace;

    std::list<struct synapse *> __connectedSynapses;
    std::list<struct synapse *> __potentialSynapses;
    void __addSynapse(id_t id, float perm);
    void __incPotentialSynapse(struct synapse *syn);
    void __decPotentialSynapse(struct synapse *syn);
  };

}
  
#endif
