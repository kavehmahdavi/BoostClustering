/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_ROOT_H__
#define __TDBSCAN_ROOT_H__

#include <vector>
#include <FrontProtocol.h>
#include "TDBSCANCore.h"
#include "Statistics.h"
#include "ClustersInfo.h"

using namespace Synapse;

/**
 * This class implements the front-end side of the TDBSCAN protocol,
 * which is the same both for the on-line and off-line back-ends.
 */
class TDBSCANRoot : public TDBSCANCore, public FrontProtocol
{
  private:
      string OutputGlobalClusteringFileName;
      libDistributedClustering *libClustering;

  protected:
      string OutputPrefix;

  public:
    TDBSCANRoot (double Eps,
                 int    MinPts,
                 string ClusteringDefinitionXML,
                 string InputTraceName,
                 string OutputFileName,
                 bool   Verbose,
                 bool   ReconstructTrace);

    TDBSCANRoot (string ClusteringDefinitionXML,
                 bool   Verbose);

    string ID()
    {
      return "TDBSCAN";
    }
    void Setup (void);
    int  Run  (void);

    void PrintGraphStats (Statistics &ClusteringStats);
};

#endif /* __TDBSCAN_ROOT_H__ */

