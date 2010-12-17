#ifndef __HACKTMCONFIG_H__
#define __HACKTMCONFIG_H__

namespace htmconfig {

  /* The minimum permanence value at which a synapse is considered "connected" */
  extern float connectedPerm;

  /* Amount pf permanence value synapse are incremented/decremented during learning. */
  extern float permanenceInc;
  extern float permanenceDec;

  /* Minimum number of inputs that must be active for the column to be
     considered in Inhibition phase. */
  extern unsigned minOverlap;


  /* A parameter controlling the number of columns that will be
     winners after the inhibition step. */
  extern unsigned desiredLocalActivity;

  /* 
   *HackTM Specific Configuration. 
   */
  
  /* This parameter controls the area where covered by the proximal
     dendrite. It is the radius of a sphere in the column space. The
     potential syapses of the column will overlap the input space of
     the columns included in the sphere. */
  extern unsigned radialOverlapping;

  /* The number of synapses in each Proximal Dendrite. */
  extern unsigned proximalSynapses;

}

#endif __HACKTMCONFIG_H__
