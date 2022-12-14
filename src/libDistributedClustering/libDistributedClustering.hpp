/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _LIB_DISTRIBUTED_CLUSTERING_HPP_
#define _LIB_DISTRIBUTED_CLUSTERING_HPP_

#include <trace_clustering_types.h>
#include <Point.hpp>
#include "HullModel.hpp"
#include "ClusterStatistics.hpp"

#define SILENT   0
#define VERBOSE  1
#define VVERBOSE 2

#include <vector>
using std::vector;
#include <set>
using std::set;
#include <map>
using std::map;



class libDistributedClusteringImplementation;

class libDistributedClustering
{
  private:
    libDistributedClusteringImplementation* Implementation;

    bool   Error,        Warning;
    string ErrorMessage, WarningMessage;

    bool   Root;

    /*
    bool                  ClusteringExecuted;
    bool                  HWCGroupSet;
    vector<INT32>         CurrentHWCGroup;
    bool                  StoreClassificationPoints;
    */

  public:
    libDistributedClustering(int         verbose,
                             const char *msg_prefix);

    /* This methods are intended to be used in the MRNet BE nodes, to perform
     * a local cluster analysis */

    bool   InitClustering(string ClusteringDefinitionXML,
                          double Epsilon,
                          int    MinPoints,
                          bool   Root,
                          int    MyRank,
                          int    TotalRanks);

    bool   InitClustering(string ClusteringDefinitionXML,
                          bool   Root,
                          int    MyRank,
                          int    TotalRanks);

    double GetEpsilon(void);

    void   SetMinPoints(int MinPts);

    int    GetMinPoints(void);

    bool   ExtractData(string InputFileName, set<int>& TasksToRead);

    bool   ExtractData(string InputFileName);

    size_t GetNumberOfPoints(void);

    bool   ClusterAnalysis(vector<HullModel*>& ClusterModels);

    bool   ClassifyData(vector<HullModel*>& ClusterModels);

    bool   FlushClustersInformation(string OutputClustersInfoFileName);

    bool   ReconstructInputTrace(string OutputTraceName);


    /* This second interface is focused to implement the cluster of local noise
     * points */

    bool InitClustering(double Epsilon,
                        int    MinPoints);

    bool ClusterAnalysis(const vector<const Point*> &Points,
                         const vector<long long>    &Durations,
                         vector<HullModel*>         &ClusterModels);

    bool GetNoisePoints(vector<const Point*>& NoisePoints, vector<long long>& NoiseDurations);

    /* Methods to be used in the ON-LINE implementation of the algorithm. In
     * this case, the data comes from buffers present in the data extraction
     * library. For this reason, we have to expose the data manipulation
     * routines */

    bool NewBurst(task_id_t                         TaskId,
                  thread_id_t                       ThreadId,
                  line_t                            Line,
                  timestamp_t                       BeginTime,
                  timestamp_t                       EndTime,
                  duration_t                        BurstDuration,
                  map<event_type_t, event_value_t>& EventsData);

    void GetParameterRanges(vector<double>& MinValues,
                            vector<double>& MaxValues);

    void NormalizeData(vector<double>& MinValues,
                       vector<double>& MaxValues);

    bool GetClusterStatistics(vector<ClusterStatistics*>& Statistics);

    /* A method to retrieve all information to perform the cross-process
     * analysis */

    bool GetFullBurstsInformation(vector<Point*>       &Points,
/*
                                  vector<task_id_t>    &TaskIDs,
                                  vector<thread_id_t>  &ThreadIDs,
*/
                                  vector<timestamp_t>  &BeginTimes,
                                  vector<timestamp_t>  &EndTimes,
                                  vector<cluster_id_t> &ClusterIDs);

    bool GetClusterIDs(vector<cluster_id_t> &ClusterIDs);


    /* Methods to print the models and the scatter plots of data. To be used
     * in BE nodes */
    bool PrintModels(vector<HullModel*> &ClusterModels,
                     string              ModelsFileName,
                     string              ScriptsFileNamePrefix = "",
                     string              PlotTitle             = "");

    bool PrintPlotScripts(string DataFileName,
                          string ScriptsFileNamePrefix = "",
                          bool   LocalPartition = false);

    bool PrintGlobalPlotScripts(string       DataFileName,
                                string       ScriptsFileNamePrefix,
                                unsigned int ClustersCount,
                                bool         PrintingModels = false);

    /* Error/Warning retrieveng methods */
    string GetErrorMessage(void);

    string GetWarningMessage(void);

protected:

private:

};

#endif // _LIB_DISTRIBUTED_CLUSTERING_HPP_
