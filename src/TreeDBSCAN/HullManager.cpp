/* Boosted density-based Kernel to apply clustering analysis*/

#include "HullManager.h"
#include "TDBSCANTags.h"
#include "Utils.h"

#include <cstring>

const char *HullFormatString     = "%ld %ld %d %d %ald %ald %alf";
const char *AllHullsFormatString = "%ald %ald %ad %ad %ald %ald %alf";
const char *AllSentFormatString  = "";

HullManager::HullManager(void) { }

/**
 * Front-end, filter and back-end call to receive a hull from a MRNet packet.
 */
HullModel* HullManager::Unpack(PACKET_PTR InputPacket)
{
  long long  Density=0, TotalTime=0;
  int        NumberOfPoints=0, NumberOfDimensions=0, DimensionsValuesSize=0;
  long long *Instances=NULL, *NeighbourhoodSizes=NULL;
  double    *DimensionsValues=NULL;

  PACKET_unpack(InputPacket, HullFormatString,
    &Density, &TotalTime,
    &NumberOfPoints,
    &NumberOfDimensions,
    &Instances, &NumberOfPoints,
    &NeighbourhoodSizes, &NumberOfPoints,
    &DimensionsValues, &DimensionsValuesSize);

  HullModel *Hull = new HullModel(Density,
                                  TotalTime,
                                  NumberOfPoints,
                                  NumberOfDimensions,
                                  Instances,
                                  NeighbourhoodSizes,
                                  DimensionsValues);

  /* DEBUG -- dump current hull
  std::cout << "[DUMP HULL] << std::endl;
  Hull->Print();
  */

  xfree(Instances);
  xfree(NeighbourhoodSizes);
  xfree(DimensionsValues);

  return Hull;
}


void HullManager::Unpack(PACKET_PTR InputPacket, vector<HullModel *> &HullsList)
{
  int        NumberOfHulls=0;
  long long *DensityArray=NULL, *TotalTimeArray=NULL;
  int       *NumberOfPointsArray=NULL, *NumberOfDimensionsArray=NULL;
  long long *InstancesArray=NULL, *NeighbourhoodSizesArray=NULL;
  double    *DimensionsValuesArray=NULL;
  int        TotalNumberOfPoints=0, TotalDimensionsValues=0;


  PACKET_unpack(InputPacket, AllHullsFormatString,
    &DensityArray, &NumberOfHulls,
    &TotalTimeArray, &NumberOfHulls,
    &NumberOfPointsArray, &NumberOfHulls,
    &NumberOfDimensionsArray, &NumberOfHulls,
    &InstancesArray, &TotalNumberOfPoints,
    &NeighbourhoodSizesArray, &TotalNumberOfPoints,
    &DimensionsValuesArray, &TotalDimensionsValues);

  int idx1=0, idx2=0;

  for (int i=0; i<NumberOfHulls; i++)
  {
    HullModel *Hull = new HullModel(DensityArray[i],
                                    TotalTimeArray[i],
                                    NumberOfPointsArray[i],
                                    NumberOfDimensionsArray[i],
                                    &InstancesArray[idx1],
                                    &NeighbourhoodSizesArray[idx1],
                                    &DimensionsValuesArray[idx2]);

    HullsList.push_back( Hull );

    idx1 += NumberOfPointsArray[i];
    idx2 += NumberOfPointsArray[i] * NumberOfDimensionsArray[i];
  }
}


/**************************************************************************\
 *                           BACK-END INTERFACE                           *
\**************************************************************************/

/**
 * Back-end call to send a list of hulls through the given stream.
 * @param OutputStream The MRNet stream.
 * @param HullsList A vector of hulls to send.
 */
void HullManager::Serialize(STREAM *OutputStream, vector<HullModel*> &HullsList)
{
  for (size_t i = 0; i < HullsList.size(); i++)
  {
    SerializeOne(OutputStream, HullsList[i]);
  }

  SerializeDone(OutputStream);
}

void HullManager::SerializeAll(STREAM *OutputStream, vector<HullModel*> &HullsList)
{
  int        NumberOfHulls           = 0;
  long long *DensityArray            = NULL;
  long long *TotalTimeArray          = NULL;
  int       *NumberOfPointsArray     = NULL;
  int       *NumberOfDimensionsArray = NULL;
  int        TotalNumberOfPoints     = 0;
  long long *InstancesArray          = NULL;
  long long *NeighbourhoodSizesArray = NULL;
  int        TotalDimensionsValues   = 0;
  double    *DimensionsValuesArray   = NULL;

  SerializeAll(HullsList,
               NumberOfHulls,
               DensityArray,
               TotalTimeArray,
               NumberOfPointsArray,
               NumberOfDimensionsArray,
               TotalNumberOfPoints,
               InstancesArray,
               NeighbourhoodSizesArray,
               TotalDimensionsValues,
               DimensionsValuesArray);

  STREAM_send(OutputStream, TAG_ALL_HULLS, AllHullsFormatString,
    DensityArray,            NumberOfHulls,
    TotalTimeArray,          NumberOfHulls,
    NumberOfPointsArray,     NumberOfHulls,
    NumberOfDimensionsArray, NumberOfHulls,
    InstancesArray,          TotalNumberOfPoints,
    NeighbourhoodSizesArray, TotalNumberOfPoints,
    DimensionsValuesArray,   TotalDimensionsValues);

  xfree(DensityArray);
  xfree(TotalTimeArray);
  xfree(NumberOfPointsArray);
  xfree(NumberOfDimensionsArray);
  xfree(InstancesArray);
  xfree(NeighbourhoodSizesArray);
  xfree(DimensionsValuesArray);

  SerializeDone(OutputStream);
}


