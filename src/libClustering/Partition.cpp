/* Boosted density-based Kernel to apply clustering analysis*/

#include "Partition.hpp"

#include <iostream>
using std::cout;
using std::endl;

/**
 * Default constructor
 */
Partition::Partition(void)
{
  _ClusterAssignmentVector = vector<cluster_id_t> (0);
}

/**
 * Clear the contents of the partition
 */
void Partition::clear(void)
{
  _ClusterAssignmentVector.clear();
  _IDs.clear();
}

/**
 * Returns the assignment vector
 *
 * \return The assignment vector
 */
vector<cluster_id_t>& Partition::GetAssignmentVector(void)
{
  return _ClusterAssignmentVector;
}

/**
 * Sets the assignment vector
 *
 * \param The new assignment vector
 */
void Partition::SetAssignmentVector(vector<cluster_id_t>& ClusterAssignmentVector)
{
  _ClusterAssignmentVector = ClusterAssignmentVector;
}

/**
 * Returns the set containing the IDs used
 *
 * \return A set containing the IDs used in the partition
 */
set<cluster_id_t>& Partition::GetIDs(void)
{
  return _IDs;
}

/**
 * Sets the set of used IDs
 *
 * \param IDs A set containing the IDs used in the partition
 */
void Partition::SetIDs(set<cluster_id_t>& IDs)
{
  _IDs = IDs;
}

/**
 * Returns the value of the maximum cluster id used in this partition
 *
 * \return Maximum cluster ID used in this partition
 */
cluster_id_t Partition::GetMaxID(void)
{
  cluster_id_t                Result = NOISE_CLUSTERID;
  set<cluster_id_t>::iterator IDsIterator;

  for (IDsIterator  = _IDs.begin();
       IDsIterator != _IDs.end();
       ++IDsIterator)
  {
    if ((*IDsIterator) > Result)
    {
      Result = (*IDsIterator);
    }
  }

  return Result;
}

/**
 * Changes all IDs of the OriginalIDs set to the ID DifinitiveID
 *
 * \param OriginalIDs  Set of IDs to be merged
 * \param DefinitiveID Resulting ID
 *
 */
void Partition::MergeIDs(set<cluster_id_t>& OriginalIDs,
                         cluster_id_t       DefinitiveID)
{
  for (size_t i = 0; i < _ClusterAssignmentVector.size(); i++)
  {
    cluster_id_t CurrentID = _ClusterAssignmentVector[i];

    if (OriginalIDs.count(CurrentID) != 0)
    {
      _ClusterAssignmentVector[i] = DefinitiveID;
      // cout << "Merging" << endl;
    }
  }

  set<cluster_id_t>::iterator OriginalIDsIt;

  for (OriginalIDsIt  = OriginalIDs.begin();
       OriginalIDsIt != OriginalIDs.end();
       ++OriginalIDsIt)
  {
    _IDs.erase((*OriginalIDsIt));
  }

  return;
}

/**
 * Updates the set of different IDs present on the partition. Needed after
 * translation
 *
 */
void Partition::UpdateIDs(void)
{
  if (_ClusterAssignmentVector.size() == 0)
  {
    return;
  }

  _IDs.clear();
  for (vector<cluster_id_t>::size_type i = 0;
                                       i < _ClusterAssignmentVector.size();
                                       i++)
  {
    _IDs.insert(_ClusterAssignmentVector[i]);
  }

  return;
}

/**
 * Return the number of clusters discovered
 *
 * \return The number of clusters discovered
 */
size_t Partition::NumberOfClusters(void) const
{
  return _IDs.size();
}

/**
 * Return if the current partition has a noise cluster
 *
 * \return True if the current partition has a noise cluster
 */
bool Partition::HasNoise(void) const
{
  if (_IDs.count(NOISE_CLUSTERID) > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}



