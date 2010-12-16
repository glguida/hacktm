#include <iostream>
#include "Space.h"

using namespace HackTM;

main()
{
  Vector v1(3, 1000);
  Vector v2(3);
  v2[0] = 20;
  v2[1] = 30;
  v2[2] = 50;
  //  v2[3] = 50;
  Space m1(v1);
  Space m2(v2);

  SpaceTransform f(&m1, &m2);

  for ( unsigned id2 = 0; id2 < m2.getSize(); id2++ ) {
    unsigned id1 = f.transformIdBackward(id2);
    Vector v(3);
    m1.setVectorFromId(id1, v);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << std::endl;
  }
}

