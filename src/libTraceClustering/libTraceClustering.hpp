/* Boosted density-based Kernel to apply clustering analysis*/


#ifndef _LIBTRACECLUSTERING_HPP_
#define _LIBTRACECLUSTERING_HPP_

#include "trace_clustering_types.h"

#include <string>
using std::string;

#include <map>
using std::map;

#include <set>
using std::set;

#define DO_NOTHING              0x00
#define CLUSTERING              0x01
#define PLOTS                   0x02
#define PARAMETER_APPROXIMATION 0x04
#define CLUSTERING_REFINEMENT   0x08
#define CLUSTERING_MPI_SUPPORT  0x10

#define USE_CLUSTERING(x)              (x & CLUSTERING)
#define USE_PLOTS(x)                   (x & PLOTS)
#define USE_PARAMETER_APPROXIMATION(x) (x & PARAMETER_APPROXIMATION)
#define USE_CLUSTERING_REFINEMENT(x)   (x & CLUSTERING_REFINEMENT)
#define USE_MPI(x)                     (x & CLUSTERING_MPI_SUPPORT)


class libTraceClusteringImplementation;

class libTraceClustering
{
  private:
    libTraceClusteringImplementation* Implementation;

    bool   Error,        Warning;
    string ErrorMessage, WarningMessage;

  public:


    libTraceClustering(bool verbose,
                       bool paraver_verbosity = false);

    bool InitTraceClustering(string        ClusteringDefinitionXML,
                             string        PCFFileName,
                             bool          UseSemanticValue,
                             bool          ApplyLogToSemanticValue,
                             unsigned char Flags);

    bool SetDBSCANParameters(double Eps, int MinPoints);

    bool ExtractData(string            InputFileName,
                     bool              SampleData      = false,
                     unsigned int      MaxSamples      = 0,
                     set<unsigned int> EventsToParse   = set<unsigned int> (),
                     bool              ConsecutiveEvts = false,
                     string            InputSemanticCSV = "");

    bool ExtractData(string            InputFileName,
                     bool              SampleData       = false,
                     unsigned int      MaxSamples       = 0,
                     string            InputSemanticCSV = "");

    bool ExtractData(string            InputFileName,
                     set<unsigned int> EventsToParse,
                     bool              ConsecutiveEvts,
                     string            InputSemanticCSV = "");

    bool ExtractData(string            InputFileName,
                     string            OutputCSVFileName,
                     bool              SampleData      = false,
                     unsigned int      MaxSamples      = 0,
                     set<unsigned int> EventsToParse   = set<unsigned int> (),
                     bool              ConsecutiveEvts = false,
                     string            InputSemanticCSV = "");

    bool ClusterAnalysis (void);

    bool ClusterRefinementAnalysis(bool   Divisive,
                                   bool   PrintStepsInformation,
                                   string OutputFileNamePrefix = "");

    bool ClusterRefinementAnalysis(bool   Divisive,
                                   int    MinPoints,
                                   double MaxEps,
                                   double MinEps,
                                   int    Steps,
                                   bool   PrintStepsInformation,
                                   string OutputFileNamePrefix = "");

    bool FlushClustersInformation(string OutputClustersInfoFileName);

    bool FlushData(string OutputCSVFileNamePrefix);

    bool ComputeSequenceScore(string OutputFilePrefix,
                              bool   FASTASequenceFile);

    bool ReconstructInputTrace(string OutputTraceName,
                               bool   PrintOnlyEventsOnOutputTrace,
                               bool   DoNotPrintFilteredEventsOnOutputTrace);

    bool PrintPlotScripts(string DataFileNamePrefix,
                          string ScriptsFileNamePrefix = "");

    bool ParametersApproximation(string              OutputFileNamePrefix,
                                 map<string, string> Parameters);

    bool   GetError(void) { return Error; };
    string GetErrorMessage(void);

    bool   GetWarning(void) { return Warning; };
    string GetWarningMessage(void);

  protected:

};

#endif // _LIBCLUSTERING_HPP_
