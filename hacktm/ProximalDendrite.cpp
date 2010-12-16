#include <vector>
#include <algorithm>
#include <cmath>
#include "HackTMConfig.h"
#include "ProximalDendrite.h"
#include "rnd_normal.h"
#include "Space.h"


namespace HackTM {

  ProximalDendrite::~ProximalDendrite()
  {
    clearPotentialSynapses();
  }

  void 
  ProximalDendrite::clearPotentialSynapses()
  {
    __connectedSynapses.clear();
    for ( synapse_iterator it = potentialBegin(); it != potentialEnd(); it++ )
      delete *it;
    __potentialSynapses.clear();
  }

  void 
  ProximalDendrite::populatePotentialSynapses(unsigned synapses, unsigned center, unsigned radius)
  {
    NormalRandomGenerator nrg(__inputSpace, center, radius);
    for ( unsigned i = 0; i < synapses; i++ )
      __addSynapse(nrg(), rnd_normal(htmconfig::connectedPerm, 0.1));
    
    __updateReceptiveFieldSize();
  }

  static bool isNotConnected(struct synapse *syn) {
    return syn->perm < htmconfig::connectedPerm;
  }

  void
  ProximalDendrite::adjustPotentialSynapses(const BitVector &input)
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
  ProximalDendrite::__addSynapse(unsigned id, float perm)
  {
    struct synapse *syn = new struct synapse;
    syn->id = id;
    syn->perm = perm;
    __potentialSynapses.push_back(syn);
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
    id_t operator()(struct synapse *s) {
      return s->id;
    };
  } syn2id;

  void
  ProximalDendrite::__updateReceptiveFieldSize()
  {
    SubSpace sub(__inputSpace, 0, 0);
    __receptiveFieldSize = sub.collect(connectedBegin(), connectedEnd(), syn2id);
#if 0
    /* The Receptive Field radius is approximated with 1/2 of the
       average of the edges of the smallest subspace (defined by two
       vectors, min and max) containing all the connections of this
       dendrite to the input space. */
    for ( synapse_iterator it = connectedBegin(); it != connectedEnd(); it++ ) {
      __inputSpace->accumulateMinMaxCoordinates(min, max, (*it)->id);
    }
    __receptiveFieldSize = (max -= min).sum() / (2 *max.size());
#endif

  }

}
