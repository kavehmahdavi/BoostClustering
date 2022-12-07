/* Boosted density-based Kernel to apply clustering analysis*/

#include "DataExtractorFactory.hpp"
#include "PRVStatesDataExtractor.hpp"
#include "PRVEventsDataExtractor.hpp"
#include "PRVSemanticGuidedDataExtractor.hpp"
#include "TRFDataExtractor.hpp"

#include <sstream>
using std::ostringstream;

#include <cstring>
#include <cerrno>

DataExtractorFactory* DataExtractorFactory::Instance = NULL;

DataExtractorFactory::DataExtractorFactory()
{
  this->FileType = UndefinedInputFile;
}

DataExtractorFactory* DataExtractorFactory::GetInstance()
{
  if (DataExtractorFactory::Instance == NULL)
  {
    DataExtractorFactory::Instance = new DataExtractorFactory();
  }

  return (DataExtractorFactory::Instance);
}

bool DataExtractorFactory::GetExtractor(string          InputFileName,
                                        DataExtractor *&DataExtractorObject,
                                        bool            EventParsing,
                                        bool            ConsecutiveEvts,
                                        bool            MPI,
                                        string          InputSemanticCSV)
{
  if (!CheckFileType(InputFileName))
  {
    return false;
  }

#ifndef HAVE_MPI
  if (MPI)
  {
    SetError(true);
    SetErrorMessage("MPI extraction not available");
    return false;
  }
#endif

  switch(FileType)
  {
    case ParaverTrace:
      if (InputSemanticCSV.compare("") != 0)
      {
        DataExtractorObject = new PRVSemanticGuidedDataExtractor(InputFileName,
                                                                 InputSemanticCSV);
      }
      else if (EventParsing)
      {
        DataExtractorObject = new PRVEventsDataExtractor(InputFileName);
      }
      else
      {
        DataExtractorObject = new PRVStatesDataExtractor(InputFileName);
      }
      break;
    case DimemasTrace:
      DataExtractorObject = new TRFDataExtractor(InputFileName);
      break;
    case ClusteringCSV:
      SetError(true);
      SetErrorMessage("CSV input files not supported yet");
      return false;
    default:
      SetError(true);
      SetErrorMessage("undefined error while manipulating input file");
      return false;
  }

  if (DataExtractorObject->GetError())
  {
    SetError(true);
    SetErrorMessage(DataExtractorObject->GetLastError());
    return false;
  }

  return true;
}

bool DataExtractorFactory::CheckFileType(string InputTraceName)
{
  FILE* InputTraceFile;
  char  Magic[10];

  if ( (InputTraceFile = fopen(InputTraceName.c_str(), "r")) == NULL)
  {
    ostringstream ErrorMessage;

    ErrorMessage << "unable to open input file \"" << InputTraceName << "\"";
    SetErrorMessage(ErrorMessage.str());

    return false;
  }

  if (fread(Magic, sizeof(char), 8, InputTraceFile) != 8)
  {
    ostringstream ErrorMessage;

    ErrorMessage << "error reading input file (" << strerror(errno) << ")";
    SetErrorMessage(ErrorMessage.str());

    return false;
  }

  if (strncmp(Magic, "#Paraver", 8) == 0)
  {
    FileType = ParaverTrace;
    return true;
  }

  if (strncmp(Magic, "\nSDDFA;;", 8) == 0 ||
      strncmp(Magic, "SDDFA;;", 7) == 0)
  {
    FileType = DimemasTrace;
    return true;
  }

  if (strncmp(Magic, "# Instance", 10) == 0)
  {
    FileType = ClusteringCSV;
    return true;
  }

  SetErrorMessage("unable to detect input file type");
  return false;
}
