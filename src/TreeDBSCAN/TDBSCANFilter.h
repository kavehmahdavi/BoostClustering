/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __TDBSCAN_FILTER_H__
#define __TDBSCAN_FILTER_H__

#include <mrnet/NetworkTopology.h>
#include <HullModel.hpp>

#include <vector>
using std::vector;

using namespace MRN;


#define TOP_FILTER(top_info)    (top_info.get_NumSiblings() == 0)
#define BOTTOM_FILTER(top_info) (top_info.get_NumChildren() == 0)
#define FILTER_ID(top_info)     (top_info.get_Rank())

extern "C" {

void Init(const TopologyLocalInfo &top_info);

/*
void MergeAlltoAll(vector<HullModel*> &ClustersHulls,
                   vector<HullModel*> &MergedModel,
                   double              Epsilon,
                   int                 MinPoints);
*/

void NewMerge(HullModel *ChildHull, double Epsilon, int MinPoints);

}

#endif /* __TDBSCAN_FILTER_H__ */
