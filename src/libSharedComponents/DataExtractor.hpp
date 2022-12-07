/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _EXTRACTOR_H
#define _EXTRACTOR_H

#include "trace_clustering_types.h"

#include <Error.hpp>
using cepba_tools::Error;

#include <set>
using std::set;

#include <TraceData.hpp>

#include <Partition.hpp>

/* Forward declarations */
class DataExtractionManager;
class Partition;

class DataExtractor: public Error
{
  protected:
    string InputTraceName;
    FILE*  InputTraceFile;
    string TraceDataFileName;


  public:
    DataExtractor(string InputTraceName);
    ~DataExtractor() {};

    virtual bool   ExtractData(TraceData* DataContainer) = 0;
    virtual input_file_t GetFileType(void) = 0;

    virtual bool SetEventsToDealWith(set<event_type_t>& EventsToDealWith,
                                     bool               ConsecutiveEvts) = 0;

    virtual bool GetPartition(Partition& DataPartition) = 0;

    string GetTraceDataFileName(void) { return TraceDataFileName; };
};

#endif
