/*
 * Copyright (C) 2010 Gianluca Guida <glguida@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
