#include "HackTM.h"
#include "Introspection.h"
#include "ProximalDendrite.h"
#include "SpatialPooler.h"

using namespace HackTM;

/*
 * Print out potential synapses of a column at the center of the input
 * space in GNU plot data format.
 *
 * Usage: [s]plot "datafile" index <COLUMN> 
 *
 * See the GNUplot script 'showDendrites.gp'.
 */

main()
{
  Vector input(2, 1000);
  Vector col(2);
  col[0] = 20;
  col[1] = 20;
 
  Space inputSpace(input), columnSpace(col);
  SpatialPooler sp(&inputSpace, &columnSpace);
  Introspection info(&sp);

   info.dumpAllPotentialSynapses();
 }

