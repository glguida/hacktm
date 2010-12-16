#include "BitVector.h"
#include "Region.h"
#include "HTMFunctions.h"

using namespace HackTM;

main()
{
  Vector input(2, 1000);
  Vector columns(2,30);
  Space inputSpace(input);
  Space columnSpace(columns);

  std::cout << "Initializing...";

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