/**
 * Back-end call to send a single hull through the given stream.
 * @param OutputStream The MRNet stream.
 * @param Hull The hull to send.
 */
void HullManager::SerializeOne(STREAM *OutputStream, HullModel *Hull)
{
  long long  Density=0, TotalTime=0;
  int        NumberOfPoints=0, NumberOfDimensions=0;
  long long *Instances=NULL, *NeighbourhoodSizes=NULL;
  double    *DimensionsValues=NULL;

  /* Serialize the hull object into basic data types that can be sent through the MRNet */
  Hull->Serialize(Density,
                  TotalTime,
                  NumberOfPoints,
                  NumberOfDimensions,
                  Instances,
                  NeighbourhoodSizes,
                  DimensionsValues);

  STREAM_send(OutputStream, TAG_HULL, HullFormatString,
    Density,
    TotalTime,
    NumberOfPoints,
    NumberOfDimensions,
    Instances, NumberOfPoints,
    NeighbourhoodSizes, NumberOfPoints,
    DimensionsValues, NumberOfPoints * NumberOfDimensions);

  xfree(Instances);
  xfree(NeighbourhoodSizes);
  xfree(DimensionsValues);
}

/**
 * Back-end call to notify that all hulls were sent.
 * @param OutputStream The MRNet stream.
 */
void HullManager::SerializeDone(STREAM *OutputStream)
{
  STREAM_send(OutputStream, TAG_ALL_HULLS_SENT, AllSentFormatString);
}

/**************************************************************************\
 *                            FILTER INTERFACE                            *
\**************************************************************************/

/**
 * Filter call to prepare a list of output packets to send the given hulls.
 * @param StreamID The stream ID where the packet will be sent.
 * @param OutputPackets Queue of output packets that has to be filled (by reference).
 * @param HullsList A vector of hulls to send.
 */
void HullManager::Serialize(int StreamID, vector<PacketPtr> &OutputPackets, vector<HullModel*> &HullsList)
{
  vector<HullModel*>::iterator it;

  for (it = HullsList.begin(); it != HullsList.end(); ++it)
  {
    HullModel *Hull = *it;

    SerializeOne(StreamID, OutputPackets, Hull);
  }
  SerializeDone(StreamID, OutputPackets);
}

void HullManager::SerializeAll(int StreamID, vector<PacketPtr> &OutputPackets, vector<HullModel*> &HullsList)
{
  int        NumberOfHulls           = 0;
  long long *DensityArray            = NULL;
  long long *TotalTimeArray     = NULL;
  int       *NumberOfPointsArray     = NULL;
  int       *NumberOfDimensionsArray = NULL;
  int        TotalNumberOfPoints     = 0;
  long long *InstancesArray          = NULL;
  long long *NeighbourhoodSizesArray = NULL;
  int        TotalDimensionsValues   = 0;
  double    *DimensionsValuesArray   = NULL;

  SerializeAll(HullsList,
               NumberOfHulls,
               DensityArray,
               TotalTimeArray,
               NumberOfPointsArray,
               NumberOfDimensionsArray,
               TotalNumberOfPoints,
               InstancesArray,
               NeighbourhoodSizesArray,
               TotalDimensionsValues,
               DimensionsValuesArray);

  PacketPtr new_packet( new Packet( StreamID, TAG_ALL_HULLS, AllHullsFormatString,
                                    DensityArray,            NumberOfHulls,
                                    TotalTimeArray,          NumberOfHulls,
                                    NumberOfPointsArray,     NumberOfHulls,
                                    NumberOfDimensionsArray, NumberOfHulls,
                                    InstancesArray,          TotalNumberOfPoints,
                                    NeighbourhoodSizesArray, TotalNumberOfPoints,
                                    DimensionsValuesArray,   TotalDimensionsValues ) );

  new_packet->set_DestroyData(true);
  OutputPackets.push_back( new_packet );

  SerializeDone(StreamID, OutputPackets);
}

