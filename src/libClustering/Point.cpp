/* Boosted density-based Kernel to apply clustering analysis*/

#include "Point.hpp"

#include <cmath>
#include <cassert>
#include <limits>

#include <iostream>
using std::cout;
using std::endl;

instance_t Point::InstanceNumber = 0;

#if defined (HAVE_MUSTER) && defined (HAVE_MPI)
size_t Point::PointDimensions = 0;
#endif

Point::Point(size_t Dimensions)
{
  this->Instance   = (Point::InstanceNumber++);
  this->Dimensions = vector<double> (Dimensions, 0.0);

#if defined (HAVE_MUSTER) && defined (HAVE_MPI)
  if (Point::PointDimensions == 0)
  {
    Point::PointDimensions = Dimensions;
  }
#endif
}

Point::Point(vector<double>& _Dimensions)
{
  Dimensions = _Dimensions;
  Normalized = false;

  this->Instance   = (Point::InstanceNumber++);

#if defined (HAVE_MUSTER) && defined (HAVE_MPI)
  if (Point::PointDimensions == 0)
  {
    Point::PointDimensions = _Dimensions.size();
  }
#endif

  /* DEBUG
  cout << "New point! Dimensions size = " << _Dimensions.size() << endl;
  */
}

Point::Point(instance_t _Instance, vector<double>& _Dimensions)
{
  Instance   = _Instance;
  Dimensions = _Dimensions;
  Normalized = true;

#if defined (HAVE_MUSTER) && defined (HAVE_MPI)
  if (Point::PointDimensions == 0)
  {
    Point::PointDimensions = _Dimensions.size();
  }
#endif
}

vector<double>& Point::GetDimensions(void)
{
  return Dimensions;
}

void
Point::RangeNormalization(const vector<double>& MaxValues,
                          const vector<double>& MinValues,
                          const vector<double>& Factors)
{
  double BaseValue;

  /* DEBUG
  cout << "Original Dimensions = {";
  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    cout << Dimensions[i] << " ";
  }
  cout << "}" << endl;
  */



  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    BaseValue = Dimensions[i];

    Dimensions[i] = Factors[i]*((BaseValue - MinValues[i]) / (MaxValues[i] - MinValues[i]));
  }

  Normalized = true;

  /* DEBUG
  cout << "Resulting Dimensions = {";
  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    cout << Dimensions[i] << " ";
  }
  cout << "}" << endl;
  */
}

double Point::EuclideanDistance(const Point& OtherPoint) const
{
  double Result = std::numeric_limits<double>::max();

  if (Dimensions.size() != OtherPoint.size())
  {
    return Result;
  }
  else
  {
    Result = 0.0;

    for (size_t i = 0; i < Dimensions.size(); i++)
    {

      Result += pow(Dimensions[i] - OtherPoint.Dimensions[i], 2.0);
    }

    return sqrt(Result);
  }
}

size_t Point::size(void) const
{
  return Dimensions.size();
}

void Point::clear(void)
{
  Dimensions.clear();
  Normalized = false;
}

double& Point::operator[](int i)
{
  assert(i >= 0 && i < Dimensions.size());

  return Dimensions[i];
}

double Point::operator[](int i) const
{
  assert(i >= 0 && i < Dimensions.size());

  return Dimensions[i];
}

Point Point::operator +  (const Point& other)
{
  Point Result((*this).size());

  if ((*this).size() != other.size())
  {
    return Point(0);
  }

  for (size_t i = 0; i < (*this).size(); i++)
  {
    Result[i] = (*this)[i] + other[i];
  }

  return Result;
}

Point Point::operator /  (const size_t scalar)
{
  Point Result((*this).size());

  for (size_t i = 0; i < (*this).size(); i++)
  {
    Result[i] = (*this)[i] / scalar;
  }

  return Result;
}

bool Point::operator != (const Point& other) const
{
  if (this->Normalized != other.IsNormalized())
    return true;

  if (this->size() != other.size())
    return true;

  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    if (this[i] != other[i])
    {
      return true;
    }
  }

  return false;
}

Point& Point::operator =  (const Point& other)
{
  if ((*this) != other)
  {
    Dimensions.clear();

    for (size_t i = 0; i < other.size(); i++)
    {
      Dimensions.push_back(other[i]);
    }
  }

  return *this;
}

void Point::PrintPoint(void)
{
  for (size_t i = 0; i < Dimensions.size(); i++)
  {
    cout << " [" << i << "] = " << Dimensions[i];
  }
  cout << endl;
}
