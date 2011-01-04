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

#ifndef __HACKTM_SPATIALPOOLER__
#define __HACKTM_SPATIALPOOLER__

#include <list>
#include <vector>
#include <iostream>

#include "HackTM.h"
#include "Space.h"
#include "ProximalDendrite.h"
#include "BitVector.h"

namespace hacktm {

  class SpatialPooler {
  public:
    SpatialPooler(const Space *inputspace, const Space *columnspace);
    ~SpatialPooler();

    void run(const BitVector &input, std::list<id_t> &actColumns);
    void updateColumnsOverlap(const BitVector&);
    void inhibitColumns(const BitVector&, std::list<id_t> &);

    friend class Introspection;
  private:
    void     __initDendrites();
    scalar_t __avgReceptiveFieldSize();
    id_t     __getColumnInputCenter(id_t id) const;
    scalar_t __scaleRadiusFromColumnSpace(scalar_t value) const;
    scalar_t __scaleRadiusToColumnSpace(scalar_t value) const;
    unsigned __kthScore(SubSpace &neighbors, unsigned);

    scalar_t              __inhibitionRadius;
    unsigned              *__columnsOverlap;
    ProximalDendrite      *__dendrites;    
    const Space           *__inputSpace;
    const Space           *__columnSpace;
    const SpaceTransform  *__inputToColumn;
  };

};

#endif
