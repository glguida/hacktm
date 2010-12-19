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
  
  /* This parameter is the fraction of the input area covered by each
     column's proximal dendrite. A value of 1 means that the whole
     input space can be reached by any column, a value of zero is
     possible but safely ignored and put to a allowed minimum. */
  extern float overlappingCoverage;

  /* The number of synapses in each Proximal Dendrite. */
  extern unsigned proximalSynapses;

}

#endif __HACKTMCONFIG_H__
