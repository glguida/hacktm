#include <ctime>

#include "BitVector.h"
#include "HackTM.h"
#include "Space.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"

using namespace hacktm;

#define CELLSPERCOL 4
#define NUM 1000

main()
{
  clock_t c1, c2, c3;

  /* First of all, diable debug flags. */
  hacktmdebug::Flags = 0;

  c1 = clock();
  Vector input(2, 1000);
  Vector columns(2, 30);
  Space inputSpace(input), columnSpace(columns);

  SpatialPooler sp(&inputSpace, &columnSpace);
  TemporalPooler tp(&columnSpace, CELLSPERCOL);

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

  std::list<hacktm::id_t> actColumns;
  BitVector output(columnSpace.getSize() * CELLSPERCOL);

  c2 = clock();
  htmtime_t cur = 0;
  htmtime_t prev = 1;
  unsigned id = NUM;
  while ( id-- > 0 ) {
    cur = ( cur + 1 ) % 2;
    prev = (prev + 1) % 2;
    switch ( id % 4) {
    case 0 : 
      sp.run(diagonalL, actColumns);
      break;
    case 1:
      sp.run(diagonalR, actColumns);
      break;
    case 2:
      sp.run(ddiag, actColumns);
      break;
    case 4:
      sp.run(black, actColumns);
    }
    tp.run(cur, prev, actColumns, output);
  }
  c3 = clock();

#define SECONDS(_c)  ((_c)/CLOCKS_PER_SEC)
#define MSECS(_c)    (((_c) % CLOCKS_PER_SEC)/(CLOCKS_PER_SEC/1000))
  std::cerr << "Initialization took " << SECONDS(c2 - c1) << " and " << MSECS(c2 - c1) << " milliseconds." << std::endl;
  std::cerr << "Spatial pooling took " << SECONDS(c3 - c2) << " and " << MSECS(c3 - c2) << " milliseconds for " << NUM 
	    << " inputs. (avg of " << SECONDS((c3 - c2)/NUM) << "s and " << MSECS((c3-c2)/NUM) << " msecs per operation)." << std::endl;
}
