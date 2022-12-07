/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _MUSTER_DBSCAN_HPP_
#define _MUSTER_DBSCAN_HPP_

#include "ClusteringAlgorithm.hpp"
#include "clustering_types.h"

/* Forward declarations */
class Partition;

#include <sstream>
using std::ostringstream;

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;

#include <list>
using std::list;
using std::pair;

class MUSTER_DBSCAN: public ClusteringAlgorithm
{
    typedef size_t point_idx;

  private:
    double              Eps;
    INT32               MinPoints;

  public:

    static const string NAME;
    
    static const string EPSILON_STRING;
    static const string MIN_POINTS_STRING;
    
    MUSTER_DBSCAN();
    
    MUSTER_DBSCAN(map < string, string > ClusteringParameters);
    ~MUSTER_DBSCAN(void)
    {
    };
    
    bool Run (const vector <const Point*> &Data,
              Partition                   &DataPartition,
              bool                         SimpleRun);
    
    string GetClusteringAlgorithmName (void) const;

    string GetClusteringAlgorithmNameFile (void) const;

    bool HasNoise(void) { return true; };
    
    bool ComputeParamsApproximation (const vector < const Point* > &Data,
                                     INT32                          ParametersCount, ...);
  private:
};

#endif /* _MUSTER_DBSCAN_HPP_ */
