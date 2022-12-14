/* Boosted density-based Kernel to apply clustering analysis*/
#include "ClusteringStatistics.hpp"
#include "SystemMessages.hpp"
using cepba_tools::system_messages;

#include <sstream>
using std::ostringstream;
using std::scientific;

#include <algorithm>
using std::sort;

#include <iomanip>
using std::setprecision;
using std::setw;
using std::setfill;

#include <limits>
#include <cstring>

/******************************************************************************
 * CLASS 'MetricContainer'
 *****************************************************************************/

/**
 * Parametrized constructor. Sets sizes of all vectors
 *
 * \param Name Name of the metric
 * \param HighPrecision True if the metric is obtained deriving two metrics
 * \param NumberOfClusters Number of metrics to extrapolate
 *
 */
MetricContainer::MetricContainer()
{
  Individuals = 0;
  Aggregate   = 0.0;
  Mean        = 0.0;
  M2          = 0.0;
  StdDev_2    = 0.0;
}

/**
 * Updates the statistics of the extrapolation metrics
 *
 * \param ExtrapolationData Hash containing the extrapolation data froma CPU Burst
 *
 */
void MetricContainer::Update(double NewValue)
{
  double Delta;
  Individuals = Individuals + 1;
  Aggregate   = Aggregate + NewValue;
  Delta       = NewValue - Mean;
  Mean        = Mean + (Delta/Individuals);
  M2          = M2 + Delta * (NewValue - Mean);
  StdDev_2    = M2/(Individuals-1);
}

/**
 * Print the metric mean of all clusters present in the container
 *
 * \param str Stream where the data will be flush
 * \param HasNoise True if the cluster_id noise should be printed
 * \param TranslationMap Renaming of clusters
 *
 * \return True if the metric means where correctly writen, false otherwise
 *

bool MetricContainer::Flush(ostream&              str,
                            bool                  HasNoise,
                            vector<cluster_id_t>& SortedClusters)
{
  size_t NumberOfClusters = Individuals.size();

  str << Name;

  if (HighPrecision)
  {
    str.precision(5);
  }
  else
  {
    str.precision(0);
  }
  str << fixed;

  if (HasNoise)
  {
    str << "," << Mean[SortedClusters[NOISE_CLUSTERID]];
  }

  for (size_t Cluster = 1; Cluster < NumberOfClusters; Cluster++)
  {
    str << ",";
    if (Individuals[SortedClusters[Cluster]] != 0)
    {
       str << Mean[SortedClusters[Cluster]]; // DEBUG << " (" << Individuals[SortedClusters[Cluster]] << ")";
    }
    else
    {
      str << "nan";
    }
  }

  str << '\n';

  return true;
}
*/

/******************************************************************************
 * CLASS 'StatisticsContainer'
 *****************************************************************************/

/**
 * Empty constructor
 *
 *
 */

StatisticsContainer::StatisticsContainer(void)
{
  this->OriginalClusterID = UNCLASSIFIED;

  Individuals      = 0;
  TotalDuration    = 0.0;
  DurationMean     = 0.0;
  DurationM2       = 0.0;

  ClusteringParameters = vector<MetricContainer>(0);
  ExtrapolationMetrics = vector<MetricContainer>(0);
}

/**
 * Parametrized constructor. Sets sizes of all vectors
 *
 * \param OriginalClusterID ID assigned before sorting in terms of time
 * \param ClusteringMetrics Number of metrics used in the cluster analysis
 * \param ExtrapolationMetrics Number of metrics to extrapolate
 *
 */
StatisticsContainer::StatisticsContainer(cluster_id_t OriginalClusterID,
                                         size_t       ClusteringParametersCount,
                                         size_t       ExtrapolationMetricsCount)
{
  this->OriginalClusterID = OriginalClusterID;

  Individuals      = 0;
  TotalDuration    = 0.0;
  DurationMean     = 0.0;
  DurationM2       = 0.0;

  ClusteringParameters = vector<MetricContainer>(ClusteringParametersCount);
  ExtrapolationMetrics = vector<MetricContainer>(ExtrapolationMetricsCount);
}

