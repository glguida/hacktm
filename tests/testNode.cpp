#include "vector2D.h"
#include "BitVector.h"
#include "Region.h"
#include "HTMFunctions.h"

using namespace metric;
using namespace HackTM;

main()
{
  Vector2D input(1000,1000);
  Vector2D columns(30,30);
  MetricSpace inputSpace(input), columnSpace(columns);

  std::cout << "Initializing...";

  Region n(&inputSpace, &columnSpace);

  spatialPoolerInit(&n);

  BitVector diagonalL(input.area());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalL.set(i * 1000 + i);
  }
  BitVector diagonalR(input.area());
  for ( unsigned i = 0; i < 1000; i++ ) {
    diagonalR.set(i * 1000 + (999 - i));
  }
  BitVector ddiag(input.area());
  for ( unsigned i = 0; i < 1000; i++ ) {
    ddiag.set(i * 1000 + (999 - i));
    ddiag.set(i * 1000 + i);
  }

  BitVector black(input.area());
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
