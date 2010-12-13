#include <iostream>
#include <valarray>
#include "Vector.h"
#include "MetricSpace.h"


using namespace metric;

main()
{
  int zero[3] = { 0, 0, 0 };
  int maxx[3] = { 9, 0, 0};
  int maxy[3] = { 0, 9, 0};
  int maxz[3] = { 0, 0, 9};
  int a[3] = { 2, 3, 5 };
  Vector x(a,3);
  Vector y(10,3);
  bool z  = (x < y).min();
  //  std::valarray<bool> z = x > y;

  Vector VV(3);
  VV[0] = 5;
  VV[1] = 2;
  VV[2] = 2;

  std::cout << "HERE!!!" << (VV >= 0).min() << '\n';
  

  MetricSpace s1(&x);
  MetricSpace s2(&y);

  std::cout << s1.size() << std::endl;
  std::cout << s1.contains(y) << std::endl;
  std::cout << s1.contains(x) << std::endl;

  std::cout << s2.size() << std::endl;
  std::cout << s2.contains(x) << std::endl;
  std::cout << s2.contains(y) << std::endl;

  std::cout << std::endl;  std::cout << std::endl;  std::cout << std::endl;

  std::cout << s2.getIdFromVector(s2.getVectorFromId(0)) << std::endl;
  std::cout << s2.getIdFromVector(s2.getVectorFromId(10)) << std::endl;;
  std::cout << s2.getIdFromVector(s2.getVectorFromId(100)) << std::endl;;
  std::cout << s2.getIdFromVector(s2.getVectorFromId(999)) << std::endl;;

  for ( unsigned i = 0; i < s1.size(); i++ )
    assert ( s1.getIdFromVector(s1.getVectorFromId(i)) == i );
  for ( unsigned i = 0; i < s2.size(); i++ )
    assert ( s2.getIdFromVector(s2.getVectorFromId(i)) == i );
  std::cout << "Test Passed.\n";

  
  std::cout << s2.getNormalRandomId(5, 50) << std::endl;
}