/**
 * Updates the statistics of a given cluster, with the data contained in the CPU burst
 *
 * \param Burst The CPU burst to take into consideration
 */
void StatisticsContainer::NewBurst(CPUBurst* Burst)
{
  double Delta;
  size_t i;
  double x;

  /* Update Duration Sum/Mean/StdDeviation */
  x = Burst->GetDuration();

  Individuals      = Individuals + 1;
  TotalDuration    = TotalDuration + x;
  Delta            = x - DurationMean;
  DurationMean     = DurationMean + (Delta/Individuals);
  DurationM2       = DurationM2 + Delta * (x - DurationMean);

  DurationStdDev_2 = DurationM2/(Individuals-1);

  /* Update Clustering Metrics values */
  /* DEBUG */
  for (size_t CurrentParameter = 0;
              CurrentParameter < ClusteringParameters.size();
              CurrentParameter++)
  {
    ClusteringParameters[CurrentParameter].Update(Burst->GetRawDimension(CurrentParameter));
  }

  /* Update Extrapolation Metrics values */

  if (ExtrapolationMetrics.size() > 0)
  {
    map<size_t, double>::const_iterator Iterator;

    for (Iterator  = Burst->GetExtrapolationDimensions().begin();
         Iterator != Burst->GetExtrapolationDimensions().end();
         ++Iterator)
    {
      size_t MetricPosition = Iterator->first;
      double MetricValue    = Iterator->second;

      ExtrapolationMetrics[MetricPosition].Update(MetricValue);
    }
  }
}

/**
 * Returns the mean of the i-th clustering metric, if it exists
 *
 * \param i Position of the clustering metric to retrieve
 *
 * \return The mean of the i-th clustering metric, NaN otherwise
 *
 */
double StatisticsContainer::GetClusteringParameterMean(size_t i)
{
  if (i >= 0 && i < ClusteringParameters.size())
  {
    return ClusteringParameters[i].GetMean();
  }
  else
  {
    return numeric_limits<float>::quiet_NaN();
  }
}

/**
 * Returns the mean of the i-th extrapolation metric, if it exists
 *
 * \param i Position of the extrapolation metric to retrieve
 *
 * \return The mean of the i-th extrapolation metric, NaN otherwise
 *
 */
double StatisticsContainer::GetExtrapolationMetricAggregate(size_t i)
{
  if (i >= 0 && i < ExtrapolationMetrics.size())
  {
    if (ExtrapolationMetrics[i].GetIndividuals() != 0)
    {
      return ExtrapolationMetrics[i].GetAggregate();
    }
    else
    {
      return numeric_limits<float>::quiet_NaN();
    }
  }
  else
  {
    return numeric_limits<float>::quiet_NaN();
  }
}

/**
 * Returns the mean of the i-th extrapolation metric, if it exists
 *
 * \param i Position of the extrapolation metric to retrieve
 *
 * \return The mean of the i-th extrapolation metric, NaN otherwise
 *
 */
double StatisticsContainer::GetExtrapolationMetricMean(size_t i)
{
  if (i >= 0 && i < ExtrapolationMetrics.size())
  {
    if (ExtrapolationMetrics[i].GetIndividuals() != 0)
    {
      return ExtrapolationMetrics[i].GetMean();
    }
    else
    {
      return numeric_limits<float>::quiet_NaN();
    }
  }
  else
  {
    return numeric_limits<float>::quiet_NaN();
  }
}

/**
 * Returns the number of individuals (observations) of the i-th extrapolation
 * metric, if it exists
 *
 * \param i Position of the extrapolation metric to retrieve
 *
 * \return The number of individuals used to compute of the i-th extrapolation
 *         mean metric, 0 otherwise
 *
 */
