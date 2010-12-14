#include "BitVector.h"
#include "metric/Vector.h"
#include "Region.h"
#include "HTMFunctions.h"

using namespace metric;
using namespace HackTM;

main()
{
  Vector input(1000,2);
  Vector columns(30,2);
  MetricSpace inputSpace(&input), columnSpace(&columns);

  std::cout << "Initializing...";

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

  std::cout << "done." << std::endl;

  while ( 1 ) {
    char c;
    std::cin >> c;
    switch ( c ) {
    case 'z' : 
      spatialPooler(&n, diagonalL);
      break;
    case 'x':
      spatialPooler(&n, diagonalR);
      break;
    case 'c':
      spatialPooler(&n, ddiag);
      break;
    default:
      spatialPooler(&n, black);
    }
  }
}
