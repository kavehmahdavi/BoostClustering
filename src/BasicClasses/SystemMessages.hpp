/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the file massaging  */

#ifndef _SYSTEMMESSAGES_HPP_
#define _SYSTEMMESSAGES_HPP_

#include <clustering_suite_config.h>

#include <Timer.hpp>

#include <cstdio>
#include <string>
using std::string;

namespace cepba_tools
{
  class system_messages
  {
    public:

      static bool verbose;

      static bool paraver_verbosity;

      static bool distributed;

      static int   my_rank;
      static bool  messages_from_all_ranks;
      static char *rank_prefix;

      static bool percentage_ongoing;
      static bool progress_ongoing;

      static bool print_timers;

      static int  last_percentage_written;

      static void set_rank_prefix(const char* desired_rank_prefix);

      static void information(string message,
                              FILE*  channel = stdout);

      static void information(const char* message,
                              FILE*       channel = stdout);

      static void error(string message);

      static void error(const char* message);

      static void silent_information(string message,
                                     FILE  *channel = stdout);

      static void silent_information(const char* message,
                                     FILE*       channel = stdout);

      static void die(const char* message,
                      FILE*       channel = stderr);

      static void show_progress(const char* message,
                                int         current,
                                int         total,
                                FILE*       channel = stdout);

      static void show_progress(string message,
                                int    current,
                                int    total,
                                FILE*  channel = stdout);

      static void show_progress_end(const char* message,
                                    int         total,
                                    FILE*       channel = stdout);

      static void show_progress_end(string message,
                                    int    total,
                                    FILE*  channel = stdout);

      static void show_percentage_progress(const char* message,
                                           int         current_percentage,
                                           FILE*       channel = stdout);

      static void show_percentage_progress(string message,
                                           int    current_percentage,
                                           FILE*  channel = stdout);

      static void show_percentage_end(string message,
                                      FILE*  channel = stdout);

      static void show_percentage_end(const char* message,
                                      FILE*       channel = stdout);

      static void show_percentage_progress_error(string message,
                                                 FILE*  channel = stdout);

      static void show_percentage_progress_error(const char* message,
                                                 FILE*  channel = stdout);

      static void show_timer(string           message,
                             Timer::diff_type Time,
                             FILE*            channel = stdout);

      static void show_timer(const char*      message,
                             Timer::diff_type Time,
                             FILE*            channel = stdout);
  };
}

#endif // _SYSTEMMESSAGES_HPP_