size_t StatisticsContainer::GetExtrapolationMetricIndividuals(size_t i)
{
  if (i >= 0 && i < ExtrapolationMetrics.size())
  {
    return ExtrapolationMetrics[i].GetIndividuals();
  }
  else
  {
    return 0;
  }
}

/**
 * Returns the variance of the i-th extrapolation metric, if it exists
 *
 * \param i Position of the extrapolation metric to retrieve
 *
 * \return The variance of the i-th extrapolation metric, NaN otherwise
 *
 */
double StatisticsContainer::GetExtrapolationMetricStdDev_2(size_t i)
{
  if (i >= 0 && i < ExtrapolationMetrics.size())
  {
    if (ExtrapolationMetrics[i].GetIndividuals() != 0)
    {
      return ExtrapolationMetrics[i].GetStdDev_2();
    }
    else
    {
      return numeric_limits<float>::quiet_NaN();
    }
  }
  else
  {
    return numeric_limits<float>::quiet_NaN();
  }
}



/**
 * Sorting operator between statistics container. If one of them represent the
 * NOISE cluster, it comes always first, otherwise, we take into account the
 * total duration
 *
 */
bool StatisticsContainer::operator<(const StatisticsContainer& other) const
{
  if (this->GetOriginalClusterID() == NOISE_CLUSTERID)
  {
    return false;
  }

  if (other.GetOriginalClusterID () == NOISE_CLUSTERID)
  {
    return true;
  }

  if (this->GetTotalDuration() < other.GetTotalDuration())
  {
    return true;
  }

  return false;
}

/******************************************************************************
 * CLASS 'ClusteringStatistics'
 *****************************************************************************/

/**
 * Parametrized constructor. Sets sizes of all vectors
 *
 * \param IDs Set containing the different IDs used
 * \param HasNoise Boolean that indicates if the current burst contain noise
 * \param ClusteringParametersNames Names of the parameters used in the cluster analysis
 * \param ClusteringParametersPrecision Precision of the parameters used in
 * \param ExtrapolationMetricsNames Names of the extrapolation metrics
 * \param ExtrapolationMetricsPrecision Precision of the extrapolation metrics
 *
 */
ClusteringStatistics::ClusteringStatistics(set<cluster_id_t>& IDs,
                                           vector<string>     ClusteringParametersNames,
                                           vector<bool>       ClusteringParametersPrecision,
                                           vector<string>     ExtrapolationMetricsNames,
                                           vector<bool>       ExtrapolationMetricsPrecision)
{
  InitStatistics(IDs,
                 ClusteringParametersNames,
                 ClusteringParametersPrecision,
                 ExtrapolationMetricsNames,
                 ExtrapolationMetricsPrecision);
}

/**
 * Initialization of all containers. Used by the parametrized constructor of by itself
 *
 * \param IDs Set containing the different IDs used
 * \param ClusteringParametersNames Names of the parameters used in the cluster analysis
 * \param ClusteringParametersPrecision Precision of the parameters used in
 * \param ExtrapolationMetricsNames Names of the extrapolation metrics
 * \param ExtrapolationMetricsPrecision Precision of the extrapolation metrics
 *
 */
