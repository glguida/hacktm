/* Poor Man's Gaussians. */
#include <stdlib.h>
#define SUMS 4

int
rnd_normal(int center, unsigned range)
{
  int j;
  float rnd = 0; 
  for ( j = 0; j < SUMS; j++ )
    rnd += (float)((random() % (1+RAND_MAX)) - (RAND_MAX/2))/(RAND_MAX/2);

  return center + (int)((rnd/SUMS) * range);
}

float
rnd_normal(float center, float range)
{
  int j;
  float rnd = 0; 
  for ( j = 0; j < SUMS; j++ )
    rnd += (float)((random() % (1+RAND_MAX)) - (RAND_MAX/2))/(RAND_MAX/2);

  return center + ((rnd/SUMS) * range);
}
