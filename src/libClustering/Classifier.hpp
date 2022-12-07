/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _CLASSIFIER_HPP_
#define _CLASSIFIER_HPP_

#include <clustering_types.h>
#include <Error.hpp>
using cepba_tools::Error;

#include <Point.hpp>
#include <Partition.hpp>

class Classifier: virtual public Error
{
  public:

    Classifier(void) {};

    template <typename T>
    bool Classify(T begin, T end, size_t size, Partition& DataPartition);

    virtual bool Classify(const Point* Point, cluster_id_t& ID) = 0;
};

#endif /* _CLASSIFIER_HPP_ */
