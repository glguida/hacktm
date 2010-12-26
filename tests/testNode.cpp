#include "BitVector.h"
#include "HackTM.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"

using namespace hacktm;

main()
{
  Vector input(2, 1000);
  Vector columns(2,30);
  Space inputSpace(input);
  Space columnSpace(columns);

  htmtime_t cur = 0;
  htmtime_t prev = 1;

  std::cout << "Initializing...";

  SpatialPooler sp(&inputSpace, &columnSpace);
  TemporalPooler tp(&columnSpace, 4);

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

  std::list<hacktm::id_t> out;

  std::cout << "done." << std::endl;

  while ( 1 ) {
    cur = ( cur + 1 ) % 2;
    prev = (prev + 1) % 2;
    char c;
    std::cin >> c;
    switch ( c ) {
    case 'z' : 
      sp.run(diagonalL, out);
      tp.calculateActiveState(cur, prev, out);
      tp.calculatePredictiveState(cur, prev);
      tp.learn(cur, prev);
      break;
    case 'x':
      sp.run(diagonalR, out);
      tp.calculateActiveState(cur, prev, out);
      tp.calculatePredictiveState(cur, prev);
      tp.learn(cur, prev);
      break;
    case 'c':
      sp.run(ddiag, out);
      tp.calculateActiveState(cur, prev, out);
      tp.calculatePredictiveState(cur, prev);
      tp.learn(cur, prev);
      break;
    default:
      sp.run(black, out);
      tp.calculateActiveState(cur, prev, out);
      tp.calculatePredictiveState(cur, prev);
      tp.learn(cur, prev);
    }
  }
}
