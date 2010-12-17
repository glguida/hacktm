#ifndef __HACKTM_PROXIMAL_DENDRITES_H__
#define __HACKTM_PROXIMAL_DENDRITES_H__

#include <list>
#include "HackTM.h"
#include "Space.h"
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

    void populateSynapses(unsigned synapses, const Space *inputspace,
				   id_t center, scalar_t radius);
    void adjustSynapses(const BitVector &input);

    unsigned getOverlap(const BitVector &input);
    inline scalar_t getReceptiveFieldSize() const { return __receptiveFieldSize; }

    typedef std::list<struct synapse *>::iterator synapse_iterator;
    inline synapse_iterator connectedBegin () {
      return __connectedSynapses.begin();
    }
    inline synapse_iterator connectedEnd () {
      return __connectedSynapses.end();
    }
    inline synapse_iterator potentialBegin() {
      return __potentialSynapses.begin();
    }
    inline synapse_iterator potentialEnd() {
      return __potentialSynapses.end();
    }

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
