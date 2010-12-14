#include <iostream>
#include "metric/Distance.h"
#include "metric/MetricSpace.h"
#include "metric/MappingFunction.h"

using namespace metric;

main()
{
  EuclideanDist dist;
  Vector v1(1000,1);
  Vector v2(1);
  v2[0] = 20;
  //  v2[1] = 30;
  //  v2[2] = 50;
  MetricSpace m1(&v1, dist);
  MetricSpace m2(&v2, dist);

  MappingFunction f(&m1, &m2);

  for ( unsigned id2 = 0; id2 < m2.size(); id2++ ) {
    unsigned id1 = f.getInputIdFromOutput(id2);
    Vector v = m1.getVectorFromId(id1);
    std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
  }
}

