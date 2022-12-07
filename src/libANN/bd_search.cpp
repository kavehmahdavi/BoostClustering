/*Approximate Nearest Neighbor Library (ANN). */

#include "bd_tree.h"					// bd-tree declarations
#include "kd_search.h"					// kd-tree search declarations

//----------------------------------------------------------------------
//	Approximate searching for bd-trees.
//		See the file kd_search.cpp for general information on the
//		approximate nearest neighbor search algorithm.  Here we
//		include the extensions for shrinking nodes.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//	bd_shrink::ann_search - search a shrinking node
//----------------------------------------------------------------------

void ANNbd_shrink::ann_search(ANNdist box_dist)
{
												// check dist calc term cond.
	if (ANNmaxPtsVisited != 0 && ANNptsVisited > ANNmaxPtsVisited) return;

	ANNdist inner_dist = 0;						// distance to inner box
	for (int i = 0; i < n_bnds; i++) {			// is query point in the box?
		if (bnds[i].out(ANNkdQ)) {				// outside this bounding side?
												// add to inner distance
			inner_dist = (ANNdist) ANN_SUM(inner_dist, bnds[i].dist(ANNkdQ));
		}
	}
	if (inner_dist <= box_dist) {				// if inner box is closer
		child[ANN_IN]->ann_search(inner_dist);	// search inner child first
		child[ANN_OUT]->ann_search(box_dist);	// ...then outer child
	}
	else {										// if outer box is closer
		child[ANN_OUT]->ann_search(box_dist);	// search outer child first
		child[ANN_IN]->ann_search(inner_dist);	// ...then outer child
	}
	ANN_FLOP(3*n_bnds)							// increment floating ops
	ANN_SHR(1)									// one more shrinking node
}
