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

#include <cmath>
#include <cassert>
#include <iostream>

#include "HackTM.h"
#include "SpatialPooler.h"
#include "BitVector.h"

namespace hacktm {

  SpatialPooler::SpatialPooler(const Space *inputspace, 
			       const Space *columnspace)
    : __inputSpace(inputspace), __columnSpace(columnspace)
  {
    __inputToColumn = new SpaceTransform(inputspace, columnspace);
    __dendrites = new ProximalDendrite[__columnSpace->getSize()];
    __columnsOverlap = new unsigned[ __columnSpace->getSize() ];
    __initDendrites();

    __inhibitionRadius = __scaleRadiusToColumnSpace(__avgReceptiveFieldSize());
  }

  SpatialPooler::~SpatialPooler()
  {
    delete __inputToColumn;
    delete [] __dendrites;
    delete [] __columnsOverlap;
  }

  void
  SpatialPooler::run(const BitVector &input, std::list<id_t> &actColumns)
  {
    /* Phase 1: Overlap. */
    updateColumnsOverlap(input);
    
    /* Phase 2: Inhibition + Synapse Learning */
    inhibitColumns(input, actColumns);

    /* Boost Missing */
    __inhibitionRadius = __scaleRadiusToColumnSpace(__avgReceptiveFieldSize());

    if ( hacktmdebug::Flags & hacktmdebug::PrintOverlappingColumns ) {
      for ( unsigned i = 0; i < __columnSpace->getSize(); i++ )
	std::cout << __columnsOverlap[i] << " ";
      std::cout << std::endl;
    }

    if ( hacktmdebug::Flags & hacktmdebug::PrintWinningColumns ) {
      std::cout << "And the winners are:\n";
      std::list<id_t>::iterator it;
      for ( it = actColumns.begin(); it != actColumns.end(); it++ )
	std::cout << *it << std::endl;
    }

    if ( hacktmdebug::Flags & hacktmdebug::PrintInhibitionRadius )
      std::cout << "Inhibition radius: " << __inhibitionRadius << std::endl;
  }

  void
  SpatialPooler::updateColumnsOverlap(const BitVector &input)
  {
    for ( unsigned i = 0; i < __columnSpace->getSize(); i++ )
      __columnsOverlap[i] = __dendrites[i].getOverlap(input);
  }

  void
  SpatialPooler::inhibitColumns(const BitVector &input, std::list<id_t> &output)
  {
    output.clear();
    for ( unsigned i = 0; i < __columnSpace->getSize(); i++ ) {
      unsigned minLocalActivity;
      SubSpace neighbors(__columnSpace, i, __inhibitionRadius);
      
      minLocalActivity = __kthScore(neighbors, htmconfig::desiredLocalActivity);

      if ( __columnsOverlap[i] > 0 && __columnsOverlap[i] >= minLocalActivity ) {
	/* Set "column" Active:
	 * - update the synapses;
	 * - set Id in the output Bitmap
	 */
	__dendrites[i].adjustSynapses(input);
	output.push_front(i);
      }
    }
  }

  scalar_t
  SpatialPooler::__avgReceptiveFieldSize()
  {
    uint64_t avg = 0; // No overflow check.
    for ( unsigned i = 0; i < __columnSpace->getSize(); i++ )
      avg += __dendrites[i].getReceptiveFieldSize();
    return avg / __columnSpace->getSize();
  }

  /*
   * KthScore functionality for SubSpaces.
   */
  /* TODO: Closures are just too ugly in C++. Make an iterator in SubSpace. */

  /* Sorry about this, I just miss lisp and closures. */
  class findScore
  {
  public:
    findScore(const unsigned *array) : __array(array) {}
    void operator()(id_t id)
    {
      unsigned newscore = __array[id];
      std::vector<unsigned>::iterator it;


      /* Early exit if the k-sized vector is already full, and we're
	 lower than the current kth element. */
      if ( (scores.size() > __k) && (newscore <= scores[__k]) )
	return;

      it = lower_bound(scores.begin(), scores.end(), 
		       newscore, std::greater<unsigned>());

      if ( it - scores.begin() < (ptrdiff_t)__k )
	scores.insert(it, newscore);

      return;
    }

    void reset(unsigned k)
    {
      __k = k;
      scores.clear();
    }

    std::vector<unsigned> scores;
  private:
    unsigned __k;
    const unsigned *__array;
  };

  unsigned
  SpatialPooler::__kthScore(SubSpace &neighbors, unsigned k)
  {
    findScore overlapScores(__columnsOverlap);
    overlapScores.reset(k);
    neighbors.apply(overlapScores);

    if ( overlapScores.scores.size() >= k )
      return overlapScores.scores[k - 1];
    else
      return 0;
  }
  
  id_t 
  SpatialPooler::__getColumnInputCenter(id_t id) const {
      return __inputToColumn->transformIdBackward(id);
    }
  scalar_t 
  SpatialPooler::__scaleRadiusFromColumnSpace(scalar_t value) const { 
    return __inputToColumn->transformScalarBackward(value);
  }
  scalar_t 
  SpatialPooler::__scaleRadiusToColumnSpace(scalar_t value) const {
    return __inputToColumn->transformScalarForward(value);
  }

  void 
  SpatialPooler::__initDendrites()
  {
    for ( unsigned i = 0; i < __columnSpace->getSize(); i++ ) {
      id_t center = __getColumnInputCenter(i);
      unsigned syns = htmconfig::proximalSynapses;

      __dendrites[i].populateSynapses(syns, __inputSpace, center);
    }
  }
}

