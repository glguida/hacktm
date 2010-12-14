#include "BitVector.h"
#include "metric/Vector.h"
#include "metric/DistanceFunctions.h"
#include "Region.h"
#include "HTMFunctions.h"

using namespace metric;
using namespace HackTM;

main()
{
  Vector input(1000,2);
  Vector columns(30,2);
  MetricSpace inputSpace(&input), columnSpace(&columns);

  Region n(&inputSpace, &columnSpace);

  spatialPoolerInit(&n);

  BitVector diagonalL(inputSpace.size());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalL.set(i * 1000 + i);
  }
  BitVector diagonalR(inputSpace.size());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalR.set(i * 1000 + (999 - i));
  }
  BitVector ddiag(inputSpace.size());
  for ( unsigned i = 0; i < 1000; i++ ) {
    ddiag.set(i * 1000 + (999 - i));
    ddiag.set(i * 1000 + i);
  }

  BitVector black(inputSpace.size());
  black.set();

  unsigned id = 30;
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
