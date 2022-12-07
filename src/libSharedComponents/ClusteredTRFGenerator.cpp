/* Boosted density-based Kernel to apply clustering analysis*/

#include <SystemMessages.hpp>

#include "ClusteredTRFGenerator.hpp"
#include <EventEncoding.h>

#include <cstring>
#include <cerrno>

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <algorithm>
using std::sort;

#include <sstream>
using std::ostringstream;


ClusteredTRFGenerator::ClusteredTRFGenerator(string       InputTraceName,
                                             string       OutputTraceName,
                                             bool         PrintClusterBlocks)
:ClusteredTraceGenerator(InputTraceName, OutputTraceName)
{
  if (GetError())
    return;

  this->PrintClusterBlocks = PrintClusterBlocks;
}


ClusteredTRFGenerator::~ClusteredTRFGenerator(void)
{
}

bool ClusteredTRFGenerator::SetEventsToDealWith (set<event_type_t>& EventsToDealWith,
                                                 bool               ConsecutiveEvts)
{

  SetError(true);
  SetErrorMessage("TRF traces doesn't permit parsing based on events");

  return false;
}

bool ClusteredTRFGenerator::Run(vector<CPUBurst*>&    Bursts,
                                vector<cluster_id_t>& IDs,
                                set<cluster_id_t>&    DifferentIDs,
                                bool                  PrintOnlyEvents,
                                bool                  DoNotPrintFilteredBursts)
{
  return Run(Bursts.begin(),
             Bursts.end(),
             IDs,
             DifferentIDs,
             PrintOnlyEvents,
             DoNotPrintFilteredBursts);
}

/* PrintClusteredBurst *******************************************************/
bool
ClusteredTRFGenerator::CheckFirstBlockDefinition(char              *Buffer,
                                                 set<cluster_id_t>& DifferentIDs)
{
  char AuxBuffer[256];
  bool Result = false;
  set<cluster_id_t>::iterator DifferentIDsIterator;
  vector<cluster_id_t>        ClusterIDs;

  cluster_id_t MaxIDUsed;
  PrepareClusterIDsVector(ClusterIDs, DifferentIDs, MaxIDUsed);

  if (sscanf(Buffer,
             "\"block definition\" {  %[^\n]\n",
             AuxBuffer) == 1)
  { /* Print Cluster ID event description records */
    Result = true;

    fprintf(OutputTraceFile, "\"user event type definition\" { 90000001, ");
    fprintf(OutputTraceFile, "\"Cluster ID\", 9 };;\n");
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile, " 0, \"End\" };;\n");

    /* Cluster for missing data bursts
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile,
            " %d, \"Missing Data\"};;\n",
            MISSING_DATA_CLUSTERID+PARAVER_OFFSET);


     /* Cluster for duration filtered bursts
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile,
            " %d, \"Dur. Filtered\"};;\n",
            DURATION_FILTERED_CLUSTERID+PARAVER_OFFSET);

     /* Cluster for range filtered bursts
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile,
            " %d, \"Range Filtered\"};;\n",
            RANGE_FILTERED_CLUSTERID+PARAVER_OFFSET);


    /* Cluster for duration filtered bursts
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile,
            " %d, \"Th. Filtered\"};;\n",
            THRESHOLD_FILTERED_CLUSTERID+PARAVER_OFFSET);

    /* Cluster for noise bursts
    fprintf(OutputTraceFile, "\"user event value definition\" { 90000001, ");
    fprintf(OutputTraceFile,
            " %d, \"Noise\"};;\n",
            NOISE_CLUSTERID+PARAVER_OFFSET);


    /*
    for (size_t i = 1; i < NumberOfClusters; i++)
    {

      if (ClusterInformationVector[i]->GetClusterId() +
          DataPoint::CLUSTERS_OFFSET ==  DataPoint::DURATION_FILTERED)
      {
        break;
      }


      fprintf(OutputTraceFile,
              "\"user event value definition\" { 90000001, ");
      fprintf(OutputTraceFile,
              "%2d, \"Cluster %d\"};;\n",
              i+PARAVER_OFFSET,
              i);
    }
    */

    for (size_t i = 0; i < ClusterIDs.size(); i++)
    {
      fprintf(OutputTraceFile,
              "\"user event value definition\" { 90000001, ");
      fprintf(OutputTraceFile,
              "%2d, \"%s\"};;\n",
              ClusterIDs[i]+PARAVER_OFFSET,
              GetClusterName(ClusterIDs[i]).c_str());
    }

    fprintf(OutputTraceFile, "\n");
    fflush(OutputTraceFile);

    if (PrintClusterBlocks)
    {
      for (size_t i = 0; i < ClusterIDs.size(); i++)
      {
        fprintf(OutputTraceFile,
                "\"block definition\" { %d, \"%s\", \"Cluster\", 0, 0 };;\n",
                ClusterIDs[i]+PARAVER_OFFSET,
                GetClusterName(ClusterIDs[i]).c_str());
      }
    }
  }

  return Result;
}

