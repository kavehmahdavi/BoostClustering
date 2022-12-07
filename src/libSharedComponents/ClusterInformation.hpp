/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTERINFORMATION_HPP_
#define _CLUSTERINFORMATION_HPP_

#include <Error.hpp>
using cepba_tools::Error;

#include <libClustering.hpp>
#include "trace_clustering_types.h"

#include <vector>
using std::vector;

typedef UINT32 node_id_t;

class ClusterInformation: public Error
{
  public:
    static node_id_t NodeIDNumber;
    
  private:
    UINT32                      NodeID;
    
    cluster_id_t                ID;

    bool                        Discarded;
    bool                        Reclassification;
    bool                        _Visited; 

    percentage_t                Score;
    size_t                      Occurrences;
    timestamp_t                 TotalDuration;
    size_t                      Individuals;

    vector<instance_t>          Instances;
    
    vector<ClusterInformation*> Children; /* Both divisive and aggregative */
    vector<ClusterInformation*> Parents;  /* Just for aggregative */

  public:
    ClusterInformation(cluster_id_t ID,
                       percentage_t Score,
                       size_t       Occurrences,
                       timestamp_t  TotalDuration,
                       size_t       Individuals);
                       
    ClusterInformation(cluster_id_t ID,
                       timestamp_t  TotalDuration,
                       size_t       Individuals);

    node_id_t    GetNodeID(void)        { return this->NodeID; };
    
    void         SetID(cluster_id_t ID) { this->ID = ID; };
    cluster_id_t GetID(void)            { return this->ID; };

    void         SetScore(percentage_t Score) { this->Score = Score; };
    percentage_t GetScore(void)               { return Score; };
    
    void         SetOccurrences(size_t Occurrences) { this->Occurrences = Occurrences; };
    size_t       GetOccurrences(void)               { return Occurrences; };
    
    timestamp_t  GetTotalDuration(void) { return TotalDuration; };
    size_t       GetIndividuals(void)   { return Individuals; };

    bool IsCandidate(size_t      TotalOccurrences,
                     timestamp_t ClustersTotalDuration);

    void SetInstances(vector<instance_t>& Instances) { this->Instances = Instances; };
    vector<instance_t>& GetInstances(void)           { return this->Instances; };

    void AddInstance(instance_t Instance) { Reclassification = true;
                                            Individuals++;
                                            Instances.push_back(Instance); };

    bool AddChild(ClusterInformation*);
    vector<ClusterInformation*>& GetChildren(void) { return Children; };
    
    bool AddParent(ClusterInformation*);
    vector<ClusterInformation*>& GetParents(void)  { return Parents; };

    size_t TotalClusters(void);

    /* Used in the aggregative */
    void RenameNode(cluster_id_t& MaxIDAssigned);

    /* Used in the divisive */
    void RenameChildren(cluster_id_t& RestOfChildrenID);

    bool IsLeaf(void);
    
    bool IsDiscarded(void) { return this->Discarded; };
    
    void Discard(void) {
      /* DEBUG 
      cout << "NODE = " << ID << " IS BEING DISCARDED" << endl; */
      this->Discarded = true; };

    void SetReclassified(void)
    {
      Reclassification = true;
    }

    bool Visited(void)         { return _Visited; };
    void Visited(bool Visited) { _Visited = Visited; };

    string NodeName(void);

    string NodeLabel(void);

    string Color(void);
};

class ClusterInformationIDOrder
{
  public:
    bool operator()(ClusterInformation* Node1, ClusterInformation* Node2)
    {
      if (Node1->GetID() <= Node2->GetID())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
};

#endif /* _CLUSTERINFORMATION_HPP_ */
