/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the file management */

#ifndef _FILENAMEMANIPULATOR_H_
#define _FILENAMEMANIPULATOR_H_

#include <string>
#include <Error.hpp>
using cepba_tools::Error;
namespace cepba_tools
{
  class FileNameManipulator: public Error
  {

    private:
      string OriginalFileName;
      string ChoppedFileName;
      string Extension;

    public:
      FileNameManipulator(string OriginalFileName, string Extension);

      string AppendString(string Append);

      string AppendStringAndExtension(string Append, string Extension);
      
      string GetChoppedFileName(void);

      static string GetExtension(string FileName);
  };
}

#endif // _FILENAMEMANIPULATOR_H_
