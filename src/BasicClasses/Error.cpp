/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the error and warning definition */

#include "Error.hpp"
using cepba_tools::Error;

/* Errors */

void Error::SetErrorMessage(string& UserMessage, string& SysMessage)
{
  LastError = UserMessage + " (" + SysMessage + ")";
  return;
}
    
void Error::SetErrorMessage(const char* UserMessage, const char* SysMessage)
{
  string UserError = UserMessage;
  string SysError  = SysMessage;

  SetErrorMessage(UserError, SysError);
};
    
void Error::SetErrorMessage(string& UserMessage, const char* SysMessage)
{
  string SysError = SysMessage;
  SetErrorMessage(UserMessage, SysError);
}
    
void Error::SetErrorMessage(const char* UserMessage, string SysMessage)
{
  string UserError = UserMessage;
  SetErrorMessage(UserError, SysMessage);
}
    
void Error::SetErrorMessage(string UserMessage)
{
  LastError = UserMessage;
}
    
void Error::SetErrorMessage(const char* UserMessage)
{
  string UserError = UserMessage;
  LastError        = UserError;
}

/*Warnings*/

void Error::SetWarningMessage(string& UserMessage, string& SysMessage)
{
  LastWarning = UserMessage + " (" + SysMessage + ")";
  return;
}
    
void Error::SetWarningMessage(const char* UserMessage, const char* SysMessage)
{
  string UserError = UserMessage;
  string SysError  = SysMessage;

  SetWarningMessage(UserError, SysError);
};
    
void Error::SetWarningMessage(string& UserMessage, const char* SysMessage)
{
  string SysError = SysMessage;
  SetWarningMessage(UserMessage, SysError);
}
    
void Error::SetWarningMessage(const char* UserMessage, string SysMessage)
{
  string UserError = UserMessage;
  SetWarningMessage(UserError, SysMessage);
}
    
void Error::SetWarningMessage(string UserMessage)
{
  LastWarning = UserMessage;
}
    
void Error::SetWarningMessage(const char* UserMessage)
{
  string UserError = UserMessage;
  LastWarning        = UserError;
}
