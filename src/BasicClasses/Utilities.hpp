/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the utilities  */

#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include <clustering_suite_config.h>

#include <cstdio>
#include <string>
#include <set>

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif


namespace cepba_tools
{
  std::string GetCWD(void);

  std::string &ltrim(std::string &s);

  std::string &rtrim(std::string &s);

  std::string &trim(std::string &s);

/* Adapted from Extrae 'utils.c' */

  unsigned long long getTimeFromStr (char *time, char *envvar);

  int is_Alphabetic(char c);

  void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);
}



#endif // _UTILITIES_HPP_
