#include <vector>
#include <algorithm>
#include <cmath>

#include "HackTM.h"
#include "ProximalDendrite.h"
#include "rnd_normal.h"

namespace HackTM {

  ProximalDendrite::~ProximalDendrite()
  {
    __connectedSynapses.clear();
    for ( synapse_iterator it = potentialBegin(); it != potentialEnd(); it++ )
      delete *it;
    __potentialSynapses.clear();
  }

  void 
  ProximalDendrite::populateSynapses(unsigned synapses, const Space *inputspace, id_t center)
  {
    __inputSpace = inputspace;

    scalar_t radius = htmconfig::overlappingCoverage * __inputSpace->getMaxSide();
    NormalRandomGenerator nrg(__inputSpace, center, radius);
    BitVector synapseMap(__inputSpace->getSize());
    for ( unsigned i = 0; i < synapses; i++ ) {
      id_t random;

      /* Ensure that we get unique IDs from the NRG. */
      do {
	random = nrg();
      } while ( synapseMap.test(random) == true );
      synapseMap.set(random);

      /* Bias the random perm value towards the center of the dendrite:
       *
       * PERM, the "center" of the random distribution, is calculated
       * as: 
       *      (1 - d/r) * (3/2 * connectedPerm)
       *
       * Which is, is at 3/2 of connectedPerm at the center and goes
       * to zero linearly as the points are at radius distance.
       */
      float perm = 1.0 - (float)__inputSpace->getDistance(random, center)/radius;
      perm = perm * htmconfig::connectedPerm * 1.5;

      __addSynapse(random, std::max(0.0f, rnd_normal(perm, htmconfig::connectedPerm/2)));
    }
    __updateReceptiveFieldSize();
  }

  static bool isNotConnected(struct synapse *syn) {
    return syn->perm < htmconfig::connectedPerm;
  }

  void
  ProximalDendrite::adjustSynapses(const BitVector &input)
  {
    for ( synapse_iterator it = potentialBegin(); it != potentialEnd(); it++ ) {
      if ( input.test((*it)->id) )
	__incPotentialSynapse(*it);
      else
	__decPotentialSynapse(*it);
    }
    __connectedSynapses.remove_if(isNotConnected);

    __updateReceptiveFieldSize();
  }

  unsigned
  ProximalDendrite::getOverlap(const BitVector &input)
  {
    unsigned overlap = 0;

    for ( synapse_iterator it = connectedBegin(); it != connectedEnd(); it++ ) {
      if ( input.test((*it)->id) == true )
	overlap++;
    }
    
    if ( overlap < htmconfig::minOverlap )
      return 0;

    /*
     * FIXME: Missing boost
     */

    return overlap;
  }

  void 
  ProximalDendrite::__addSynapse(id_t id, float perm)
  {
    struct synapse *syn = new struct synapse;
    syn->id = id;
    syn->perm = perm;
    __potentialSynapses.push_front(syn);
    if ( syn->perm >= htmconfig::connectedPerm )
      __connectedSynapses.push_front(syn);
  }

  void
  ProximalDendrite::__incPotentialSynapse(struct synapse *syn)
  {
    float prev_perm = syn->perm;
    syn->perm = std::min(syn->perm + htmconfig::permanenceInc, 1.0f);

    if ( prev_perm <= htmconfig::connectedPerm 
	 && syn->perm >= htmconfig::connectedPerm )
      __connectedSynapses.push_front(syn);
  }
  
  void
  ProximalDendrite::__decPotentialSynapse(struct synapse *syn)
  {
    syn->perm = std::max(0.0f, syn->perm - htmconfig::permanenceDec);
  }
  
  static struct synapseToId {
    inline id_t operator()(struct synapse *s) {
      return s->id;
    };
  } syn2id;

  void
  ProximalDendrite::__updateReceptiveFieldSize()
  {
    SubSpace sub(__inputSpace, 0, 0);
    __receptiveFieldSize = sub.collect(connectedBegin(), connectedEnd(), syn2id);
  }

}
