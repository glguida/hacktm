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

  /*Activation threshold for a segment. If the number of active
    connected synapses in a segment is greater than
    activationThreshold, the segment is said to be active. */
  unsigned activationThreshold = 8;

  /* Minimum segment activity for learning. */
  unsigned minThreshold = 5;

  unsigned newSynapseCount = 20;

  float initialPerm = 0.15;

  /* 
   *HackTM Specific Configuration. 
   */
  
  /* This parameter is the fraction of the input area covered by each
     column's proximal dendrite. A value of 1 means that the whole
     input space can be reached by any column, a value of zero is
     possible but safely ignored and put to a allowed minimum. */
  float overlappingCoverage = 0.5;
  
  /* The number of synapses in each Proximal Dendrite. */
  unsigned proximalSynapses = 1000;


}




