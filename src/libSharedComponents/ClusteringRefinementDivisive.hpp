/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTERINGREFINEMENTDIVISIVE_HPP_
#define _CLUSTERINGREFINEMENTDIVISIVE_HPP_

#include <Error.hpp>
using cepba_tools::Error;

#include <libClustering.hpp>
#include "TraceData.hpp"
#include "Partition.hpp"
#include "ClusteringStatistics.hpp"
#include "ClusterInformation.hpp"

#include <list>
using std::list;

#include <set>
using std::set;

#include <fstream>
using std::ofstream;

class ClusteringRefinementDivisive: public Error
{
  private:
    INT32  MinPoints;

    vector<double> EpsilonPerLevel;

    size_t Steps;
    size_t LastStep;

    string OutputFilePrefix;
    bool   PrintStepsInformation;

    libClustering*                       ClusteringCore;

    map<instance_t, size_t>              Instance2Burst;

    vector<ClusteringStatistics>         StatisticsHistory;
    vector<vector<ClusterInformation*> > NodesPerLevel;

  public:

    /*
    ClusteringRefinementDivisive(INT32      MinPoints,
                                 double     MaxEpsilon,
                                 double     MinEpsilon,
                                 size_t     Steps);
    */
    ClusteringRefinementDivisive(INT32          MinPoints,
                                 vector<double> EpsilonPerLevel);

    ~ClusteringRefinementDivisive(void);

    bool Run(const vector<CPUBurst*>& Bursts,
             vector<Partition>&       IntermediatePartitions,
             Partition&               LastPartition,
             bool                     PrintStepsInformation,
             string                   OutputFilePrefix = "");

  private:
    bool GenerateEpsilons(void);

    bool IsSplitOK(ClusterInformation* Parent);

    vector<CPUBurst*> GenerateBurstsSubset(const vector<CPUBurst*>& Bursts,
                                           ClusterInformation*      Node);

    bool RunFirstStep(const vector<CPUBurst*>& Bursts,
                      Partition&               FirstPartition);

    bool RunStep(size_t                   Step,
                 const vector<CPUBurst*>& Bursts,
                 Partition&               PreviousPartition,
                 Partition&               NewPartition,
                 bool&                    Stop);

    bool RunDBSCAN(const vector<const Point*>& CurrentData,
                   double                      Epsilon,
                   Partition&                  CurrentPartition);

    bool GenerateNodes(const vector<CPUBurst*>&     Bursts,
                       Partition&                   CurrentPartition,
                       vector<ClusterInformation*>& Nodes);

    void GeneratePartition(Partition& NewPartition);

    vector<pair<instance_t, cluster_id_t> > GetAssignment(ClusterInformation* Node,
                                                          size_t              Level);

    size_t ColapseNonDividedSubtrees(ClusterInformation* Node);

    void   ReclassifyNoise(const vector<CPUBurst*>& Bursts,
                           ClusterInformation*      Node,
                           size_t                   Level);

    bool IsIDInSet(cluster_id_t       ID,
                   set<cluster_id_t>& IDsSet);

    bool PrintPlots(const vector<CPUBurst*>& Bursts,
                    Partition&               CurrentPartition,
                    size_t                   Step);

    bool PrintTrees(size_t          Level,
                    bool            LastTree = false);

    bool PrintTreeNodes(ofstream& str, ClusterInformation* Node);
    bool PrintTreeLinks(ofstream& str, ClusterInformation* Node);

};

#endif // _CLUSTERINGREFINEMENTDIVISIVE_HPP_
