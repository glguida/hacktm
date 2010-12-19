#include "HackTM.h"

using namespace htmconfig;

namespace hacktmdebug {
  int Flags = All;
}

namespace htmconfig {

  /* The minimum permanence value at which a synapse is considered "connected" */
  float connectedPerm = 0.2;

  /* Amount pf permanence value synapse are incremented/decremented during learning. */
  float permanenceInc = 0.05;
  float permanenceDec = 0.05;

  /* Minimum number of inputs that must be active for the column to be
     considered in Inhibition phase. */
  unsigned minOverlap = 3;


  /* A parameter controlling the number of columns that will be
     winners after the inhibition step. */
  unsigned desiredLocalActivity = 10;

  /* 
   *HackTM Specific Configuration. 
   */
  
  /* This parameter controls the area where covered by the proximal
     dendrite. It is the radius of a sphere in the column space. The
     potential syapses of the column will overlap the input space of
     the columns included in the sphere. */
  float overlappingCoverage = 0.3;
  
  /* The number of synapses in each Proximal Dendrite. */
  unsigned proximalSynapses = 1000;
}







