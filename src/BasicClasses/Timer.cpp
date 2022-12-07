/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the timer implementation  */

#include "Timer.hpp"

#include <iostream>
using std::cout;
using std::endl;

using cepba_tools::Timer;

Timer::Timer()
{
  begin();
}

void Timer::begin()
{
  gettimeofday(&time_start, 0);

  elapsed = 0;
}

Timer::diff_type Timer::end()
{
  struct timeval time_end;
  struct timeval time_elapsed;

  gettimeofday(&time_end, 0);

  timersub(&time_end, &time_start, &time_elapsed);

  elapsed = time_elapsed.tv_sec * 1000000 + time_elapsed.tv_usec;

  return elapsed;
}
/*
cepba_tools::Timer::diff_type cepba_tools::Timer::end()
{
  elapsed = (diff_type) std::clock() - start;
  elapsed /= CLOCKS_PER_SEC;
  return elapsed;
}
*/