void ClusteringStatistics::InitStatistics(set<cluster_id_t> &IDs,
                                          vector<string>    ClusteringParametersNames,
                                          vector<bool>      ClusteringParametersPrecision,
                                          vector<string>    ExtrapolationMetricsNames,
                                          vector<bool>      ExtrapolationMetricsPrecision)
{
  set<cluster_id_t>::iterator IDsIterator;
  size_t ClusteringParameters = ClusteringParametersNames.size();
  size_t ExtrapolationMetrics = ExtrapolationMetricsNames.size();
  size_t Position;

  this->NumberOfClusters = IDs.size();
  this->HasNoise         = HasNoise;

  this->ClusteringParametersNames     = ClusteringParametersNames;
  this->ClusteringParametersPrecision = ClusteringParametersPrecision;
  this->ExtrapolationMetricsNames     = ExtrapolationMetricsNames;
  this->ExtrapolationMetricsPrecision = ExtrapolationMetricsPrecision;

  TotalBurstsDuration = 0.0;

  StatisticsPerCluster.clear();

  /* DEBUG
  cout << "IDs = ";
  */

  for (IDsIterator  = IDs.begin(), Position = 0;
       IDsIterator != IDs.end();
       ++IDsIterator)
  {
    if (((*IDsIterator) == NOISE_CLUSTERID))
    {
      NoiseStatistics = StatisticsContainer (NOISE_CLUSTERID,
                                             ClusteringParameters,
                                             ExtrapolationMetrics);
      this->HasNoise = true;
    }
    else
    {
      IDsPosition[(*IDsIterator)] = Position;
      StatisticsPerCluster.push_back(StatisticsContainer((*IDsIterator),
                                                         ClusteringParameters,
                                                         ExtrapolationMetrics));
      Position++;
    }

    /* DEBUG
    cout << (*IDsIterator) << " ";
    */
  }

  Translated = false;

  /* DEBUG
  cout << endl;
  */
}

/**
 * Updates the statistics of the container
 *
 * \param Bursts The set of burst used in the cluster analysis
 * \param IDs    The set cluster ids of the bursts analyzed
 *
 * \return True if the statistics has been computed correctly, false otherwise
 */
bool ClusteringStatistics::ComputeStatistics(const vector<CPUBurst*>&    Bursts,
                                             const vector<cluster_id_t>& IDs)
{
  return ComputeStatistics(Bursts.begin(), Bursts.end(), Bursts.size(), IDs);

}

/**
 * Returns the translation of the cluster IDs, in terms of cluster aggregate duration
 *
 * \param NewIDs In/Out vector that contains the original IDs as input and the translation as output
 *
 */
void ClusteringStatistics::TranslatedIDs(vector<cluster_id_t>& NewIDs)
{
  map<cluster_id_t, cluster_id_t>::iterator TranslationMapIterator;

  cluster_id_t NewClusterId;

  sort(StatisticsPerCluster.rbegin(), StatisticsPerCluster.rend());

  if (HasNoise)
  {
    TranslationMap[NOISE_CLUSTERID] = NOISE_CLUSTERID;
  }

  /* DEBUG
  cout << "StatisticsPerCluster.size = " << StatisticsPerCluster.size() << endl;
  cout << "MIN_CLUSTERID = " << MIN_CLUSTERID << endl;
  */

  for (size_t i = 0, NewClusterId = MIN_CLUSTERID;
       i < StatisticsPerCluster.size();
       i++, NewClusterId++)
  {
    /* DEBUG
    cout << "i = " << i << " NewClusterId = " << NewClusterId << endl;
    */

    cluster_id_t OriginalClusterID = StatisticsPerCluster[i].GetOriginalClusterID();

    TranslationMap[OriginalClusterID] = NewClusterId;
  }

  /* DEBUG
  cout << "Translations" << endl;
  for (TranslationMapIterator  = TranslationMap.begin();
       TranslationMapIterator != TranslationMap.end();
       ++TranslationMapIterator)
  {
    cout << "Cluster " << TranslationMapIterator->first;
    cout << " -> " << TranslationMapIterator->second;
    cout << endl;
  }
  */

  for (size_t i = 0; i < NewIDs.size(); i++)
  {
    NewIDs[i] = TranslationMap[NewIDs[i]];
  }

  Translated = true;

  return;
}

/**
 * Returns the percentage durations of each cluster
 *
 * \return The vector containing the percentage durations
 */
