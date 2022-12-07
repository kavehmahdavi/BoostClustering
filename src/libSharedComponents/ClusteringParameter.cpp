/* Boosted density-based Kernel to apply clustering analysis*/

#include "ClusteringParameter.hpp"

using std::endl;

#include <limits>
using std::numeric_limits;

#include <math.h>
#include <float.h>

#include <iostream>
using std::cout;
using std::endl;

/*****************************************************************************
 * class ClusteringParameter (superclass)
 ****************************************************************************/
ClusteringParameter::ClusteringParameter(string ParameterName,
                                         double Factor,
                                         bool   ApplyLog,
                                         double RangeMin,
                                         double RangeMax)
{
  this->ParameterName = ParameterName;
  this->Factor        = Factor;
  this->ApplyLog      = ApplyLog;
  this->Ready         = false;
  this->Used          = false;
  this->RangeMin      = RangeMin;
  this->RangeMax      = RangeMax;
}

ostream& operator<< (ostream& os, ClusteringParameter& ClusteringParameterObj)
{
  return ClusteringParameterObj.Write(os);
}

/*****************************************************************************
 * class SingleEvent
 ****************************************************************************/

SingleEvent::SingleEvent(string        ParameterName,
                         double        Factor,
                         bool          ApplyLog,
                         event_type_t  EventType,
                         double        RangeMin,
                         double        RangeMax)
:ClusteringParameter(ParameterName, Factor, ApplyLog, RangeMin, RangeMax)
{
  this->EventType     = EventType;
  this->EventValue    = 0;
}

void
SingleEvent::Clear(void)
{
  this->SetReady(false);
  this->EventValue = 0;
}

void
SingleEvent::NewData(event_type_t EventType, event_value_t EventValue)
{
  /* JGG 20100525 -> To fixe overflow counters */
  // if (Ready)
  //  return;

  if (this->EventType == EventType)
  {
    // fix broken counters, PM_CMPLU_STALL_FDIV and PM_CMPLU_STALL_ERAT_MISS
    if (this->EventType == 42001226 || this->EventType == 42001219 )
    {
      event_value_t bogus1, bogus2;

      bogus1 = EventValue << 32; // shift left 32 bits (to clear out upper 32 bits)
      bogus2 = bogus1 >> 32; // shift right 32 bits (to restore the lower 32 bits)

      /* DEBUG
      cout << "Cleaning event. Type = " << this->EventType;
      cout << ". Original Value = " << EventValue; */

      if (bogus2 >> 30 >= 1)
      { // if this is an overflow value, flip all bits
        this->EventValue = bogus2 ^ (4294967295UL); // do an XOR with 11111111111111111111111111111111  (32 1's)
      }
      else
      {
        this->EventValue = bogus2;
     }

     /* DEBUG
     cout << " - Clean Value = " << this->EventValue << endl; */
    }
    else
    {
      this->EventValue += EventValue;
    }
  }
  else
    return;

  this->SetReady(true);
}

/* DEBUG */
#include <iostream>
using std::cout;
using std::endl;

bool
SingleEvent::IsRangeFiltered(void)
{
  if (Ready == true)
  {
    if (RangeMin != -1.0 && EventValue < RangeMin)
      return true;

    if (RangeMax != -1.0 && EventValue > RangeMax)
      return true;

    return false;
  }
  else
    return false;
}

double
SingleEvent::GetRawMetric(void)
{
  if (!Ready)
    return -1.0;

  return (1.0*EventValue);
}

double
SingleEvent::GetMetric(void)
{
  if (!ApplyLog)
    return GetRawMetric();
  else
  {
    if (EventValue == 0.0)
      return numeric_limits<double>::min();
    else
      return log(GetRawMetric());
  }
}

ostream&
SingleEvent::Write(ostream& os)
{
  os << "Single Event:  " << ParameterName << endl;
  os << "Event Type:    " << EventType << endl;
  os << "Factor:        " << Factor << endl;

  os << "Apply Log:     " << ApplyLog << endl;
  if (Ready)
    os << "Current Value: " << EventValue << endl;
  else
    os << "No value" << endl;

  return os;
}

/*****************************************************************************
 * class MixedEvents
 ****************************************************************************/

MixedEvents::MixedEvents(string              ParameterName,
                         double              Factor,
                         bool                ApplyLog,
                         event_type_t        EventTypeA,
                         event_type_t        EventTypeB,
                         derived_event_op_t  Operation,
                         double              RangeMin,
                         double              RangeMax)
:ClusteringParameter(ParameterName, Factor, ApplyLog, RangeMin, RangeMax)
{
  this->EventTypeA = EventTypeA;
  this->EventTypeB = EventTypeB;
  this->Operation  = Operation;
  EventARead = EventBRead = false;
}

void
MixedEvents::NewData(event_type_t EventType, event_value_t EventValue)
{
  if (EventType == EventTypeA && !EventARead)
  {
    EventValueA += EventValue;
    EventARead   = true;
  }
  else if (EventType == EventTypeB && !EventBRead)
  {
    EventValueB += EventValue;
    EventBRead   = true;
  }
  else
    return;

  if (EventARead && EventBRead)
  {

    this->SetReady(true);
  }
}

bool
MixedEvents::IsRangeFiltered(void)
{
  if (Ready)
  {
    if (RangeMin != -1.0 && GetRawMetric() < RangeMin)
      return true;

    if (RangeMax != -1.0 && GetRawMetric() > RangeMax)
      return true;

    return false;
  }
  else
    return false;
}

void
MixedEvents::SetReady(bool Ready)
{
  if (Ready)
    this->Ready = Ready;
  else
  {
    this->Ready      = false;
    this->EventARead = false;
    this->EventBRead = false;
  }
}

void
MixedEvents::Clear(void)
{
  this->SetReady(false);
  this->EventValueA = 0;
  this->EventValueB = 0;
}

double MixedEvents::GetRawMetric(void)
{
  if (!Ready)
    return -1.0;

  switch (Operation)
  {
    case Add:
      Result = (1.0*EventValueA) + (1.0*EventValueB);
      break;
    case Substract:
      Result = (1.0*EventValueA) - (1.0*EventValueB);
      break;
    case Multiply:
      Result = (1.0*EventValueA) * (1.0*EventValueB);
      break;
    case Divide:
      if (EventValueB != 0)
        Result = (1.0*EventValueA) / (1.0*EventValueB);
      else
        Result = 0.0;
      break;
    default: /* This branch must be unreacheable */
      Result = -1.0;
      break;
  }

  return Result;
}

double
MixedEvents::GetMetric(void)
{

  if (!Ready)
    return -1.0;

  if (!ApplyLog)
    return 1.0*GetRawMetric();
  else
  {
    if (Result == 0.0)
      return numeric_limits<double>::min();
    else
      return log(GetRawMetric());
  }
}

ostream&
MixedEvents::Write(ostream& os)
{
  os << "Mixed Events:  " << ParameterName << endl;
  os << "Event Type A:  " << EventTypeA << endl;
  os << "Event Type B:  " << EventTypeB << endl;
  os << "Factor:        " << Factor << endl;
  os << "Operation:     " << this->GetOperation() << endl;

  os << "Apply Log:     " << ApplyLog << endl;
  if (Ready)
  {
    os << "Event Value A: " << EventValueA << endl;
    os << "Event Value B: " << EventValueB << endl;
  }
  else
    os << "No values" << endl;

  return os;
}
