/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __NOISE_MANAGER_H__
#define __NOISE_MANAGER_H__

#include "libDistributedClustering.hpp"
#include "MRNet_wrappers.h"

#define PROCESS_NOISE

using MRN::Stream;
using MRN::Packet;
using MRN::PacketPtr;

/**
 * This class manages the serialization and clustering of the noise data through the MRNet.
 */
class NoiseManager
{
   public:
      /* Front-end/Filter interface */
      NoiseManager();
      NoiseManager(double Epsilon, int MinPoints);

      bool ClusterNoise(vector<const Point*>& Points, vector<long long>& Durations, vector<HullModel*>& NoiseModel, int &CountRemainingNoise);

      void Serialize(int StreamID, std::vector< PacketPtr >& OutputPackets);

      int  Unpack(PACKET_PTR in_packet, vector<const Point *> &NoisePoints, vector<long long> &NoiseDurations);


      /* Back-end interface */
      NoiseManager(libDistributedClustering *libClustering);

      void Serialize(Stream *OutputStream);

   private:
      libDistributedClustering *libClustering;

      void Serialize(int &NoiseCount, int &DimensionsCount, double *&SerialPoints, unsigned int &SerialPointsCount, long long *&SerialNoiseDurations);
};

#endif /* __NOISE_MANAGER_H__ */