map<cluster_id_t, percentage_t> ClusteringStatistics::GetPercentageDurations(void)
{
  map<cluster_id_t, percentage_t> Result;

  sort(StatisticsPerCluster.rbegin(), StatisticsPerCluster.rend());

  /* DEBUG
  cout << fixed;
  cout << setprecision(0);
  cout << "TotalBurstsDuration = " << TotalBurstsDuration << endl;

  if (HasNoise)
  {
    cout << "NOISE      = " << NoiseStatistics.GetTotalDuration();
    cout << " (" << NoiseStatistics.GetIndividuals() << ")" << endl;
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    cout << "Cluster " << setw(2) << setfill(' ');
    cout << StatisticsPerCluster[i].GetOriginalClusterID();
    cout << " = " << StatisticsPerCluster[i].GetTotalDuration();
    cout << " (" << StatisticsPerCluster[i].GetIndividuals() << ")" << endl;
  }
  */

  if (HasNoise)
  {
    Result[NOISE_CLUSTERID] = NoiseStatistics.GetTotalDuration()/TotalBurstsDuration;
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    if (Translated)
    {
      Result[MIN_CLUSTERID+i] = StatisticsPerCluster[i].GetTotalDuration()/TotalBurstsDuration;
    }
    else
    {
      Result[StatisticsPerCluster[i].GetOriginalClusterID()] =
        StatisticsPerCluster[i].GetTotalDuration()/TotalBurstsDuration;
    }
  }

  return Result;
}

/**
 * Returns the durations sums of each cluster
 *
 * \return The vector containing the durations sums
 */
map<cluster_id_t, double> ClusteringStatistics::GetDurationSums(void)
{
  map<cluster_id_t, double> Result;

  sort(StatisticsPerCluster.rbegin(), StatisticsPerCluster.rend());

  if (HasNoise)
  {
    Result[NOISE_CLUSTERID] = NoiseStatistics.GetTotalDuration();
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    if (Translated)
    {
      Result[MIN_CLUSTERID+i] = StatisticsPerCluster[i].GetTotalDuration();
    }
    else
    {
      Result[StatisticsPerCluster[i].GetOriginalClusterID()] =
        StatisticsPerCluster[i].GetTotalDuration();
    }
  }

  return Result;
}

/**
 * Returns the individuals of each cluster
 *
 * \return The vector containing the durations sums
 */
map<cluster_id_t, size_t> ClusteringStatistics::GetIndividuals(void)
{
  map<cluster_id_t, size_t> Result;

  sort(StatisticsPerCluster.rbegin(), StatisticsPerCluster.rend());

  if (HasNoise)
  {
    Result[NOISE_CLUSTERID] = NoiseStatistics.GetIndividuals();
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    if (Translated)
    {
      Result[MIN_CLUSTERID+i] = StatisticsPerCluster[i].GetIndividuals();
    }
    else
    {
      Result[StatisticsPerCluster[i].GetOriginalClusterID()] =
        StatisticsPerCluster[i].GetIndividuals();
    }
  }

  return Result;
}

/**
 * Returnts the StatisticsContainer object that aggregates the different
 * statistics of the cluster with the ID provided
 *
 * \param ID Identifier of the cluster whose statistics we want to retrieve
 *
 * \param Statistics I/O Statistics container with the desired information,
 *                   if the ID is present in the statistics set
 *
 * \return True if the ID is present on the statistics set, false otherwise
 */
bool ClusteringStatistics::GetClusterStatistics(cluster_id_t         ID,
                                                StatisticsContainer& Statistics)
{
  if ( (ID != NOISE_CLUSTERID && IDsPosition.count(ID) == 0) ||
       (ID == NOISE_CLUSTERID && !HasNoise) )
  {
    return false;
  }
  else
  {
    if (ID == NOISE_CLUSTERID)
    {
      Statistics = NoiseStatistics;
    }
    else
    {
      Statistics = StatisticsPerCluster[IDsPosition[ID]];
    }

    return true;
  }
}

