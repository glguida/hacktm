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

#include "BitVector.h"
#include "xbmlib.h"

namespace xbmlib {

  void setBitVectorFromXbm(xbmImage &im, hacktm::BitVector &bv)
  {
    unsigned size = im.width * im.height;
    bv.resize(size);

    for ( unsigned i = 0; i < size; i++ ) {
      unsigned charidx = i / 8;
      unsigned bitidx = i % 8;
      unsigned char c = im.bits[charidx];
      char mask = (1 << bitidx);

      /* Skip to next entry if they're all zero. */
      if ( bitidx == 0 && c == 0 )
	  i += 8;

      if ( c & mask )
	bv.set(i);
    }
  }

}
