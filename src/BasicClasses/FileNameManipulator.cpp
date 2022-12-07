/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the file management */

#include "FileNameManipulator.hpp"
using cepba_tools::FileNameManipulator;

FileNameManipulator::FileNameManipulator(string OriginalFileName,
                                         string Extension)
{
  this->OriginalFileName = OriginalFileName;
  this->Extension        = Extension;
  
  string::size_type SubstrPosition;

  SubstrPosition = OriginalFileName.rfind("."+Extension);

  if (SubstrPosition == string::npos)
  {
    ChoppedFileName = OriginalFileName;
  }
  else
  {
    ChoppedFileName = OriginalFileName.substr(0, SubstrPosition);
  }
}

string FileNameManipulator::AppendString(string Append)
{
  return (ChoppedFileName+"."+Append+"."+Extension);
}

string FileNameManipulator::AppendStringAndExtension(string Append, string Extension)
{
  return (ChoppedFileName+"."+Append+"."+Extension);
}

string FileNameManipulator::GetChoppedFileName(void)
{
  return this->ChoppedFileName;
}

string FileNameManipulator::GetExtension(string FileName)
{
  size_t LastPointPosition;

  LastPointPosition = FileName.find_last_of(".");

  if (LastPointPosition == string::npos)
  {
    return string("");
  }
  else
  {
    return FileName.substr(LastPointPosition+1);
  }
}
