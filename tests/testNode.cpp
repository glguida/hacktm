#include "BitVector.h"
#include "HackTM.h"
#include "SpatialPooler.h"

using namespace HackTM;

main()
{
  Vector input(2, 1000);
  Vector columns(2,30);
  Space inputSpace(input);
  Space columnSpace(columns);

  std::cout << "Initializing...";

  SpatialPooler sp(&inputSpace, &columnSpace);

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

  BitVector out(columnSpace.getSize());

  std::cout << "done." << std::endl;

  while ( 1 ) {
    char c;
    std::cin >> c;
    switch ( c ) {
    case 'z' : 
      sp.run(diagonalL, out);
      break;
    case 'x':
      sp.run(diagonalR, out);
      break;
    case 'c':
      sp.run(ddiag, out);
      break;
    default:
      sp.run(black, out);
    }
  }
}
