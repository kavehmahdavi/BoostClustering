/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_WORKER_OFFLINE_H__
#define __TDBSCAN_WORKER_OFFLINE_H__

#include "TDBSCANWorker.h"

/**
 * This class implements an specific back-end protocol that
 * extracts data from a Paraver trace.
 */
class TDBSCANWorkerOffline: public TDBSCANWorker
{
   public:
      bool InitLibrary();
      bool ExtractData();
      bool ProcessResults(Support &GlobalSupport);
};

#endif /* __TDBSCAN_WORKER_OFFLINE_H__ */


