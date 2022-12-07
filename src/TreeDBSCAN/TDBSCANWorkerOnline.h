/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_WORKER_ONLINE_H__
#define __TDBSCAN_WORKER_ONLINE_H__

#include "TDBSCANWorker.h"

/**
 * This class implements an specific back-end protocol that
 * extracts data from an on-line tracing system.
 */
class TDBSCANWorkerOnline: public TDBSCANWorker
{
   public:
      typedef void (*FetchCallback)(libDistributedClustering *libClustering);
      typedef void (*FeedCallback) (vector<timestamp_t> &BeginTimes, vector<timestamp_t> &EndTimes, vector<cluster_id_t> &ClusterIDs, vector<int> &BurstsSupport);

      TDBSCANWorkerOnline(FetchCallback DataExtractCallback, FeedCallback DataFeedCallback);

      bool InitLibrary();
      bool ExtractData();
      bool ProcessResults(Support &GlobalSupport);

   private:
      FetchCallback DataExtractCallback;
      FeedCallback  DataFeedCallback;
};

#endif /* __TDBSCAN_WORKER_ONLINE_H__ */
