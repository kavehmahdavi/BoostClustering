/* Boosted density-based Kernel to apply clustering analysis*/

#include <string.h>

#include "ClusteredTraceGenerator.hpp"

ClusteredTraceGenerator::ClusteredTraceGenerator(string  InputTraceName,
                                                 string  OutputTraceName)
{
  this->InputTraceName = InputTraceName;
  if ((InputTraceFile = fopen(InputTraceName.c_str(), "r")) == NULL)
  {
    SetError(true);
    SetErrorMessage("error opening input trace", strerror(errno));
    return;
  }

  this->OutputTraceName = OutputTraceName;
  if ((OutputTraceFile = fopen(OutputTraceName.c_str(), "w")) == NULL)
  {
    SetError(true);
    SetErrorMessage("error opening output trace", strerror(errno));
    return;
  }
}

ClusteredTraceGenerator::~ClusteredTraceGenerator(void)
{
  if (InputTraceFile != NULL)
    fclose(InputTraceFile);

  if (OutputTraceFile != NULL)
    fclose(OutputTraceFile);
}

