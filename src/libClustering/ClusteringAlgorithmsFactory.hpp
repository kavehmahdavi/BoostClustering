/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTERINGALGORITHMSFACTORY_HPP_
#define _CLUSTERINGALGORITHMSFACTORY_HPP_

#include "ClusteringAlgorithm.hpp"

class ClusteringAlgorithmsFactory
{
  public:
    static ClusteringAlgorithmsFactory* GetInstance();

    ClusteringAlgorithm* GetClusteringAlgorithm(string                AlgorithmDefinition,
                                                map<string, string>&  ClusteringParameters);

    string GetLastError(void) { return ErrorMessage; };
    
  private:
    bool   Error;

    string ErrorMessage;
    
    static ClusteringAlgorithmsFactory* Instance;
    
    ClusteringAlgorithmsFactory(void)  { Error = false; ErrorMessage = ""; };
    ~ClusteringAlgorithmsFactory(void) {};
};

#endif
