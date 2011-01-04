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

#include "HackTM.h"
#include "DendriteSegment.h"

typedef std::list<struct synapse *>::iterator synapse_iterator;

namespace hacktm {

  void
  DendriteSegment::computeState(const htmtime_t t)
  {
    unsigned learn = 0;
    unsigned active = 0;
    __state[t] = inactiveState;

    synapse_iterator it;
    for ( it = __potentialSynapses.begin(); 
	  it != __potentialSynapses.end(); it++ ) {
      struct synapse *syn = *it;
      if ( syn->perm < htmconfig::connectedPerm ) {
	continue;
      }

      if ( __cellstate->activeState(syn->id, t) )
	active++;
      if ( __cellstate->learnState(syn->id) )
	learn++;
    }

    if ( learn > htmconfig::activationThreshold ) {
      __state[t] = learnState;
      __activity[t] = learn;
    } else if ( active > htmconfig::activationThreshold ) {
      __state[t] = activeState;
      __activity[t] = active;
    } else {
      __state[t] = inactiveState;
      __activity[t] = 0;
    }
  }

  unsigned
  DendriteSegment::getMatchingSynapses(const htmtime_t t) const
  {
    unsigned active = 0;
    std::list<struct synapse *>::const_iterator it;
    for ( it = __potentialSynapses.begin();
	  it != __potentialSynapses.end(); it++ ) {
      struct synapse *syn = *it;

      if ( __cellstate->activeState(syn->id, t) 
	   && syn->perm >= htmconfig::initialPerm )
	active++;
    }

    if ( active > htmconfig::minThreshold )
      return active;

    return 0;
  }

  void 
  DendriteSegment::getActiveSynapses(const htmtime_t t, std::list<id_t> &activeSynapses) const
  {
    std::list<struct synapse *>::const_iterator it;
    for ( it = __potentialSynapses.begin();
	  it != __potentialSynapses.end(); it++ ) {
      struct synapse *syn = *it;

      /* push_back here is important: it ensures that activeSynapses
	 are ordered by ID, given the way they're added! */
      if ( __cellstate->activeState(syn->id, t) && syn->perm > 0.0f )
	activeSynapses.push_back(syn->id);
    }
  }

  static void __decrementSynapse(struct synapse *s)
  {
    s->perm = std::max(0.0f, s->perm - htmconfig::permanenceDec);
  }

  static void __incrementSynapse(struct synapse *s)
  {
    s->perm = std::min(1.0f, s->perm + htmconfig::permanenceInc);
  }

  static bool __syn_id_comp(struct synapse *syn, id_t id)
  {
    return syn->id < id;
  }

  void
  DendriteSegment::addSynapses(std::list<id_t> &newSynapses)
  {
    std::list<id_t>::iterator newSynIt;
    synapse_iterator it;
    for ( newSynIt = newSynapses.begin(); newSynIt != newSynapses.end(); newSynIt++ ) {

      it = std::lower_bound(__potentialSynapses.begin(), __potentialSynapses.end(), *newSynIt, __syn_id_comp);

      /* getRandomLearnCells does not guaratee us that synapse aren't there already. */
      if ( it != __potentialSynapses.end() 
	   && (*it)->id == *newSynIt ) {
	__incrementSynapse(*it);
	continue;
      }

      /* Insert new synapse here. */
      synapse *newsyn = new synapse;
      newsyn->id = *newSynIt;
      newsyn->perm = htmconfig::initialPerm;
      __potentialSynapses.insert(it, newsyn);
    }
  }

  void
  DendriteSegment::synapseReinforcement(std::list<id_t> &activeSynapses, bool sequence, bool positive)
  {
    std::list<id_t>::iterator actSynIterator = activeSynapses.begin();
    synapse_iterator synapseIterator;

    if ( positive && sequence )
      __sequence = true;

    for ( synapseIterator = __potentialSynapses.begin();
	  synapseIterator != __potentialSynapses.end(); synapseIterator++) {
      struct synapse *syn = *synapseIterator;
	
      if ( positive && syn->id < *actSynIterator ) {
	__decrementSynapse(syn);
	continue;
      }

      if ( syn->id == *actSynIterator ) {
	if ( positive )
	  __incrementSynapse(syn);
	else
	  __decrementSynapse(syn);
	++actSynIterator;
	if ( actSynIterator == activeSynapses.end() )
	  break;
      }
    }
    /* activeSynapses scanned */

    if ( !positive )
      return;

    for (; synapseIterator != __potentialSynapses.end(); synapseIterator++) {
      struct synapse *syn = *synapseIterator;
      __decrementSynapse(syn);
    }
  }

} // namespace
