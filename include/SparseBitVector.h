#ifndef __SPARSEBITVECTOR_H__
#define __SPARSEBITVECTOR_H__

#include "llvm/ADT/SparseBitVector.h"
#include <iostream>

using namespace llvm;


namespace std {

  template <unsigned ES>
  ostream &operator<< (ostream &out, const SparseBitVector<ES>&RHS) {
    out << "[ ";

    typename SparseBitVector<ES>::iterator bi;
    for (bi = RHS.begin(); bi != RHS.end(); ++bi) {
      out << *bi << " ";
    }

    out << " ]\n";
    return out;
  }
}

using namespace llvm;


#endif
