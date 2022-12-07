/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef __CLUSTERING_TAGS_H__
#define __CLUSTERING_TAGS_H__

#include <MRNet_tags.h>

/* Define here all message tags used during the TreeDBSCAN protocol */
enum 
{
   TAG_CLUSTERING_CONFIG=FirstProtocolTag,
   TAG_HULL,
   TAG_ALL_HULLS,
   TAG_ALL_HULLS_SENT,
   TAG_NOISE,
   TAG_ALL_NOISE_SENT,
   TAG_XCHANGE_DIMENSIONS,
   TAG_STATISTICS,
   TAG_CLUSTERS_INFO,
   TAG_SUPPORT
};

#endif /* __CLUSTERING_TAGS_H__ */
