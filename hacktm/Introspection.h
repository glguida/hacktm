#ifndef __HACKTM_INTROSPECTION_H__
#define __HACKTM_INTROSPECTION_H__

/*
 * This class is used to gather statistics and display private
 * information of the system.
 */

namespace HackTM {
  class SpatialPooler;
  class ProximalDendrite;

  class Introspection {
  public:
    Introspection(SpatialPooler *sp);

    static void dumpSynapse(ProximalDendrite *, struct synapse *);
    void dumpPotentialSynapses(unsigned column);
    void dumpAllPotentialSynapses();


    SpatialPooler *spatialPooler;
    ProximalDendrite *dendrites;
  };
}

#endif
