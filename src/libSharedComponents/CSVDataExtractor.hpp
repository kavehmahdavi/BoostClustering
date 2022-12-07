/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef CSVDATAEXTRACTOR_H
#define CSVDATAEXTRACTOR_H

#include <trace_clustering_types.h>

#include "DataExtractor.hpp"

#include <Partition.hpp>

#include <cmath>
#include <string>

#include <fstream>
using std::ifstream;

class CSVDataExtractor: public DataExtractor
{
  private:
    string              CSVFileName;
    ifstream            CSVFile;

    ifstream::pos_type FirstPos;
    ifstream::pos_type EndPos;

    set<cluster_id_t>    IDs;
    vector<cluster_id_t> AssignmentVector;

    vector<string>       ClusteringParameters;
    vector<string>       NormalizedParameters;
    vector<string>       ExtrapolationParameters;

    vector<string>::size_type RecordSize;
    UINT32                    CurrentLine;

  public:

    CSVDataExtractor(string CSVFileName);
    ~CSVDataExtractor();

    bool SetEventsToDealWith(set<event_type_t>& EventsToDealWith,
                             bool               ConsecutiveEvts);

    bool ExtractData(TraceData* TraceDataSet);

    bool GetPartition(Partition& ReadPartition);

    input_file_t GetFileType(void) { return ClusteringCSV; };

  private:

    void PopulateRecord(vector<string> &Record,
                        const string   &Line,
                        char            Delimiter);

    bool ParseHeader(vector<string> &Record);

    bool ParseRecord(vector<string> &Record, TraceData* TraceDataSet);

};

#endif /* CSVDATAEXTRACTOR_H */
