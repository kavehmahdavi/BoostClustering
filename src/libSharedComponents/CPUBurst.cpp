/* Boosted density-based Kernel to apply clustering analysis*/

#include <CPUBurst.hpp>

CPUBurst::CPUBurst(task_id_t            TaskId,
                   thread_id_t          ThreadId,
                   line_t               Line,
                   timestamp_t          BeginTime,
                   timestamp_t          EndTime,
                   duration_t           Duration,
                   vector<double>&      ClusteringRawData,
                   vector<double>&      ClusteringProcessedData,
                   map<size_t, double>& ExtrapolationData,
                   burst_type_t         BurstType,
                   bool                 ToClassify)
:Point (ClusteringProcessedData)
{
  this->TaskId                  = TaskId;
  this->ThreadId                = ThreadId;
  this->Line                    = Line;
  this->BeginTime               = BeginTime;
  this->EndTime                 = EndTime;
  this->Duration                = Duration;
  this->RawDimensions           = ClusteringRawData;
  this->ExtrapolationDimensions = ExtrapolationData;
  this->BurstType               = BurstType;
  this->ToClassify              = ToClassify;
}

CPUBurst::CPUBurst(instance_t          Instance,
                   task_id_t           TaskId,
                   thread_id_t         ThreadId,
                   line_t              Line,
                   timestamp_t         BeginTime,
                   timestamp_t         EndTime,
                   duration_t          Duration,
                   vector<double>&     ClusteringRawData,
                   vector<double>&     ClusteringProcessedData,
                   map<size_t, double>& ExtrapolationData,
                   burst_type_t        BurstType,
                   bool                ToClassify)
:Point(Instance, ClusteringProcessedData)
{
  this->TaskId                  = TaskId;
  this->ThreadId                = ThreadId;
  this->Line                    = Line;
  this->BeginTime               = BeginTime;
  this->EndTime                 = EndTime;
  this->Duration                = Duration;
  this->RawDimensions           = ClusteringRawData;
  this->ExtrapolationDimensions = ExtrapolationData;
  this->BurstType               = BurstType;
  this->ToClassify              = ToClassify;
}

CPUBurst::~CPUBurst(void)
{
  /*
  RawDimensions.clear();
  Dimensions.clear();
  ExtrapolationDimensions.clear();
  */
}

double CPUBurst::GetRawDimension(size_t Index)
{
  if (Index >= RawDimensions.size() || Index < 0)
    return 0.0;
  else
    return RawDimensions[Index];
}

size_t CPUBurst::GetExtrapolationDimensionsCount(void) const
{
  return ExtrapolationDimensions.size();
};

burst_type_t CPUBurst::GetBurstType(void) const
{
  return this->BurstType;
}

bool CPUBurst::Scale(vector<double>& Mean, vector<double>& RMS)
{
  if ( Mean.size() != Dimensions.size() ||
       RMS.size()  != Dimensions.size())
    return false;


  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    Dimensions[i] =
      (Dimensions[i]-Mean[i])/RMS[i];
  }

  return true;
}

bool
CPUBurst::MeanAdjust(vector<double>& DimensionsAverage)
{
  if (DimensionsAverage.size() != RawDimensions.size())
    return false;

  for (INT32 i = 0; i < DimensionsAverage.size(); i++)
  {
    Dimensions[i] = RawDimensions[i] - DimensionsAverage[i];
  }

  return true;
}

bool
CPUBurst::BaseChange(vector< vector<double> >& BaseChangeMatrix)
{
  vector<double> BaseChangedDimensions (BaseChangeMatrix.size());

  /* DEBUG
  cout << "Original RawDimensions = {";
  for (INT32 i = 0; i < RawDimensions.size(); i++)
  {
    cout << RawDimensions[i] << " ";
  }
  cout << "}" << endl;

  cout << "Original NormalizedDimensions = {";
  for (INT32 i = 0; i < NormalizedDimensions.size(); i++)
  {
    cout << NormalizedDimensions[i] << " ";
  }
  cout << "}" << endl;
  */


  for (INT32 i = 0; i < BaseChangeMatrix.size(); i++)
  {
    if (BaseChangeMatrix[i].size() != Dimensions.size())
    {
      /* DEBUG */
      cout << "ERROR!: BaseChangeMatrix.size = " << BaseChangeMatrix[i].size();
      cout << " NormalizedDimensions.size = " << Dimensions.size() << endl;
      return false;
    }

    BaseChangedDimensions[i] = 0.0;

    for (INT32 j = 0; j < BaseChangeMatrix[i].size(); j++)
    {
      BaseChangedDimensions[i] +=
        (Dimensions[j]*BaseChangeMatrix[i][j]);
    }
  }

  Dimensions = BaseChangedDimensions;

  /* DEBUG
  cout << "Base changed NormalizedDimensions = {";
  for (INT32 i = 0; i < NormalizedDimensions.size(); i++)
  {
    cout << NormalizedDimensions[i] << " ";
  }
  cout << "}" << endl;
  */

  return true;
}

/**
 * Print all information contained in the point
 * \param str Output stream where information will be flushed
 * \param ExtrapolationDimensionsTotalSize Number of extrapolation dimensions to correctly flush this partial data
 * \resutl True if point has been written correctly, false otherwise
 */
bool
CPUBurst::Print(ostream&       str,
                 vector<bool>& ClusteringParametersPrecision,
                 vector<bool>& ExtrapolationParametersPrecision,
                 cluster_id_t   ClusterId)
{
  map<size_t, double>::iterator ExtrapolationData;

  size_t TotalExtrapolationDimensions = ExtrapolationParametersPrecision.size();

  /* Common data */
  str << Instance  << "," << TaskId  << "," << ThreadId << ",";
  str << BeginTime << "," << EndTime << "," << Duration << "," << Line;

  str.setf(ios::fixed,ios::floatfield);

  /* Clustering Dimensions Raw */
  for (INT32 i = 0; i < RawDimensions.size(); i++)
  {
    if (ClusteringParametersPrecision[i])
    { /* High precision */
      str.precision(6);
    }
    else
    {
      str.precision(0);
    }

    str << "," << RawDimensions[i];
  }

  /* Clustering Dimensions Normalized */
  str.precision(6);
  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    str << ", " << Dimensions[i];
  }

  /* Extrapolation Dimensions */
  for (size_t i = 0; i < TotalExtrapolationDimensions; i++)
  {
    str << ",";

    ExtrapolationData = ExtrapolationDimensions.find(i);
    if (ExtrapolationData != ExtrapolationDimensions.end())
    {

      if (ExtrapolationParametersPrecision[i])
      { /* High precision */
        str.precision(6);
      }
      else
      {
        str.precision(0);
      }

      str << ExtrapolationData->second;
    }
    else
    {
      str << "nan";
    }
  }

  /* ClusterID */
  str << "," << ClusterId << endl;

  return true;
}

/* DEBUG */
void
CPUBurst::PrintBurst(void)
{
  cout << "Instance: " << Instance;

  PrintPoint ();
}

string CPUBurst::BurstTypeStr(burst_type_t T)
{
  switch(T)
  {
    case CompleteBurst:
      return "CompleteBurst";
      break;
    case MissingDataBurst:
      return "MissingDataBurst";
      break;
    case DurationFilteredBurst:
      return "DurationFilteredBurst";
      break;
    case RangeFilteredBurst:
      return "RangeFilteredBurst";
      break;
    default:
      return "ERROR";
      break;
  }
}
