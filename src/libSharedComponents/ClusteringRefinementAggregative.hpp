/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTERINGREFINEMENTAGGREGATIVE_HPP_
#define _CLUSTERINGREFINEMENTAGGREGATIVE_HPP_

#include <Error.hpp>
using cepba_tools::Error;

#include <libClustering.hpp>
#include "TraceData.hpp"
#include "Partition.hpp"
#include "ClusteringStatistics.hpp"
#include "ClusterInformation.hpp"
#include "SequenceScore.hpp"

#include <list>
using std::list;

#include <set>
using std::set;

#include <fstream>
using std::ofstream;

class ClusteringRefinementAggregative: public Error
{
  private:
    INT32          MinPoints;
    vector<double> EpsilonPerLevel;

    size_t         Steps;
    size_t         LastStep;

    bool           PrintStepsInformation;
    string         OutputFilePrefix;

    libClustering*                         ClusteringCore;

    map<instance_t, size_t>                Instance2Burst;
    map<instance_t, vector<cluster_id_t> > IDPerLevel;

    vector<ClusteringStatistics>           StatisticsHistory;
    vector<vector<ClusterInformation*> >   NodesPerLevel;

    cluster_id_t                           MaxIDAssigned;

    vector<SequenceScoreValue>             CurrentClustersScores;
    double                                 CurrentGlobalScore;
    vector<percentage_t>                   GlobalScoresPerLevel;

  public:

    ClusteringRefinementAggregative(INT32          MinPoints,
                                    vector<double> EpsilonPerLevel);

    bool Run(const vector<CPUBurst*>& Bursts,
             vector<Partition>&       IntermediatePartitions,
             Partition&               LastPartition,
             bool                     PrintStepsInformation,
             string                   OutputFilePrefix = "");

  private:

    bool RunFirstAnalysis(const vector<CPUBurst*>& Bursts,
                          Partition&               FirstPartition);

    bool RunStep(size_t                   Step,
                 const vector<CPUBurst*>& Bursts,
                 Partition&               PreviousPartition,
                 Partition&               NewPartition,
                 bool&                    Stop);

    bool RunDBSCAN(const vector<const Point*>& CurrentData,
                   double                      Epsilon,
                   Partition&                  CurrentPartition);

    bool GenerateCandidatesAndBurstSubset(const vector<CPUBurst*>&     Bursts,
                                          vector<ClusterInformation*>& ParentNodes,
                                          vector<CPUBurst*>&           BurstsSubset,
                                          vector<ClusterInformation*>& NodesSubset);

    bool StopCondition(void);

    bool GenerateNodes(size_t                       Step,
                       const vector<CPUBurst*>&     Bursts,
                       Partition&                   CurrentPartition,
                       vector<ClusterInformation*>& Nodes,
                       bool                         LastPartition = false);

    void LinkNodes(const vector<CPUBurst*>&     BurstsSubset,
                   vector<ClusterInformation*>& Parent,
                   vector<ClusterInformation*>& Children,
                   Partition&                   PreviousPartition,
                   Partition&                   NewPartition);

    void GeneratePartition(Partition& NewPartition);

    bool ComputeScores(size_t                       Step,
                       const vector<CPUBurst*>&     Bursts,
                       vector<ClusterInformation*>& NewNodes,
                       Partition&                   CurrentPartition,
                       bool                         LastPartition);

    vector<pair<instance_t, cluster_id_t> > GetAssignment(ClusterInformation* Node);

    bool GenerateLastPartition(const vector<CPUBurst*>& Bursts,
                               Partition&               PreviousPartition,
                               Partition&               LastPartition);

    void LinkLastNodes(cluster_id_t        MainClusterID,
                       set<cluster_id_t>&  Merges,
                       ClusterInformation* NewNode);

    bool RenameLastPartition(const vector<CPUBurst*>& Bursts,
                             Partition& LastPartition);

    ClusterInformation* LocateNode(cluster_id_t ClusterID);

    set<set<cluster_id_t> > getSubsets(set<cluster_id_t>&          in_set,
                                       set<cluster_id_t>::iterator index);

    bool PrintPlots(const vector<CPUBurst*>& Bursts,
                    Partition&               CurrentPartition,
                    size_t                   Step);

    bool PrintTree(string TreeFileName, bool FinalTree);

    bool PrintTreeNodes(ofstream& str);

    bool PrintTreeLinks(ofstream& str);



};

struct setSizeCmp
{
  bool operator()(set<cluster_id_t> s1, set<cluster_id_t> s2) const
  {
    if (s1.size() == s2.size())
      return (s1 < s2);
    else
      return s1.size() > s2.size();
  }
};

#endif // _CLUSTERINGREFINEMENTAGGREGATIVE_HPP_
