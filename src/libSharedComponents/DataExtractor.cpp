/* Boosted density-based Kernel to apply clustering analysis*/

#include "DataExtractor.hpp"
#include <stdio.h>

DataExtractor::DataExtractor(string InputTraceName)
{
  this->InputTraceName = InputTraceName;
  
  /* Check TraceFile accesibility */
  this->InputTraceFile = fopen(this->InputTraceName.c_str(), "r");

  if (this->InputTraceFile == NULL)
  {
    string ErrorMessage;
    ErrorMessage = "unable to open trace '"+this->InputTraceName+"'";

    SetError(true);
    SetErrorMessage(ErrorMessage);

    return;
  }
}

