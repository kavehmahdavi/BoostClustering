/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_CORE_H__
#define __TDBSCAN_CORE_H__

#include <MRNet_wrappers.h>
#include "libDistributedClustering.hpp"

/**
 * This is the base class both for the front-end and the back-ends.
 * It stores common configuration attributes and data structures
 * that have to be present in both sides of the MRNet.
 */
class TDBSCANCore
{
   public:
      TDBSCANCore();

   protected:
      STREAM                 *stClustering;
      STREAM                 *stXchangeDims;
      STREAM                 *stSupport;
      std::vector<HullModel*> GlobalModel;

      double Epsilon;
      int    MinPoints;
      int    TargetMinPoints;
      string ClusteringDefinitionXML;
      string InputTraceName;
      string OutputFileName;
      bool   Verbose;
      bool   ReconstructTrace;

      void Send_Configuration(void);
      void Recv_Configuration(void);
};

#endif /* __TDBSCAN_CORE_H__ */

