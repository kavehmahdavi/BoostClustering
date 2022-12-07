/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _LIBCLUSTERINGIMPLEMENTATION_HPP_
#define _LIBCLUSTERINGIMPLEMENTATION_HPP_


#include <Error.hpp>
using cepba_tools::Error;

#include <libClustering.hpp>
#include <ClusteringAlgorithm.hpp>

class libClusteringImplementation: public Error
{
  private:

    ClusteringAlgorithm* Algorithm;

  public:
    libClusteringImplementation(void);

    bool InitClustering(string AlgorithmName, map<string, string> Parameters);

    bool ExecuteClustering(const vector<const Point*>& Data, Partition& Partition);

    bool ClassifyData(const vector<const Point*>& Data, Partition& DataPartition);

    bool ParametersApproximation(const vector<const Point*>& Data,
                                 map<string, string>         Parameters,
                                 string                      OutputFileNamePrefix);

    bool UsingADistributedAlgorithm(void);

    bool HasNoise(void);

    string GetClusteringAlgorithmName(void);

private:

};

#endif // _LIBCLUSTERING_HPP_
