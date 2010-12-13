#ifndef __HACKTMCONFIG_H__
#define __HACKTMCONFIG_H__

namespace htmconfig {

  /* The minimum permanence value at which a synapse is considered "connected" */
  const float connectedPerm = 0.2;

  /* Amount pf permanence value synapse are incremented/decremented during learning. */
  const float permanenceInc = 0.05;
  const float permanenceDec = 0.05;

  /* Minimum number of inputs that must be active for the column to be
     considered in Inhibition phase. */
  const unsigned minOverlap = 3;


  /* A parameter controlling the number of columns that will be
     winners after the inhibition step. */
  const unsigned desiredLocalActivity = 10;

}


/*
 * Local modification to the algorithms (previously known as hacks or heresies)
 */

#define OPT_PROPRECFIELD 	1L
#define OPT_OVERLAPINHIB	2L

#ifdef NUMENTA_STRICT
#define OPTIONS 0
#else
#define OPTIONS (0x3)
#endif

/*
 * Debug and printout controls.
 */

namespace hacktmdebug {

  const bool Debug_PrintSynapses = true;
  const bool Debug_PrintOverlappingColumns = true;
  const bool Debug_PrintWinningColumns = true;

}

#endif __HACKTMCONFIG_H__
