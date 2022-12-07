/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLUSTERING_TYPES_H_
#define _CLUSTERING_TYPES_H_

#include <types.h>

#include <cstdlib>
#include <cstddef>
#include <string>
using std::string;

#include <limits>
using std::numeric_limits;

typedef enum { UndefinedAlgorithm,
               DBSCAN_Algorithm,
               GMEANS_Algorithm,
               MUSTER_DBSCAN_Algorithm,
               MUSTER_PAM_Algorithm,
               MUSTER_XCLARA_Algorithm} clustering_algorithm_t;

typedef INT32 cluster_id_t;

typedef double percentage_t;

typedef UINT64 instance_t;

static const string UNDEFINED = "Undefined";
static const INT32  NOT_FOUND = -1;

static const double MAX_DOUBLE = numeric_limits<double>::max();
static const double MIN_DOUBLE = -1.0*MAX_DOUBLE;

static const cluster_id_t UNCLASSIFIED    = -5; /* Just to uniform generate GNUplots when extracting data */
static const cluster_id_t NOISE_CLUSTERID = 0;
static const cluster_id_t MIN_CLUSTERID   = NOISE_CLUSTERID+1;

#endif
