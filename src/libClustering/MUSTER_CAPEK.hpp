/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _MUSTER_CAPEK_HPP_
#define _MUSTER_CAPEK_HPP_

#include "ClusteringAlgorithm.hpp"
#include "clustering_types.h"

#include <Point.hpp>

#include "Partition.hpp"

#include <par_kmedoids.h>

#include <sstream>
using std::ostringstream;

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;

#include <list>
using std::list;
using std::pair;


/* Internal points for CAPEK */
class CAPEK_Point: public Point
{
  public:
    CAPEK_Point (void) {};
    
    CAPEK_Point(Point OriginalPoint);
    /* Serialization methods required by muster */
    /// Returns the size of a packed point
    int packed_size(MPI_Comm comm) const;
  
    /// Packs a point into an MPI packed buffer
    void pack(void *buf, int bufsize, int *position, MPI_Comm comm) const;

    /// Unpacks a point from an MPI packed buffer
    static Point unpack(void *buf, int bufsize, int *position, MPI_Comm comm);
};

class MUSTER_CAPEK: public ClusteringAlgorithm
{
    typedef size_t point_idx;

  private:
    INT32 k;

  public:

    static const string NAME;
    static const string K_STRING;
    
    MUSTER_CAPEK();
    
    MUSTER_CAPEK(map < string, string > ClusteringParameters);
    ~MUSTER_CAPEK(void)
    {
    };
    
    bool Run (const vector <const Point*> &Data,
              Partition                   &DataPartition,
              bool                         SimpleRun);
    
    string GetClusteringAlgorithmName (void) const;

    string GetClusteringAlgorithmNameFile (void) const;

    bool   IsDistributed (void) { return true; };

    bool ComputeParamsApproximation (const vector < const Point* > &Data,
                                     INT32                          ParametersCount, ...);
  private:

    void ProcessClusterAssignment(cluster::par_kmedoids &muster_algorithm,
                                  Partition            &DataPartition,
                                  size_t                DataSize);
};

#endif /* _MUSTER_DBSCAN_HPP_ */
