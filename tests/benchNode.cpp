#include "BitVector.h"
#include "Space.h"
#include "Region.h"
#include "HTMFunctions.h"

using namespace HackTM;

main()
{
  Vector input(2, 1000);
  Vector columns(2, 30);
  Space inputSpace(input), columnSpace(columns);

  Region n(&inputSpace, &columnSpace);

  spatialPoolerInit(&n);

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

  unsigned id = 100;
  while ( id-- > 0 ) {
    switch ( id % 4) {
    case 0 : 
      spatialPooler(&n, diagonalL);
      break;
    case 1:
      spatialPooler(&n, diagonalR);
      break;
    case 2:
      spatialPooler(&n, ddiag);
      break;
    case 4:
      spatialPooler(&n, black);
    }
  }
}
