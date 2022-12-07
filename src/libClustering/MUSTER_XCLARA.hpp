/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _MUSTER_XCLARA_HPP_
#define _MUSTER_XCLARA_HPP_

#include "clustering_types.h"
#include "ClusteringAlgorithm.hpp"
#include "Partition.hpp"

#include <kmedoids.h>

#include <sstream>
using std::ostringstream;

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;

#include <list>
using std::list;
using std::pair;

class MUSTER_XCLARA: public ClusteringAlgorithm
{
    typedef size_t point_idx;

  private:
    INT32               MaxClusters;

  public:

    static const string NAME;
    static const string MAX_CLUSTERS_STRING;
    
    MUSTER_XCLARA();
    
    MUSTER_XCLARA(map < string, string > ClusteringParameters);
    ~MUSTER_XCLARA(void)
    {
    };
    
    bool Run (const vector <const Point*> &Data,
              Partition                   &DataPartition,
              bool                         SimpleRun);
    
    string GetClusteringAlgorithmName (void) const;

    string GetClusteringAlgorithmNameFile (void) const;

    bool ComputeParamsApproximation (const vector < const Point* > &Data,
                                     INT32                          ParametersCount, ...);
  private:

    void ProcessClusterAssignment(cluster::kmedoids &muster_algorithm,
                                  Partition         &DataPartition,
                                  size_t             DataSize);
};

#endif /* _MUSTER_XCLARA_HPP_ */
