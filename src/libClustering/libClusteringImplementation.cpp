/* Boosted density-based Kernel to apply clustering analysis*/

#include "libClusteringImplementation.hpp"

#include "ClusteringAlgorithmsFactory.hpp"

#include <cerrno>
#include <cstring>

#include <fstream>
using std::ofstream;
using std::ios_base;

/**
 * Empty constructor
 */
libClusteringImplementation::libClusteringImplementation(void)
{
  Algorithm = NULL;
}

/**
 * Initialization of the clustering algorithm
 *
 * \param AlgorithmName Name of the clustering algorithm to be used
 * \param AlgorithmParameters Map with the parameters needed by the algorithm
 *
 * \return True if the library has been correctly initialized, false otherwise
 */
bool libClusteringImplementation::InitClustering(string              AlgorithmName,
                                                 map<string, string> AlgorithmParameters)
{
  if (Algorithm != NULL)
  {
    delete Algorithm;
  }

  ClusteringAlgorithmsFactory* AlgorithmsFactory;

  AlgorithmsFactory = ClusteringAlgorithmsFactory::GetInstance();

  Algorithm = AlgorithmsFactory->GetClusteringAlgorithm(AlgorithmName,
                                                        AlgorithmParameters);

  if (Algorithm == NULL)
  {
    SetErrorMessage(AlgorithmsFactory->GetLastError());
    return false;
  }

  if (Algorithm->GetError())
  {
    SetErrorMessage(Algorithm->GetLastError());
    return false;
  }

  return true;
}

/**
 * Actual execution of the clustering algorithm
 *
 * \param Data Input vector containing the geometrical points to cluster. See class Point
 * \param Partition Output vector containing the assigned cluster ids for each point in the input vector
 *
 * \return True if the clustering algorithm was correctly applied, false otherwise
 */
bool libClusteringImplementation::ExecuteClustering(const vector<const Point*>& Data,
                                                    Partition&                  DataPartition)
{
  if (Algorithm == NULL)
  {
    SetErrorMessage("clustering algorithm not initialized");
    SetError(true);
    return false;
  }

  if (!Algorithm->Run(Data, DataPartition))
  {
    SetErrorMessage(Algorithm->GetLastError());
    SetError(true);
    return false;
  }

  return true;
}

/**
 * Performs the classification of the data using the (possible) internal
 * classification structures of the clustering algorithm
 *
 * \param Data      Input vector containing the geometrical points to cluster. See class Point
 * \param Partition Output class containing the assigned cluster ids for each point in the input vector
 *
 * \return True if the classification was correctly applied, false otherwise
 */
bool libClusteringImplementation::ClassifyData(const vector<const Point*> &Data,
                                               Partition                  &DataPartition)
{
  if (Algorithm == NULL)
  {
    SetErrorMessage("clustering algorithm not initialized");
    SetError(true);
    return false;
  }

  if (!Algorithm->Classify(Data, DataPartition))
  {
    SetErrorMessage(Algorithm->GetLastError());
    SetError(true);
    return false;
  }

  return true;
}

/**
 * Generates a possible parameter approximation needed by the cluster algorithm
 *
 * \param OutputFileNamePrefix The prefix of the output files that will be generated
 * \param Parameters Map of key and value strings parameters of the approximation
 *
 * \result True if the approximation wero done correctly, false otherwise
 */
bool libClusteringImplementation::ParametersApproximation(const vector<const Point*>& Data,
                                                          map<string, string>         Parameters,
                                                          string                      OutputFileNamePrefix)
{
  if (Algorithm == NULL)
  {
    SetErrorMessage("clustering algorithm not initialized");
    SetError(true);
    return false;
  }

  if (!Algorithm->ParametersApproximation(Data,
                                          Parameters,
                                          OutputFileNamePrefix))
  {
    SetErrorMessage(Algorithm->GetLastError());
    SetError(true);
    return false;
  }

  return true;
}

/**
 * Return true if the clustering algorithm to be used is based on MPI
 *
 * \return True if the clustering algorithm is based on MPI
 */
bool libClusteringImplementation::UsingADistributedAlgorithm(void)
{
  if (Algorithm == NULL)
  {
    return false;
  }

  return Algorithm->IsDistributed();
}

/**
 * Check if the algorithm uses a noise cluster
 *
 * \return True if the clustering algorithm returns noise clusters
 */
bool libClusteringImplementation::HasNoise(void)
{
  if (Algorithm == NULL)
  {
    return false;
  }

  return Algorithm->HasNoise();
}

/**
 * Returns the name of the clustering algorithm to be used
 *
 * \return String with the name and parameters of the clustering algorithm  used
 */
string libClusteringImplementation::GetClusteringAlgorithmName(void)
{
  if (Algorithm == NULL)
  {
    return string("No algorithm initialized");
  }

  return Algorithm->GetClusteringAlgorithmName();
}
