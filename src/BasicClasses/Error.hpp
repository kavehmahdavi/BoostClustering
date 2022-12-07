/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the error and warning definition */

#ifndef _ERROR_H
#define _ERROR_H

#include <string>
using std::string;

namespace cepba_tools
{
  class Error
  {
    protected:
      bool   _Error;
      string LastError;

      bool   _Warning;
      string LastWarning;

    public:
      Error(void) { _Error = false; _Warning = false; };

      bool GetError(void)        { return _Error; };
      void SetError(bool _Error) { this->_Error = _Error; };

      bool GetWarning(void)          { return _Warning; };
      void SetWarning(bool _Warning) { this->_Warning = _Warning; };

      string GetLastError(void)      { return LastError;   };
      string GetErrorMessage(void)   { return LastError;   };
      string GetLastWarning(void)    { return LastWarning; };
      string GetWarningMessage(void) { return LastWarning; };

      void SetErrorMessage(string& UserMessage, string& SysMessage);
      void SetErrorMessage(const char* UserMessage, const char* SysMessage);
      void SetErrorMessage(string& UserMessage, const char* SysMessage);
      void SetErrorMessage(const char* UserMessage, string SysMessage);
      void SetErrorMessage(string UserMessage);
      void SetErrorMessage(const char* UserMessage);

      void SetWarningMessage(string& UserMessage, string& SysMessage);
      void SetWarningMessage(const char* UserMessage, const char* SysMessage);
      void SetWarningMessage(string& UserMessage, const char* SysMessage);
      void SetWarningMessage(const char* UserMessage, string SysMessage);
      void SetWarningMessage(string UserMessage);
      void SetWarningMessage(const char* UserMessage);
  };

} /* End namespace */

/* Special prototype of 'error' function needed by R*-Tree. Implemented in
 * 'main.cpp'
extern void error (char* Message, bool Exit); */

#endif /* _ERROR_H */
