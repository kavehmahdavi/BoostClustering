/*Approximate Nearest Neighbor Library (ANN). */

#ifndef ANN_kd_pr_search_H
#define ANN_kd_pr_search_H

#include "kd_tree.h"					// kd-tree declarations
#include "kd_util.h"					// kd-tree utilities
#include "pr_queue.h"					// priority queue declarations
#include "pr_queue_k.h"					// k-element priority queue

#include <ANN/ANNperf.h>				// performance evaluation

//----------------------------------------------------------------------
//	Global variables
//		Active for the life of each call to Appx_Near_Neigh() or
//		Appx_k_Near_Neigh().
//----------------------------------------------------------------------

extern double			ANNprEps;		// the error bound
extern int				ANNprDim;		// dimension of space
extern ANNpoint			ANNprQ;			// query point
extern double			ANNprMaxErr;	// max tolerable squared error
extern ANNpointArray	ANNprPts;		// the points
extern ANNpr_queue		*ANNprBoxPQ;	// priority queue for boxes
extern ANNmin_k			*ANNprPointMK;	// set of k closest points

#endif
