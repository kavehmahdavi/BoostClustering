/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _NEARESTNEIGHBOURCLASSIFIER_HPP_
#define _NEARESTNEIGHBOURCLASSIFIER_HPP_

#include <Error.hpp>
using cepba_tools::Error;

#include "Classifier.hpp"

#include <ANN/ANN.h>

/* Forward declarations */

class NearestNeighbourClassifier: public Classifier
{
  protected:

    double               Eps;
    ANNkd_tree*          SpatialIndex;
    vector<cluster_id_t> Partition;

  public:
    
    NearestNeighbourClassifier(ANNkd_tree*           _SpatialIndex,
                               vector<cluster_id_t>& _Partition,
                               double                _Eps):
    SpatialIndex(_SpatialIndex),
    Partition(_Partition),
    Eps(_Eps)
    {}

    NearestNeighbourClassifier(vector<Point*>        Data,
                               vector<cluster_id_t>& Partition,
                               double                Eps);
    
    bool Classify(vector<Point*>& Data,
                  bool            AddData = false);
  
    bool Classify(Point* QueryPoint, cluster_id_t& ID);
  private:
    
    ANNpoint ToANNPoint(Point* InputPoint);
  
};

#endif /* _NEARESTNEIGHBOURCLASSIFIER_HPP_ */
