#ifndef __HACKTM_PROXIMAL_DENDRITES_H__
#define __HACKTM_PROXIMAL_DENDRITES_H__

#include <list>
#include "metric/MetricSpace.h"
#include "BitVector.h"

namespace HackTM {

  struct synapse {
    float perm;
    unsigned id;
  };

  class ProximalDendrite {
  public:
    ProximalDendrite(const metric::MetricSpace *inputspace)
      : __inputSpace(inputspace) {}
    ~ProximalDendrite();

    void populatePotentialSynapses(unsigned synapses, unsigned center, unsigned radius);
    void adjustPotentialSynapses(const BitVector &input);
    void clearPotentialSynapses();

    unsigned getOverlap(const BitVector &input);
    inline unsigned getReceptiveFieldSize() const { return __receptiveFieldSize; }

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
    unsigned __receptiveFieldSize;
    void __updateReceptiveFieldSize();

    const metric::MetricSpace *__inputSpace;

    std::list<struct synapse *> __connectedSynapses;
    std::list<struct synapse *> __potentialSynapses;
    void __addSynapse(unsigned id, float perm);
    void __incPotentialSynapse(struct synapse *syn);
    void __decPotentialSynapse(struct synapse *syn);
  };

}
  
#endif