/**
 * Filter call to prepare an output packet to send the given hull.
 * @param StreamID The stream ID where the packet will be sent.
 * @param OutputPackets Queue of output packets that has to be filled (by reference).
 * @param Hull The hull to send.
 */
void HullManager::SerializeOne(int StreamID, vector<PacketPtr> &OutputPackets, HullModel *Hull)
{
  long long  Density=0, TotalTime=0;
  int        NumberOfPoints=0, NumberOfDimensions=0;
  long long *Instances=NULL, *NeighbourhoodSizes=NULL;
  double    *DimensionsValues=NULL;

  /* Serialize the hull object into basic data types that can be sent through the MRNet */
  Hull->Serialize(Density,
                  TotalTime,
                  NumberOfPoints,
                  NumberOfDimensions,
                  Instances,
                  NeighbourhoodSizes,
                  DimensionsValues);

  PacketPtr new_packet( new Packet( StreamID, TAG_HULL, HullFormatString,
                                    Density, TotalTime,
                                    NumberOfPoints,
                                    NumberOfDimensions,
                                    Instances, NumberOfPoints,
                                    NeighbourhoodSizes, NumberOfPoints,
                                    DimensionsValues, NumberOfPoints * NumberOfDimensions ) );

  new_packet->set_DestroyData(true);
  OutputPackets.push_back( new_packet );
}

/**
 * Filter call to prepare an output packet to send that notifies that all hulls were sent.
 * @param StreamID The stream ID where the packet will be sent.
 * @param OutputPackets Queue of output packets that has to be filled (by reference).
 */
void HullManager::SerializeDone(int StreamID, vector<PacketPtr> &OutputPackets)
{
  /* Notify all hulls were sent */
  PacketPtr new_packet( new Packet( StreamID, TAG_ALL_HULLS_SENT, AllSentFormatString ) );
  OutputPackets.push_back( new_packet );
}

void HullManager::SerializeAll(
  vector<HullModel*> &HullsList,
  int                &NumberOfHulls,
  long long         *&DensityArray,
  long long         *&TotalTimeArray,
  int               *&NumberOfPointsArray,
  int               *&NumberOfDimensionsArray,
  int                &TotalNumberOfPoints,
  long long         *&InstancesArray,
  long long         *&NeighbourhoodSizesArray,
  int                &TotalDimensionsValues,
  double            *&DimensionsValuesArray)
{
  NumberOfHulls = HullsList.size();

  DensityArray            = (long long *)malloc(NumberOfHulls * sizeof(long long));
  TotalTimeArray          = (long long *)malloc(NumberOfHulls * sizeof(long long));
  NumberOfPointsArray     =       (int *)malloc(NumberOfHulls * sizeof(int));
  NumberOfDimensionsArray =       (int *)malloc(NumberOfHulls * sizeof(int));

  for (unsigned int i=0; i<NumberOfHulls; i++)
  {
    long long  Density=0;
    long long  TotalTime=0;
    int        NumberOfPoints=0;
    int        NumberOfDimensions=0;
    long long *Instances=NULL;
    long long *NeighbourhoodSizes=NULL;
    double    *DimensionsValues=NULL;
    int        idx=0;

    HullsList[i]->Serialize(Density,
                            TotalTime,
                            NumberOfPoints,
                            NumberOfDimensions,
                            Instances,
                            NeighbourhoodSizes,
                            DimensionsValues);

    DensityArray[i]            = Density;
    TotalTimeArray[i]          = TotalTime;
    NumberOfPointsArray[i]     = NumberOfPoints;
    NumberOfDimensionsArray[i] = NumberOfDimensions;

    TotalNumberOfPoints   += NumberOfPoints;
    TotalDimensionsValues += NumberOfPoints * NumberOfDimensions;

    InstancesArray          = (long long *)realloc(InstancesArray,          TotalNumberOfPoints * sizeof(long long));
    NeighbourhoodSizesArray = (long long *)realloc(NeighbourhoodSizesArray, TotalNumberOfPoints * sizeof(long long));
    DimensionsValuesArray   =    (double *)realloc(DimensionsValuesArray,   TotalDimensionsValues * sizeof(double));

    idx = TotalNumberOfPoints - NumberOfPoints;

    memcpy(&InstancesArray[idx],
           Instances,
           NumberOfPoints * sizeof(long long));

    memcpy(&NeighbourhoodSizesArray[idx],
           NeighbourhoodSizes,
           NumberOfPoints * sizeof(long long));

    idx = TotalDimensionsValues - (NumberOfPoints * NumberOfDimensions);

    memcpy(&DimensionsValuesArray[idx],
           DimensionsValues,
           NumberOfPoints * NumberOfDimensions * sizeof(double));

    xfree(Instances);
    xfree(NeighbourhoodSizes);
    xfree(DimensionsValues);
  }
}

