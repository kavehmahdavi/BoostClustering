/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _GMEANS_HPP_
#define _GMEANS_HPP_

#include "ClusteringAlgorithm.hpp"
#include "clustering_types.h"

/* Gmeans C code declaration */
extern "C"
{

#include "gmeans/gmeans.h"
}


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
class GMEANS:public ClusteringAlgorithm
{
  typedef size_t point_idx;
  private:

    double CriticalValue;
    INT32  MaxClusters;
    
  public:

    static const string NAME;
    
    static const string CRITICAL_VALUE_STRING;
    static const string MAX_CLUSTERS_STRING;
    GMEANS ();
    GMEANS (map < string, string > ClusteringParameters);
    ~GMEANS (void) {};
    
    bool Run (const vector <const Point *> &Data,
              Partition                    &DataPartition,
              bool                          SimpleRun);
    string GetClusteringAlgorithmName (void) const;
    string GetClusteringAlgorithmNameFile (void) const;
    bool
      ComputeParamsApproximation (const vector <const Point *> &Data,
                                  INT32                         ParametersCount, ...);
  private:
};
#endif                                            /* _DBSCAN_HPP_ */
