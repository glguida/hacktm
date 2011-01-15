/* HackTM - C++ implementation of Numenta Cortical Learning Algorithm.
 * Copyright (c) 2010-2011 Gianluca Guida <glguida@gmail.com>
 *  
 * This software is released under the Numenta License for
 * Non-Commercial Use. You should have received a copy of the Numenta
 * License for Non-Commercial Use with this software (usually
 * contained in a file called LICENSE.TXT). If you don't, you can
 * download it from 
 * http://numenta.com/about-numenta/Numenta_Non-Commercial_License.txt.
 */

#ifndef __HACKTMCONFIG_H__
#define __HACKTMCONFIG_H__

namespace htmconfig {

  /* The minimum permanence value at which a synapse is considered
     "connected" */
  extern float connectedPerm;

  /* Amount of permanence value synapse are incremented/decremented
     during learning. */
  extern float permanenceInc;
  extern float permanenceDec;

  /* Minimum number of inputs that must be active for the column to be
     considered in Inhibition phase. */
  extern unsigned minOverlap;


  /* A parameter controlling the number of columns that will be
     winners after the inhibition step. */
  extern unsigned desiredLocalActivity;

  /*Activation threshold for a segment. If the number of active
    connected synapses in a segment is greater than
    activationThreshold, the segment is said to be active. */
  extern unsigned activationThreshold;

  /* Minimum segment activity for learning. */
  extern unsigned minThreshold;

  extern unsigned newSynapseCount;

  extern unsigned learningRadius;

  extern float initialPerm;

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

#endif /*__HACKTMCONFIG_H__*/
