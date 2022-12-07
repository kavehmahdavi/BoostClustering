/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _PARTITION_HPP_
#define _PARTITION_HPP_

#include "clustering_types.h"

#include <vector>
using std::vector;

#include <set>
using std::set;

class Partition
{
  private:
    set<cluster_id_t>    _IDs;
    vector<cluster_id_t> _ClusterAssignmentVector;


  public:

    Partition(void);

    void clear(void);

    vector<cluster_id_t>& GetAssignmentVector(void);
    void SetAssignmentVector(vector<cluster_id_t>& ClusterAssignmentVector);

    set<cluster_id_t>& GetIDs(void);
    void               SetIDs(set<cluster_id_t>& IDs);
    cluster_id_t       GetMaxID(void);

    void MergeIDs(set<cluster_id_t>& OriginalIDs, cluster_id_t DefinitiveID);

    void UpdateIDs(void);

    size_t NumberOfClusters(void) const;
    bool   HasNoise(void) const;

};

#endif // _PARTITION_HPP_
