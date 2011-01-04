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
