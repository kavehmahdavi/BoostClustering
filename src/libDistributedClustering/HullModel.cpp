/* Boosted density-based Kernel to apply clustering analysis*/

#include "HullModel.hpp"
#include <ConvexHullModel.hpp>

HullModel::HullModel(void)
{
  _Model = NULL;
}

HullModel::HullModel(ConvexHullModel* Model)
{
  _Model = Model;
}

HullModel::HullModel(long long  Density,
                     long long  TotalTime,
                     int        NumPoints,
                     int        NumDimensions,
                     long long *Instances,
                     long long *NeighbourhoodSizes,
                     double    *DimValues)
{
  _Model = new ConvexHullModel(Density,
                               TotalTime,
                               NumPoints,
                               NumDimensions,
                               Instances,
                               NeighbourhoodSizes,
                               DimValues);
}

HullModel::~HullModel(void)
{
  if (_Model != NULL)
  {
    delete _Model;
  }
}

void HullModel::Serialize(long long  &Density,
                          long long  &TotalTime,
                          int        &NumPoints,
                          int        &NumDimensions,
                          long long *&Instances,
                          long long *&NeighbourhoodSizes,
                          double    *&DimValues )
{
  if (_Model != NULL)
  {
    _Model->Serialize(Density,
                      TotalTime,
                      NumPoints,
                      NumDimensions,
                      Instances,
                      NeighbourhoodSizes,
                      DimValues);
  }
}

HullModel* HullModel::Merge (HullModel* Other, double Epsilon, int MinPoints)
{
  ConvexHullModel *ThisModel, *OtherModel, *MergedModel;

  if (_Model == NULL)
    return NULL;

  ThisModel = _Model;

  OtherModel = Other->Model();

  if (Other->Model() == NULL)
    return NULL;

  MergedModel = ThisModel->Merge(OtherModel, Epsilon, MinPoints);

  if (MergedModel == NULL)
    return NULL;

  return new HullModel(MergedModel);
}

ConvexHullModel* const HullModel::Model(void)
{
  return _Model;
}

int  HullModel::Size(void)
{
  if (_Model == NULL)
    return -1;
  else
    return _Model->size();
}

long long HullModel::Density(void)
{
  if (_Model == NULL)
    return -1;
  else
    return _Model->GetDensity();
}

long long HullModel::TotalTime(void)
{
  if (_Model == NULL)
    return -1;
  else
    return _Model->GetTotalTime();
}

void HullModel::Flush(void)
{

  if (_Model != NULL)
  {
    _Model->Flush();
  }
}

bool HullModel::Flush(ostream&             str,
                      cluster_id_t         id)
{
  if (_Model == NULL)
  {
    return false;
  }
  else
  {
    return _Model->Flush(str, id);
  }
}

