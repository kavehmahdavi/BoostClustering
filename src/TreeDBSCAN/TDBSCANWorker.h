/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_WORKER_H__
#define __TDBSCAN_WORKER_H__

#include <vector>
#include <libDistributedClustering.hpp>
#include <BackProtocol.h>
#include "TDBSCANCore.h"
#include "Support.h"

using namespace Synapse;

/**
 * This class implements the back-end side of the TDBSCAN protocol.
 */
class TDBSCANWorker : public TDBSCANCore, public BackProtocol
{
   public:
      TDBSCANWorker();

      string ID() { return "TDBSCAN"; }
      void   Setup(void);
      int    Run  (void);

      virtual bool InitLibrary(void) = 0;
      virtual bool ExtractData(void) = 0;
      virtual bool ProcessResults(Support &GlobalSupport) { return true; };
   private:
      bool ExchangeDimensions(void);

   protected:
      libDistributedClustering *libClustering;
      vector<HullModel*> LocalModel;

      /* Names of the output scripts and plots */
      string GlobalModelDataFileName;
      string GlobalModelPlotFileNamePrefix;

      string LocalModelDataFileName;
      string LocalModelPlotFileNamePrefix;

      string OutputLocalClusteringFileName;
      string OutputGlobalClusteringFileName;

      string FinalClusteringFileName;
      string FinalClustersInformationFileName;

      vector<double> GlobalMin;
      vector<double> GlobalMax;

   private:
      void CheckOutputFile();
      bool GenerateScripts();
};

#endif /* __TDBSCAN_WORKER_H__ */
