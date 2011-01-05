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

#include <ctime>

#include "BitVector.h"
#include "HackTM.h"
#include "Space.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"

using namespace hacktm;

#define CELLSPERCOL 4
#define NUM 1000

int
main()
{
  clock_t c1, c2, c3;

  /* First of all, disable debug flags. */
  hacktmdebug::Flags = 0;

  c1 = clock();
  Vector input(2, 1000);
  Vector columns(2, 30);
  Space inputSpace(input), columnSpace(columns);

  SpatialPooler sp(&inputSpace, &columnSpace);
  TemporalPooler tp(&columnSpace, CELLSPERCOL);

  BitVector diagonalL(inputSpace.getSize());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalL.set(i * 1000 + i);
  }
  BitVector diagonalR(inputSpace.getSize());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalR.set(i * 1000 + (999 - i));
  }
  BitVector ddiag(inputSpace.getSize());
  for ( unsigned i = 0; i < 1000; i++ ) {
    ddiag.set(i * 1000 + (999 - i));
    ddiag.set(i * 1000 + i);
  }

  BitVector black(inputSpace.getSize());
  black.set();

  std::list<hacktm::id_t> actColumns;
  BitVector output(columnSpace.getSize() * CELLSPERCOL);

  c2 = clock();
  htmtime_t cur = 0;
  htmtime_t prev = 1;
  unsigned id = NUM;
  while ( id-- > 0 ) {
    cur = ( cur + 1 ) % 2;
    prev = (prev + 1) % 2;
    switch ( id % 4) {
    case 0 : 
      sp.run(diagonalL, actColumns);
      break;
    case 1:
      sp.run(diagonalR, actColumns);
      break;
    case 2:
      sp.run(ddiag, actColumns);
      break;
    case 4:
      sp.run(black, actColumns);
    }
    tp.run(cur, prev, actColumns, output);
  }
  c3 = clock();

#define SECONDS(_c)  ((_c)/CLOCKS_PER_SEC)
#define MSECS(_c)    (((_c) % CLOCKS_PER_SEC)/(CLOCKS_PER_SEC/1000))
  std::cerr << "Initialization took " << SECONDS(c2 - c1) << " and " << MSECS(c2 - c1) << " milliseconds." << std::endl;
  std::cerr << "HackTM took " << SECONDS(c3 - c2) << " and " << MSECS(c3 - c2) << " milliseconds for " << NUM 
	    << " inputs. (avg of " << SECONDS((c3 - c2)/NUM) << "s and " << MSECS((c3-c2)/NUM) << " msecs per operation)." << std::endl;
}
