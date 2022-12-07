/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _HULLMODEL_HPP_
#define _HULLMODEL_HPP_

#include "clustering_types.h"
#include <iostream>
using std::ostream;

class ConvexHullModel;

class HullModel
{
  private:

    ConvexHullModel* _Model;

  public:

    HullModel(void);

    HullModel(ConvexHullModel* Model);

    HullModel(long long  Density,
              long long  TotalTime,
              int        NumPoints,
              int        NumDimensions,
              long long *Instances,
              long long *NeighbourhoodSizes,
              double    *DimValues);

    ~HullModel(void);

    void Serialize(long long  &Density,
                   long long  &TotalTime,
                   int        &NumPoints,
                   int        &NumDimensions,
                   long long *&Instances,
                   long long *&NeighbourhoodSizes,
                   double    *&DimValues );

    HullModel* Merge (HullModel* Other, double Epsilon, int MinPoints);

    ConvexHullModel* const Model(void);

    long long Density(void);
    long long TotalTime();

    int  Size(void);

    void Flush(void);

    bool Flush(ostream&     str,
               cluster_id_t id);
};

struct SortHullsByTime
{
    bool operator()(HullModel * const & a, HullModel * const & b) const
    {
        return a->TotalTime() > b->TotalTime();
    }
};

#endif

