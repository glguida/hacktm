#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__

#include "llvm/BitVector.h"
#include <iostream>

namespace std {
  inline ostream &operator<< (ostream &ostr, const llvm::BitVector&RHS) {
    for (unsigned i = 0; i < RHS.size(); i++)
      ostr << RHS[i] ? '1' : '0';
    return ostr;
  }
}

using namespace llvm;

#endif
