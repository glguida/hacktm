#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__

#include "llvm/ADT/BitVector.h"

#include <iostream>

namespace std {
  inline ostream &operator<< (ostream &ostr, const llvm::BitVector&RHS) {
    for ( unsigned i = 0; i < RHS.size(); i++ )
      ostr << RHS.test(i);

    ostr << "\n";
    return ostr;
  }
}

namespace hacktm {

  typedef llvm::BitVector BitVector;

}

#endif
