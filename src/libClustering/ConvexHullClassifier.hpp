/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CONVEX_HULL_CLASSIFIER_HPP_
#define _CONVEX_HULL_CLASSIFIER_HPP_

#include <Error.hpp>
using cepba_tools::Error;
#include <SystemMessages.hpp>
using cepba_tools::system_messages;

#include "Classifier.hpp"
#include "ConvexHullModel.hpp"

#include <iostream>

#include <sstream>
using std::ostringstream;

/* Forward declarations */

class ConvexHullClassifier: public Classifier
{
  protected:

    double                   Eps;
    int                      MinPoints;
    vector<ConvexHullModel>& HullModels;

  public:

    ConvexHullClassifier(vector<ConvexHullModel>& _HullModels,
                         double                   _Eps,
                         int                      _MinPoints):
    HullModels(_HullModels),
    Eps(_Eps),
    MinPoints(_MinPoints)
    {
      ostringstream Message;
      Message << "Classifier - Total Models = " << _HullModels.size();
      Message << " Epsilon = " << _Eps;
      Message << " MinPoints = " << _MinPoints << std::endl;
      system_messages::information(Message.str());
    };

    bool Classify(vector<const Point*>& Data,
                  Partition&            DataPartition);

    template <typename T>
    bool Classify(T begin, T end, size_t size, Partition& DataPartition);

    bool Classify(const Point* QueryPoint, cluster_id_t& ID);

  private:

};

template <typename T>
bool ConvexHullClassifier::Classify(T          begin,
                                    T          end,
                                    size_t     size,
                                    Partition& DataPartition)
{
  T PointsIt;

  cluster_id_t          CurrentClusterId;
  vector<cluster_id_t>& AssignmentVector = DataPartition.GetAssignmentVector();
  set<cluster_id_t>     DifferentIDs;

  ostringstream Message;

  for (PointsIt = begin; PointsIt != end; ++PointsIt)
  {
    /* DEBUG
    Message.str("");
    Message << "Point[" << i << "]";
    system_messages::information(Message.str()); */

    Classify((Point*) (*PointsIt), CurrentClusterId);
    AssignmentVector.push_back(CurrentClusterId);

    DifferentIDs.insert(CurrentClusterId);

    /* DEBUG
    Message.str("");
    Message << " ID = " << CurrentClusterId << endl;
    system_messages::information(Message.str()); */
  }

  DataPartition.SetIDs(DifferentIDs);

  // DataPartition.NumberOfClusters(HullModels.size()+1);
  // DataPartition.HasNoise(true);

  /* DEBUG
  Message.str("");
  Message << "IDs in use { ";
  set<cluster_id_t>::iterator IDsIt;
  for (IDsIt  = DifferentIDs.begin();
       IDsIt != DifferentIDs.end();
       ++IDsIt)
  {
    Message << *IDsIt << " ";
  }
  Message << "}" << endl;


  // Message << "Classification Partition Size = " << DataPartition.NumberOfClusters() << " ";
  // Message << "Has noise? " << DataPartition.HasNoise() << endl;
  system_messages::information(Message.str().c_str());
  */

  return true;
}

#endif /* _NEARESTNEIGHBOURCLASSIFIER_HPP_ */
