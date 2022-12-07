/*Approximate Nearest Neighbor Library (ANN). */

#include "bd_tree.h"					// bd-tree declarations
#include "kd_pr_search.h"				// kd priority search declarations

//----------------------------------------------------------------------
//	Approximate priority searching for bd-trees.
//		See the file kd_pr_search.cc for general information on the
//		approximate nearest neighbor priority search algorithm.  Here
//		we include the extensions for shrinking nodes.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//	bd_shrink::ann_search - search a shrinking node
//----------------------------------------------------------------------

void ANNbd_shrink::ann_pri_search(ANNdist box_dist)
{
	ANNdist inner_dist = 0;						// distance to inner box
	for (int i = 0; i < n_bnds; i++) {			// is query point in the box?
		if (bnds[i].out(ANNprQ)) {				// outside this bounding side?
												// add to inner distance
			inner_dist = (ANNdist) ANN_SUM(inner_dist, bnds[i].dist(ANNprQ));
		}
	}
	if (inner_dist <= box_dist) {				// if inner box is closer
		if (child[ANN_OUT] != KD_TRIVIAL)		// enqueue outer if not trivial
			ANNprBoxPQ->insert(box_dist,child[ANN_OUT]);
												// continue with inner child
		child[ANN_IN]->ann_pri_search(inner_dist);
	}
	else {										// if outer box is closer
		if (child[ANN_IN] != KD_TRIVIAL)		// enqueue inner if not trivial
			ANNprBoxPQ->insert(inner_dist,child[ANN_IN]);
												// continue with outer child
		child[ANN_OUT]->ann_pri_search(box_dist);
	}
	ANN_FLOP(3*n_bnds)							// increment floating ops
	ANN_SHR(1)									// one more shrinking node
}
