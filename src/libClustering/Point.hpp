/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <vector>
using std::vector;

#include "clustering_types.h"

class Point
{
  public:


  protected:
    instance_t     Instance;
    vector<double> Dimensions;
    bool           Normalized;
    size_t         NeighbourhoodSize;

  public:
    static instance_t InstanceNumber;

#if defined (HAVE_MUSTER) && defined (HAVE_MPI)
    static size_t PointDimensions;
#endif

    Point() {};

    Point(size_t Dimensions);

    Point(vector<double>& _Dimensions);

    Point(instance_t _Instance, vector<double>& _Dimensions);

    instance_t GetInstance(void) const { return this->Instance; }

    vector<double>& GetDimensions(void);

    void       SetNeighbourhoodSize(size_t NeighbourhoodSize) { this->NeighbourhoodSize = NeighbourhoodSize; }
    size_t     GetNeighbourhoodSize(void) const               { return this->NeighbourhoodSize; }

    void   RangeNormalization(const vector<double>& MaxValues,
                              const vector<double>& MinValues,
                              const vector<double>& Factors);

    void   ScaleDimensions(const vector<double>& Factors);

    double EuclideanDistance(const Point& OtherPoint) const;

    double NormalizedEuclideanDistance(Point& OtherPoint) const;

    bool   IsNormalized(void) const { return Normalized; };
    void   SetNormalized(bool Normalized) { this->Normalized = Normalized; };

    void   clear(void);

    size_t size(void) const;

    double& operator [] (int i);
    double  operator [] (int i) const;
    Point   operator +  (const Point& other);
    Point   operator /  (const size_t scalar);
    bool    operator != (const Point& other) const;
    Point&  operator =  (const Point& other);

    void check_const(void) { Normalized = true;};

    void PrintPoint(void);

  private:

};

/*
Point* operator+ (Point* left, Point* right)
{
  Point* result;

  if (left == NULL && right == NULL)
  {
    return NULL;
  }
  else if (left == NULL && right != NULL)
  {
    result = new Point(right->size());

    for (size_t i = 0; i < right->size(); i++)
    {
      Point->[i] = right->[i];
    }

    return result;
  }
  else if (left != NULL && right == NULL)
  {
    result = new Point(left->size());

    for (size_t i = 0; i < left->size(); i++)
    {
      Point->[i] = left->[i];
    }

    return result;
  }

  if (left->size() != right->size())
  {
    return NULL;
  }

  Point* result = new Point(left->size());

  for (size_t i = 0; i < left->size(); i++)
  {
    result->[i] = left->[i] + right->[i];
  }

  return result
}
*/

/* Distance functor to use with 'muster' library */
struct PointEuclideanDistance
{
  double operator()(const Point* left, const Point* right) const
  {
      return left->EuclideanDistance((*right));
  }

  /*
  double operator()(const Point left, const Point right) const
  {
    return left.EuclideanDistance (right);
  }
  */

  double operator()(const Point& left, const Point& right) const
  {
    return left.EuclideanDistance (right);
  }
};


#endif // _POINT_HPP_
