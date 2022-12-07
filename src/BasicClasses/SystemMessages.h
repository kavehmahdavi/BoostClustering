/* Boosted density-based Kernel to apply clustering analysis
 * This module includes the file massaging  */

#ifndef _SYSTEMMESSAGES_H_
#define _SYSTEMMESSAGES_H_

#include <stdio.h>

void show_percentage_progress(const char* message,
                              int         current_percentage,
                              FILE*       channel);

void show_percentage_end(const char* message,
                         FILE*       channel);


#endif // _SYSTEMMESSAGES_H_
