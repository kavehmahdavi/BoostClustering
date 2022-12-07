/* Boosted density-based Kernel to apply clustering analysis*/


#ifndef _LIBCLUSTERING_HPP_
#define _LIBCLUSTERING_HPP_

#include <string>
using std::string;
#include <map>
using std::map;

#include "clustering_types.h"
#include "Point.hpp"
#include "Partition.hpp"

class libClusteringImplementation;

class libClustering
{
  private:
    libClusteringImplementation* Implementation;

    bool   Error,        Warning;
    string ErrorMessage, WarningMessage;

  public:

    libClustering(void);

    bool InitClustering(string AlgorithmName, map<string, string> Parameters);

    bool ExecuteClustering(const vector<const Point*>& Data, Partition& DataPartition);

    bool ClassifyData(const vector<const Point*>& Data, Partition& DataPartition);

    bool ParametersApproximation(const vector<const Point*>& Data,
                                 map<string, string>         Parameters,
                                 string                      OutputFileNamePrefix);

    bool UsingADistributedAlgorithm(void);

    bool HasNoise(void);

    string GetClusteringAlgorithmName(void);

    string GetErrorMessage(void);

    string GetWarningMessage(void);

  protected:

};

#endif // _LIBCLUSTERING_HPP_
