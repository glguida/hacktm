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
