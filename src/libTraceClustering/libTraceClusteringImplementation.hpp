/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _LIBTRACECLUSTERINGIMPLEMENTATION_HPP_
#define _LIBTRACECLUSTERINGIMPLEMENTATION_HPP_

#include <string>
using std::string;

#include <Error.hpp>
using cepba_tools::Error;

#include <libClustering.hpp>
#include <TraceData.hpp>
#include <ClusteringStatistics.hpp>

#include "trace_clustering_types.h"

class libTraceClusteringImplementation: public Error
{

    static const string  DataFilePostFix;
    static const string  SampledDataFilePostFix;

  private:

    TraceData           *Data;
    libClustering       *ClusteringCore;
    Partition            LastPartition, ClassificationPartition;
    ClusteringStatistics Statistics;

    string               InputFileName;
    input_file_t         InputFileType;

    bool                 SampleData;

    bool                 ClusteringExecuted;
    bool                 ClusteringRefinementExecution;

    bool                 PRVEventsParsing;
    bool                 ConsecutiveEvts;
    set<event_type_t>    EventsToDealWith;

    unsigned char  UseFlags;

  public:
    libTraceClusteringImplementation(bool verbose,
                                     bool paraver_verbosity = false);

    bool InitTraceClustering(string        ClusteringDefinitionXML,
                             string        PCFFileName,
                             bool          UseSemanticValue,
                             bool          ApplyLogToSemanticValue,
                             unsigned char Flags);

    bool SetDBSCANParameters(double Eps, int MinPoints);

    bool ExtractData(string            InputFileName,
                     bool              SampleData = false,
                     unsigned int      MaxSamples = 0,
                     set<event_type_t> EventsToDealWith = set<event_type_t> (),
                     bool              ConsecutiveEvts  = false,
                     string            InputSemanticCSV = "");

    bool FlushData(string OutputCSVFileNamePrefix);

    bool ClusterAnalysis(void);

    bool ClusterRefinementAnalysis(bool   Divisive,
                                   bool   PrintStepsInformation,
                                   string OutputFileNamePrefix);

    bool ClusterRefinementAnalysis(bool   Divisive,
                                   int    MinPoints,
                                   double MaxEps,
                                   double MinEps,
                                   int    Steps,
                                   bool   PrintStepsInformation,
                                   string OutputFileNamePrefix);

    bool FlushClustersInformation(string OutputClustersInfoFileName);

    bool ComputeSequenceScore(string OutputFilePrefix,
                              bool   FASTASequenceFile);

    bool ReconstructInputTrace(string OutputTraceName,
                               bool   PrintOnlyEventsOnOutputTrace,
                               bool   DoNotPrintFilteredEventsOnOutputTrace);

    bool PrintPlotScripts(string DataFileNamePrefix,
                          string ScriptsFileNamePrefix);

    bool ParametersApproximation(string              OutputFileNamePrefix,
                                 map<string, string> Parameters);

  private:
    bool GenericRefinement(bool           Divisive,
                           int            MinPoints,
                           vector<double> EpsilonPerLevel,
                           bool           PrintStepsInformation,
                           string         OutputFileNamePrefix);

    void GetTaskSet(size_t TotalTasksInTrace);

    bool GatherMPIPartition(void);

    bool GatherMaster(void);

    bool GatherSlave(void);

    bool ReconstructMasterPartition(vector<vector<vector<long> > >& GlobalLinesPerCluster);

private:

};

#endif // _LIBCLUSTERING_HPP_
