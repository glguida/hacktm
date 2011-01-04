/*
 * Copyright (C) 2010 Gianluca Guida <glguida@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vector>
#include <algorithm>
#include <cmath>

#include "HackTM.h"
#include "ProximalDendrite.h"
#include "rnd_normal.h"

namespace hacktm {

  ProximalDendrite::~ProximalDendrite()
  {
    synapse_iterator it;
    for ( it = __potentialSynapses.begin(); it != __potentialSynapses.end(); it++ )
      delete *it;
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
    synapse_iterator it;
    for ( it = __potentialSynapses.begin(); it != __potentialSynapses.end(); it++ ) {
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
    synapse_iterator it;
    unsigned overlap = 0;

    for ( it = __connectedSynapses.begin(); it != __connectedSynapses.end(); it++ ) {
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
    __receptiveFieldSize = sub.collect(__connectedSynapses.begin(), 
				       __connectedSynapses.end(), syn2id);
  }

}
