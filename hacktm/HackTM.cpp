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




