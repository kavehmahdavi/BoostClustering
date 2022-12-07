/*Approximate Nearest Neighbor Library (ANN). */

#ifndef ANN_kd_fix_rad_search_H
#define ANN_kd_fix_rad_search_H

#include "kd_tree.h"					// kd-tree declarations
#include "kd_util.h"					// kd-tree utilities
#include "pr_queue_k.h"					// k-element priority queue

#include <ANN/ANNperf.h>				// performance evaluation

//----------------------------------------------------------------------
//	Global variables
//		These are active for the life of each call to
//		annRangeSearch().  They are set to save the number of
//		variables that need to be passed among the various search
//		procedures.
//----------------------------------------------------------------------

extern ANNpoint			ANNkdFRQ;			// query point (static copy)

#endif
