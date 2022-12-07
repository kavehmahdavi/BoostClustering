/* Boosted density-based Kernel to apply clustering analysis*//

#ifndef __HULL_MANAGER_H__
#define __HULL_MANAGER_H__

#include <vector>
#include "libDistributedClustering.hpp"
#include "MRNet_wrappers.h"

using namespace std;

/**
 * This class manages the serialization of convex hulls through the MRNet.
 */
class HullManager
{
  public:
    HullManager();

    /* Common API */
    HullModel* Unpack(PACKET_PTR InputPacket);
    void Unpack(PACKET_PTR InputPacket, vector<HullModel *> &HullsList);


    /* Back-end API */
    void Serialize(STREAM *OutputStream, vector<HullModel*> &HullsList);
    void SerializeAll(STREAM *OutputStream, vector<HullModel*> &HullsList);

    /* Filter API */
    void Serialize(int StreamID, vector<PacketPtr> &OutputPackets, vector<HullModel*> &HullsList);
    void SerializeAll(int StreamID, vector<PacketPtr> &OutputPackets, vector<HullModel*> &HullsList);

  private:
    void SerializeAll(
      vector<HullModel*> &HullsList, 
      int                &NumberOfHulls,
      long long         *&DensityArray,
      long long         *&TotalTimeArray,
      int               *&NumberOfPointsArray,
      int               *&NumberOfDimensionsArray,
      int                &TotalNumberOfPoints,
      long long         *&InstancesArray,
      long long         *&NeighbourhoodSizesArray,
      int                &TotalDimensionsValues,
      double            *&DimensionsValuesArray);

    /* Back-end API */
    void SerializeOne(STREAM *OutputStream, HullModel *Hull);
    void SerializeDone(STREAM *OutputStream);

    /* Filter API */
    void SerializeOne(int StreamID, vector<PacketPtr> &OutputPackets, HullModel *Hull);
    void SerializeDone(int StreamID, vector<PacketPtr> &OutputPackets);
};

#endif /* __HULL_MANAGER_H__ */

