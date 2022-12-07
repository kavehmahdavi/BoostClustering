/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _OPTICS_HPP_
#define _OPTICS_HPP_

#include "ClusteringAlgorithm.hpp"
#include "clustering_types.h"

/* Forward declarations */
class Classifier;
class Partition;

#include <ANN/ANN.h>

#include <sstream>
using std::ostringstream;

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;
#include <list>
using std::list;

using std::pair;

class OPTICS: public ClusteringAlgorithm
{

  typedef size_t point_idx;
  
  private:
    double              Eps;
    INT32               MinPoints;

    /*
    vector<INT32>       ClusterIdTranslation;
    ClusterInformation* NoiseClusterInfo;
    ClusterInformation* ThresholdFilteredClusterInfo; */

    ANNpointArray       ANNDataPoints;
    ANNkd_tree*         SpatialIndex;

  public:
    static const string NAME;
    
    static const string EPSILON_STRING;
    static const string MIN_POINTS_STRING;

    /*
    DBSCAN(double _Eps, INT32 _MinPoints, double FilterThreshold = 0.0)
    :ClusteringAlgorithm(FilterThreshold),
     Eps(_Eps),
     MinPoints(_MinPoints)
    {};
    */
    OPTICS(map<string, string> ClusteringParameters);

    ~OPTICS(void) {};

    double GetEpsilon(void) const     { return Eps; };
    void   SetEpsilon(double Epsilon) { Eps = Epsilon; };

    INT32  GetMinPoints(void) const      { return MinPoints; };
    void   SetMinPoints(INT32 MinPoints) { this->MinPoints = MinPoints; };
    
    bool Run(const vector<const Point*>& Data,
             Partition&                  DataPartition,
             bool                        SimpleRun);

    /*
    Classifier*
    GetClassifier(void) { return new KDTreeClassifier(SpatialIndex, Eps); }; */
  
    string GetClusteringAlgorithmName(void) const;
    string GetClusteringAlgorithmNameFile(void) const;

    /*
    bool
    GetClustersInformation(vector<ClusterInformation*>& ClusterInfoVector);
    */

    /* bool GetDataPoints(vector<DataPoint*>& DataPoints); */

    bool HasNoise(void) { return true; };

  private:
    
    bool BuildKDTree(const vector<const Point*>& Data);

    ANNpoint ToANNPoint(const Point* const InputPoint);

};

#endif /* _OPTICS_HPP_ */

