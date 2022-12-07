/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _DATAEXTRACTORFACTORY_HPP_
#define _DATAEXTRACTORFACTORY_HPP_

#include <trace_clustering_types.h>
#include <Error.hpp>
using cepba_tools::Error;

#include <set>
using std::set;

#include "DataExtractor.hpp"

class DataExtractorFactory: public Error
{
  private:

    input_file_t FileType;

    static DataExtractorFactory* Instance;

    set<event_type_t> EventsToDealWith;

    DataExtractorFactory(void);

    bool CheckFileType(string InputFileName);

  public:
    static DataExtractorFactory* GetInstance(void);

    bool   GetExtractor(string          InputFileName,
                        DataExtractor *&DataExtractorObject,
                        bool            EventParsing     = false,
                        bool            ConsecutiveEvts  = false,
                        bool            Distributed      = false,
                        string          InputSemanticCSV = "");

    input_file_t  GetFileType(void);


};

#endif // _DATAEXTRACTORFACTORY_HPP_