/**
 * Flush the statistics in the indicated stream
 *
 * \param str The stream where the information will be flushed
 *
 * \return True if the flush was done correctly, false otherwise
 */
bool ClusteringStatistics::Flush(ostream& str)
{
  sort(StatisticsPerCluster.rbegin(), StatisticsPerCluster.rend());

  /* Header */
  str << "Cluster Name";

  /* JGG (2013/05/27): NOISE is always printed, even when it does not group
   * any individual */
  // if (HasNoise)
  // {
  str << ",NOISE";
  //}

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    if (Translated)
    {
      str << ",Cluster " << i+1;
    }
    else
    {
      str << ", Cluster " << StatisticsPerCluster[i].GetOriginalClusterID();
    }
  }

  str << '\n'; // System aware endline

  /* Density */
  str << "Density";

  str.precision(0);
  str << fixed;

  if (HasNoise)
  {
    str << "," << NoiseStatistics.GetIndividuals();
  }
  else
  {
    str << ",0";
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    str << "," << StatisticsPerCluster[i].GetIndividuals();
  }

  str << '\n';

  /* Total Duration */
  str << "Total Duration";

  // str.precision(3);
  // str << scientific;

  if (HasNoise)
  {
    str << ",";

    str << NoiseStatistics.GetTotalDuration();
  }
  else
  {
    str << ",0";
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    str << ",";

    str << StatisticsPerCluster[i].GetTotalDuration();
  }

  str << '\n';

  /* Avg. Duration */
  str << "Avg. Duration";

  str.precision(0);
  str << fixed;
  if (HasNoise)
  {
    str << "," << NoiseStatistics.GetDurationMean();
  }
  else
  {
    str << ",0";
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    str << "," << StatisticsPerCluster[i].GetDurationMean();
  }

  str << '\n';

  /* % Total Duration */
  str << "% Total Duration";

  str.precision(5);
  str << fixed;
  if (HasNoise)
  {
    str << "," << NoiseStatistics.GetTotalDuration()/TotalBurstsDuration;
  }
  else
  {
    str << ",0";
  }

  for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
  {
    str << "," << StatisticsPerCluster[i].GetTotalDuration()/TotalBurstsDuration;
  }

  str << '\n';

  /* Clustering Metrics: ORIGINAL 'clusters_info' file didn't have clustering metrics
  for (size_t CurrentMetric = 0; CurrentMetric < ClusteringMetrics; CurrentMetric++)
  {
    str << ClusteringParameterNames[CurrentMetric];

    if (HasNoise)
    {
      vector<double>& NoiseClusteringMetrics = ClusteringParametersMean[TranslationMap[NOISE_CLUSTERID]];
      str << ", " << NoiseClusteringMetrics[CurrentMetric];
    }

    for (size_t Cluster = 1; Cluster < NumberOfClusters; Cluster++)
    {
      vector<double>& NoiseClusteringMetrics = ClusteringParametersMean[TranslationMap[Cluster]];
      str << ", " << NoiseClusteringMetrics[CurrentMetric];
    }

    str << '\n';
  }
  */

  /* Extrapolation Metrics */
  for (size_t CurrentMetric = 0; CurrentMetric < ExtrapolationMetricsNames.size(); CurrentMetric++)
  {
    str << ExtrapolationMetricsNames[CurrentMetric];

    if (ExtrapolationMetricsPrecision[CurrentMetric])
    {
      str.precision(5);
    }
    else
    {
      str.precision(0);
    }
    str << fixed;

    if (HasNoise)
    {
      str << "," << NoiseStatistics.GetExtrapolationMetricMean(CurrentMetric);
    }
    else
    {
      str << ",0";
    }

    for (size_t i = 0; i < StatisticsPerCluster.size(); i++)
    {
      str << "," << StatisticsPerCluster[i].GetExtrapolationMetricMean(CurrentMetric);
    }

    str << '\n';
  }

  return true;
}



