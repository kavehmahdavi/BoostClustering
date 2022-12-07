/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTEREDTRACEGENERATOR_H
#define _CLUSTEREDTRACEGENERATOR_H

#include "trace_clustering_types.h"
#include <Error.hpp>
using cepba_tools::Error;

#include <CPUBurst.hpp>

#include <set>
using std::set;

class ClusteredTraceGenerator: public Error
{
  protected:
    enum   ReconstructorType { PRVStates, PRVEvents, SemanticGuidedPRV, TRF };

    string InputTraceName;
    FILE*  InputTraceFile;
    string OutputTraceName;
    FILE*  OutputTraceFile;
    bool   DestroyClusteredFile;

  public:
    ClusteredTraceGenerator (string  InputTraceName,
                             string  OutputTraceName);

    ~ClusteredTraceGenerator (void);

    virtual ReconstructorType GetType(void) = 0;

    template <typename T>
    bool Run (T                     begin,
              T                     end,
              vector<cluster_id_t>& IDs,
              set<cluster_id_t>&    DifferentIDs,
              bool                  PrintOnlyEvents,
              bool                  DoNotPrintFilteredBursts);

    virtual bool Run (vector<CPUBurst*>&    Bursts,
                      vector<cluster_id_t>& IDs,
                      set<cluster_id_t>&    DifferentIDs,
                      bool                  PrintOnlyEvents,
                      bool                  DoNotPrintFilteredBursts) = 0;

    virtual bool SetEventsToDealWith (set<event_type_t>& EventsToDealWith,
                                      bool               ConsecutiveEvts) = 0;
};


#endif