/* PrintClusteredBurst *******************************************************/
bool
ClusteredTRFGenerator::PrintClusteredBurst(FILE        *Trace,
                                           long         TaskId,
                                           long         ThreadId,
                                           double       BurstDuration,
                                           cluster_id_t ClusterId,
                                           bool         ReplaceDurations)
{

  if (fprintf(Trace,
              "\"user event\" { %ld, %ld, 90000001, %ld };;\n",
              TaskId,
              ThreadId,
              (long) ClusterId) < 0)
  {
    SetError(true);
    SetErrorMessage("error writing opening cluster event on output trace");
    return false;
  }

  if (PrintClusterBlocks)
  {
    if (fprintf(Trace,
                "\"block begin\" { %ld, %ld, %ld };;\n",
                TaskId,
                ThreadId,
                (long) ClusterId) < 0)
    {
      SetError(true);
      SetErrorMessage("error writing cluster block begin on output trace");
      return false;
    }
  }

  if (ReplaceDurations)
  {
    if (fprintf(Trace,
                "\"CPU burst\" { %ld, %ld, %ld };;\n",
                TaskId,
                ThreadId,
                (long) ClusterId) < 0)
    {
      SetError(true);
      SetErrorMessage("error writing CPU burst on output trace");
      return false;
    }
  }
  else
  {
    if (fprintf(Trace,
                "\"CPU burst\" { %ld, %ld, %.06f };;\n",
                TaskId,
                ThreadId,
                BurstDuration) < 0)
    {
      SetError(true);
      SetErrorMessage("error writing CPU burst on output trace");
      return false;
    }
  }

  if (PrintClusterBlocks)
  {
    if (fprintf(Trace,
                "\"block end\" { %ld, %ld, %ld };;\n",
                TaskId,
                ThreadId,
                (long) ClusterId) < 0)
    {
      SetError(true);
      SetErrorMessage("error writing cluster block end on output trace");
      return false;
    }
  }

  if (fprintf(Trace,
              "\"user event\" { %ld, %ld, 90000001, 0 };;\n",
              TaskId,
              ThreadId) < 0)
  {
    SetError(true);
    SetErrorMessage("error writing closing cluster event on output trace");
    return false;
  }

  return true;
}

void ClusteredTRFGenerator::PrepareClusterIDsVector(vector<cluster_id_t>& ClusterIDs,
                                                    set<cluster_id_t>&    DifferentIDs,
                                                    cluster_id_t&         MaxIDUsed)
{
  set<cluster_id_t>::iterator DifferentIDsIterator;

  MaxIDUsed = NOISE_CLUSTERID;

  if (DifferentIDs.count(DURATION_FILTERED_CLUSTERID) == 0)
  {
    ClusterIDs.push_back(DURATION_FILTERED_CLUSTERID);
  }

  if (DifferentIDs.count(RANGE_FILTERED_CLUSTERID) == 0)
  {
    ClusterIDs.push_back(RANGE_FILTERED_CLUSTERID);
  }

  if (DifferentIDs.count(THRESHOLD_FILTERED_CLUSTERID) == 0)
  {
    ClusterIDs.push_back(THRESHOLD_FILTERED_CLUSTERID);
  }

  if (DifferentIDs.count(NOISE_CLUSTERID) == 0)
  {
    ClusterIDs.push_back(NOISE_CLUSTERID);
  }

  for (DifferentIDsIterator  = DifferentIDs.begin();
       DifferentIDsIterator != DifferentIDs.end();
       ++DifferentIDsIterator)
  {
    ClusterIDs.push_back((*DifferentIDsIterator));
    if ((*DifferentIDsIterator) > MaxIDUsed)
    {
      MaxIDUsed = (*DifferentIDsIterator);
    }
  }
  sort(ClusterIDs.begin(), ClusterIDs.end());

  return;
}

/**
 * Returns the cluster name using the ID
 *
 * \param ID ID value to generate the cluster name
 *
 * \return The name of the cluster, taking into account the special cluster ids
 */
string ClusteredTRFGenerator::GetClusterName(cluster_id_t ID)
{
  ostringstream ClusterName;

  switch (ID)
  {
    case UNCLASSIFIED:
      ClusterName << "Unclassified";
      break;
    case DURATION_FILTERED_CLUSTERID:
      ClusterName << "Duration Filtered";
      break;
    case RANGE_FILTERED_CLUSTERID:
      ClusterName << "Range Filtered";
      break;
    case THRESHOLD_FILTERED_CLUSTERID:
      ClusterName << "Threshold Filtered";
      break;
    case NOISE_CLUSTERID:
      ClusterName << "Noise";
      break;
    default:
      ClusterName << "Cluster " << ID;
      break;
  }

  return ClusterName.str();
}

