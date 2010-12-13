#include <iostream>
#include "BitVector.h"
#include "metric/Sphere.h"
#include "metric/DistanceFunctions.h"

using namespace metric;

main()
{
  Vector max(100, 2), center(2);
  center[0] = 50;
  center[1] = 50;
  MetricSpace space(&max, NormInf);
  Sphere s;
  Sphere x(&space, space.getIdFromVector(center), 20);

  s = x;

  std::cout << s.getCenter() << " " << s.getRadius() << " " << s.getDistance(49) << std::endl;

  Sphere::iterator it;
  BitVector bitmap(VectorArea(max));
  for ( it = s.begin(); it != s.end(); it++ )
    bitmap.set(*it);

  std::cout << bitmap << std::endl;
  bitmap.reset();

  Sphere::random_iterator rnd_it = s.getNormalDistributionIterator();
  for (unsigned i = 0; i < 20; i++ ) {
    bitmap.set(*(rnd_it++));
  }
    /*    unsigned random;
    do {
      random = s.getNormalRandomPoint();
    } while ( bitmap.test(random) == true );
    bitmap.set(random);
    }*/
  std::cout << bitmap << std::endl;
}

