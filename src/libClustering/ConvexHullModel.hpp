/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CONVEX_HULL_MODEL_HPP_
#define _CONVEX_HULL_MODEL_HPP_

#include <vector>
using std::vector;
#include <iostream>
using std::ostream;
using std::endl;

#include <Point.hpp>

#include <CGAL/basic.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/convex_hull_2.h>

#include "CGAL_Kernel/MyKernel.h"

/*
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
typedef  CGAL::Lazy_exact_nt< CGAL::Quotient<CGAL::MP_Float> > ET; */

/* Try using this data type in case it fails  */
#include <CGAL/Gmpq.h>
typedef CGAL::Lazy_exact_nt<CGAL::Gmpq> ET;


typedef MyKernel<ET>                      K;
//typedef CGAL::Filtered_kernel_adaptor<MK> K;
typedef K::Point_2                        MyPoint_2;
typedef CGAL::Polygon_2<K>                Polygon_2;

class ConvexHullModel
{
  protected:

    int               Dimensions;
    long long         Density;
    long long         TotalTime;
    vector<MyPoint_2> HullPoints;

  public:

    static int MIN_HULL_POINTS;

    ConvexHullModel(void);

    ConvexHullModel ( vector< const Point* >, long long TotalTime );

    ConvexHullModel(vector<MyPoint_2> HullPoints, long long Density, long long TotalTime);
    /* ConvexHullModel ( Polygon_2 P, Polygon_2 Q ); */
    ConvexHullModel(long long  Density,
                    long long  TotalTime,
                    int        NumPoints,
                    int        NumDimensions,
                    long long *Instances,
                    long long *NeighbourhoodSizes,
                    double    *DimValues);

    int size(void);
    long long GetDensity(void);
    long long GetTotalTime(void);

    void Serialize(long long  &Density,
                   long long  &TotalTime,
                   int        &NumPoints,
                   int        &NumDimensions,
                   long long *&Instances,
                   long long *&NeighbourhoodSizes,
                   double    *&DimValues );
    void Flush( );
    ConvexHullModel * Merge( ConvexHullModel * CHull2, double Epsilon = 0, int MinPoints = 1);

    vector<MyPoint_2>& getHullPoints();

    bool IsInside(const Point* QueryPoint);
    bool IsInside(const MyPoint_2& QueryPoint);
    bool IsNear(const Point* QueryPoint, double Epsilon, int MinPoints);
    bool IsNear (ConvexHullModel *Hull2, double Epsilon, int MinPoints);

    void GetDistanceAndDensity(const Point* QueryPoint, double &SqDistance, int&Density);

    bool    Flush(ostream&             str,
                  cluster_id_t         id);

    string  GetPlotLine(string DataFileName, cluster_id_t ID);

  private:

    void   Assemble(long long  Density,
                    long long  TotalTime,
                    int        NumPoints,
                    int        NumDimensions,
                    long long *Instances,
                    long long *NeighbourhoodSizes,
                    double    *DimValues );

};

#endif // _CONVEX_HULL_MODEL_HPP_
