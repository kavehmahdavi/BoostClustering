/* Boosted density-based Kernel to apply clustering analysis*/

#include "ClusteringAlgorithmsFactory.hpp"
#include "DBSCAN.hpp"
#include "GMEANS.hpp"

#ifdef HAVE_MUSTER
#include "MUSTER_DBSCAN.hpp"
#include "MUSTER_PAM.hpp"
#include "MUSTER_XCLARA.hpp"
#include "MUSTER_CAPEK.hpp"
#endif

ClusteringAlgorithmsFactory*
ClusteringAlgorithmsFactory::Instance = NULL;

ClusteringAlgorithmsFactory* ClusteringAlgorithmsFactory::GetInstance()
{
  if (Instance == NULL)
    Instance = new ClusteringAlgorithmsFactory();

  return Instance;
}

ClusteringAlgorithm*
ClusteringAlgorithmsFactory::GetClusteringAlgorithm(string                AlgorithmDefinition,
                                                    map<string, string>&  ClusteringParameters)
{
  ClusteringAlgorithm* Algorithm;
  DBSCAN             * dbScan_alg;
  
  if (AlgorithmDefinition.compare(DBSCAN::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new DBSCAN(ClusteringParameters);
  }
  else if (AlgorithmDefinition.compare(GMEANS::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new GMEANS(ClusteringParameters);
  }
#ifdef HAVE_MUSTER
  else if (AlgorithmDefinition.compare(MUSTER_DBSCAN::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new MUSTER_DBSCAN(ClusteringParameters);
  }
  else if (AlgorithmDefinition.compare(MUSTER_PAM::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new MUSTER_PAM(ClusteringParameters);
  }
  else if (AlgorithmDefinition.compare(MUSTER_XCLARA::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new MUSTER_XCLARA(ClusteringParameters);
  }
  else if (AlgorithmDefinition.compare(MUSTER_CAPEK::NAME) == 0)
  {
    return (ClusteringAlgorithm*) new MUSTER_CAPEK(ClusteringParameters);
  }
#endif
  else
  {
    Algorithm = NULL;
    Error = true;
    ErrorMessage = "unknown clustering algorithm '"+AlgorithmDefinition+"'";
  }

  return Algorithm;
}
