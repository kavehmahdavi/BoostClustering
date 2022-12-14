/* Boosted density-based Kernel to apply clustering analysis*/

#include "SystemMessages.hpp"
using cepba_tools::system_messages;

#include "MUSTER_XCLARA.hpp"
#include "Point.hpp"

#include <cstring>
#include <cmath>
#include <cassert>
#include <cerrno>

#include <cstdarg>

#include <algorithm>
using std::sort;

#include <iostream>
using std::cout;
using std::endl;

using std::make_pair;

using cluster::medoid_id;

const string MUSTER_XCLARA::NAME                = "MUSTER_XCLARA";
const string MUSTER_XCLARA::MAX_CLUSTERS_STRING = "max_clusters";


/*****************************************************************************
 * class MUSTER_XCLARA implementation                                        *
 ****************************************************************************/
MUSTER_XCLARA::MUSTER_XCLARA()
{
}

MUSTER_XCLARA::MUSTER_XCLARA(map<string, string> ClusteringParameters)
{
  map<string, string>::iterator ParametersIterator;

  /* maximum clusters */
  ParametersIterator = ClusteringParameters.find(MUSTER_XCLARA::MAX_CLUSTERS_STRING);
  if (ParametersIterator == ClusteringParameters.end())
  {
    string ErrorMessage;
    ErrorMessage = "parameter '" + MUSTER_XCLARA::MAX_CLUSTERS_STRING + "' not found in MUSTER_XCLARA definition";
    
    SetErrorMessage(ErrorMessage);
    SetError(true);
    return;
  }
  else
  {
    char* err;
    MaxClusters = strtol(ParametersIterator->second.c_str(), &err, 0);

    if (*err)
    {
      string ErrorMessage;
      ErrorMessage = "incorrect value for MUSTER_XCLARA parameter '"+ MUSTER_XCLARA::MAX_CLUSTERS_STRING + "'";

      SetErrorMessage(ErrorMessage);
      SetError(true);
      return;
    }
  }

  return;
}

bool MUSTER_XCLARA::Run(const vector<const Point*>& Data,
                     Partition&                     DataPartition,
                     bool                           SimpleRun)
{
  cluster::kmedoids    muster_algorithm = cluster::kmedoids();
  size_t               dimensions;
  vector<Point> ReferencesData;

  if (Data.empty())
  {
    SetError(true);
    SetErrorMessage("no data to apply clustering analysis");
    return false;
  }

  system_messages::information("Using MUSTER's XCLARA we can not provide algorithm progress information\n");

  /* Use the dimensions of the first point */
  dimensions = Data[0]->size();
  
  for (size_t i = 0; i < Data.size(); i++)
  {
    ReferencesData.push_back((*Data[i]));
  }

  muster_algorithm.xclara(ReferencesData, PointEuclideanDistance(), (size_t) MaxClusters, dimensions);

  /* Translate MUSTER partition to 'libClustering' partition */
  ProcessClusterAssignment (muster_algorithm, DataPartition, Data.size());

  return true;
}


string MUSTER_XCLARA::GetClusteringAlgorithmName(void) const
{
  ostringstream Result;
  Result << "MUSTER_XCLARA (MaxClusters=" << MaxClusters << ")";
  return Result.str();
}


string MUSTER_XCLARA::GetClusteringAlgorithmNameFile(void) const
{
  ostringstream Result;
  Result << "MUSTER_XCLARA_MaxClusters_" << MaxClusters;
  return Result.str();
}


bool MUSTER_XCLARA::ComputeParamsApproximation(const vector<const Point*>& Data,
INT32 ParametersCount, ...)
{
  return true;
}

void MUSTER_XCLARA::ProcessClusterAssignment(cluster::kmedoids &muster_algorithm,
                                             Partition         &DataPartition,
                                             size_t             DataSize)
{
  vector<cluster_id_t>& ClusterAssignmentVector = DataPartition.GetAssignmentVector();
  set<cluster_id_t>&    DifferentIDs            = DataPartition.GetIDs();

  /*
  map<medoid_id, cluster_id_t>           ClusterTranslation;
  map<medoid_id, cluster_id_t>::iterator ClusterTranslationQuery;
  cluster_id_t                           CurrentClusterId = MIN_CLUSTERID;
  */
  
  ClusterAssignmentVector.clear();
  DifferentIDs.clear();

  for (size_t i = 0; i < DataSize; i++)
  {
    ClusterAssignmentVector.push_back(muster_algorithm.cluster_ids[i]);
    DifferentIDs.insert(muster_algorithm.cluster_ids[i]);

  }

  /* Add one more cluster, to avoid the non-existent NOISE cluster */
  /* Not needed any more
  DataPartition.NumberOfClusters (DifferentIDs.size());
  DataPartition.HasNoise(false); */

  /* DEBUG 
  for (ClusterTranslationQuery  = ClusterTranslation.begin();
       ClusterTranslationQuery != ClusterTranslation.end();
       ++ClusterTranslationQuery)
  {
    cout << "medoid_id = " << ClusterTranslationQuery->first;
    cout << " cluster_id = " << ClusterTranslationQuery->second;
    cout << endl;
  }
  */
}

