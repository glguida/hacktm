#include <vector>
#include <algorithm>
#include <cmath>
#include "HackTMConfig.h"
#include "ProximalDendrite.h"
#include "metric/rnd_normal.h"
#include "metric/Sphere.h"


using namespace metric;

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
    metric::Sphere s(__inputSpace, center, radius);
    metric::Sphere::random_iterator random_it = s.getNormalDistributionIterator();

    for ( unsigned i = 0; i < synapses; i++, random_it++ )
      __addSynapse(*random_it, rnd_normal(htmconfig::connectedPerm, 0.1));
    
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
    syn->perm = std::min(syn->perm + htmconfig::permanenceInc, 1.0f);

    if ( syn->perm >= htmconfig::connectedPerm )
      __connectedSynapses.push_front(syn);
  }
  
  void
  ProximalDendrite::__decPotentialSynapse(struct synapse *syn)
  {
    float prev_perm = syn->perm;
    syn->perm = std::max(0.0f, syn->perm - htmconfig::permanenceDec);
  }
}

namespace std {

  vector<unsigned> operator-=(vector<unsigned> &LHS, vector<unsigned> &RHS)
  {
    for ( unsigned i = 0; i < LHS.size(); i++ )
      LHS[i] -= RHS[i];
    return LHS;
  }

}

namespace HackTM {

  void
  ProximalDendrite::__updateReceptiveFieldSize()
  {
    Vector min, max;
    for ( synapse_iterator it = connectedBegin(); it != connectedEnd(); it++ ) {
      __inputSpace->accumulateMinCoordinates(min, (*it)->id);
      __inputSpace->accumulateMaxCoordinates(max, (*it)->id);
    }
    __receptiveFieldSize = (max - min).max();
  }

};
