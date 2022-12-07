/* Boosted density-based Kernel to apply clustering analysis*/
#include "clustering_types.h"

#include <SystemMessages.hpp>
using cepba_tools::system_messages;

#include <sstream>
using std::ostringstream;

#include <cassert>

#include "ConvexHullClassifier.hpp"

bool ConvexHullClassifier::Classify(vector<const Point*>& Data,
                                    Partition&            DataPartition)
{
  return Classify(Data.begin(), Data.end(), (size_t) Data.size(), DataPartition);
}


bool ConvexHullClassifier::Classify(const Point* QueryPoint, cluster_id_t& ID)
{
  ostringstream Message;
  ID = NOISE_CLUSTERID;

  double MinSqDistance = MAX_DOUBLE;

  if (QueryPoint == NULL)
  {
    SetErrorMessage("no point to classify!");
    SetError(true);
    return false;
  }

  /* To improve the search, first we just look using the inclusion */
  for (size_t i = 0; i < HullModels.size(); i++)
  {
    if (HullModels[i].IsInside(QueryPoint))
    {
      ID    = (cluster_id_t) i+1;
      return true;
    }
  }

  /* If point hasn't been classified, we try the proximity */
  for (size_t i = 0; i < HullModels.size(); i++)
  {
    double CurrentSqDistance;
    int    CurrentDensity;

    HullModels[i].GetDistanceAndDensity(QueryPoint, CurrentSqDistance, CurrentDensity);

    if ((CurrentSqDistance <= pow(Eps
                                  , 2.0)) &&
        (CurrentSqDistance < MinSqDistance) &&
        (CurrentDensity+(QueryPoint->GetNeighbourhoodSize())+1) >= MinPoints)
    {
      MinSqDistance = CurrentSqDistance;
      ID            = (cluster_id_t) i+1;
    }
  }

  /* DEBUG
  Message << " ID = " << ID << endl;
  system_messages::information(Message.str().c_str());
  */

  return true;
}
