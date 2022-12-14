/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                             ClusteringSuite                               *
 *   Infrastructure and tools to apply clustering analysis to Paraver and    *
 *                              Dimemas traces                               *
 *                                                                           *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\

  $Id::                                           $:  Id
  $Rev::                                          $:  Revision of last commit
  $Author::                                       $:  Author of last commit
  $Date::                                         $:  Date of last commit

\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <SystemMessages.hpp>
using cepba_tools::system_messages;

#include "PRVEventsDataExtractor.hpp"
#include "ParaverTraceParser.hpp"

#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <sstream>
using std::ostringstream;

#define DEBUG_PARAVER_INPUT 1

PRVEventsDataExtractor::PRVEventsDataExtractor(string InputTraceName)
:DataExtractor(InputTraceName)
{
  if (GetError())
    return;

  TraceParser = new ParaverTraceParser(InputTraceName, InputTraceFile);

  if (!TraceParser->InitTraceParsing())
  {
    SetError(true);
    SetErrorMessage("error init tracing", TraceParser->GetLastError());
    return;
  }

  if (TraceParser->GetTimeUnits() == MICROSECONDS)
    TimeFactor = 1e3;
  else
    TimeFactor = 1;

  return;
}


PRVEventsDataExtractor::~PRVEventsDataExtractor()
{
  unlink(TraceDataFileName.c_str());
}

bool PRVEventsDataExtractor::SetEventsToDealWith (set<event_type_t>& EventsToDealWith,
                                                  bool               ConsecutiveEvts)
{
  this->EventsToDealWith = EventsToDealWith;
  this->ConsecutiveEvts  = ConsecutiveEvts;


  if (EventsToDealWith.size() == 0)
  {
    SetErrorMessage("no events definend in a PRV event parsing extractor");
    SetError(true);

    return false;
  }

  return true;
}

bool PRVEventsDataExtractor::ExtractData(TraceData* TraceDataSet)
{
  vector<ApplicationDescription_t> AppsDescription;
  vector<TaskDescription_t>        TaskInfo;
  size_t                           TraceObjects = 0;

  ParaverRecord *CurrentRecord;
  State         *CurrentState;
  Event         *CurrentEvent;

  percentage_t  CurrentPercentage = 0;

  if (EventsToDealWith.size() == 0)
  {
    SetError(true);
    SetErrorMessage("event types not set in that event based Paraver parser");
    return false;
  }

  /* Create the structure to manage the different bursts foreach task/thread */
  AppsDescription = TraceParser->GetApplicationsDescription();

  if (TraceParser->GetError())
  {
    SetError(true);
    SetErrorMessage("unable to start data extraction",
                    TraceParser->GetLastError());
    return false;
  }

  if (AppsDescription.size() != 1)
  {
    SetError(true);
    SetErrorMessage("unable to clusterize a trace with more than one application");
    return false;
  }

  TaskInfo = AppsDescription[0]->GetTaskInfo();

  /* Set the number of tasks on trace data, to perform the possible data distribution */
  TraceDataSet->SetNumberOfTasks(TaskInfo.size());

  for (size_t i = 0; i < TaskInfo.size(); i++)
  {
    TraceObjects += TaskInfo[i]->GetThreadCount();
    TaskData.push_back(vector<TaskDataContainer>(TaskInfo[i]->GetThreadCount()));
    FutureTaskData.push_back(vector<TaskDataContainer>(TaskInfo[i]->GetThreadCount()));
    EventsStack.push_back(vector<stack<event_type_t> > (TaskInfo[i]->GetThreadCount()));
  }

  TraceDataSet->SetTraceObjects(TraceObjects);

  for (size_t i = 0; i < TaskData.size(); i++)
  {
    for (size_t j = 0; j < TaskData[i].size(); j++)
    {
      /*
      TaskData[i][j].TaskId   = i;
      TaskData[i][j].ThreadId = j;
      */
      TaskData[i][j].Clear();

      /*
      FutureTaskData[i][j].TaskId   = i;
      FutureTaskData[i][j].ThreadId = j;
      */

      FutureTaskData[i][j].Clear();
    }
  }

  CurrentPercentage = TraceParser->GetFilePercentage();

  system_messages::show_percentage_progress("Parsing Paraver Input Trace",
                                            CurrentPercentage);

  while (true)
  {
    INT32 PercentageRead;

    CurrentRecord = TraceParser->GetNextRecord(EVENT_REC);

    if (CurrentRecord == NULL)
      break;

    if (!CheckEvent((Event*) CurrentRecord, TraceDataSet))
    {
      return false;
    }

    /* Show progress */
    PercentageRead = TraceParser->GetFilePercentage();
    if (PercentageRead > CurrentPercentage)
    {
      CurrentPercentage = PercentageRead;
      system_messages::show_percentage_progress("Parsing Paraver Input Trace", CurrentPercentage);
    }

    /* Free memory used by the parser */
    delete CurrentRecord;
  }

  if (TraceParser->GetError())
  {
    SetError(true);
    SetErrorMessage("error parsing trace ", TraceParser->GetLastError());
    return false;
  }

  system_messages::show_percentage_end("Parsing Paraver Input Trace");

  if (ferror(InputTraceFile) != 0)
  {
    SetError(true);
    SetErrorMessage("error reading input trace file", strerror(errno));
    return false;
  }
  else
  { /* Check if there is any pending burst */
    for (size_t i = 0; i < TaskData.size(); i++)
    {
      for (size_t j = 0; j < TaskData[i].size(); j++)
      {
        if (TaskData[i][j].OngoingBurst)
        { /* There is an ongoing burst.  */
          if (!TraceDataSet->NewBurst(TaskData[i][j].TaskId,
                                      TaskData[i][j].ThreadId,
                                      TaskData[i][j].Line,
                                      TaskData[i][j].BeginTime,
                                      TaskData[i][j].EndTime,
                                      TaskData[i][j].BurstDuration,
                                      TaskData[i][j].EventsData,
                                      TaskData[i][j].BurstEndEvents)) /* DEBUG (2013/10/23): To be changed to correct the parsing */
          {
            SetError(true);
            SetErrorMessage("error storing burst data",
                            TraceDataSet->GetLastError());
            return false;
          }
        }
      }
    }
  }

  if (!TraceDataSet->DataExtractionFinished())
  {
    SetError(true);
    SetErrorMessage(TraceDataSet->GetLastError());
    return false;
  }

  /* No more burst
  if (!TraceDataSet->NoMoreBursts())
  {
    SetError(true);
    SetErrorMessage("error finishing data adcquisition");
    return false;
  }
  */

#ifdef DEBUG_PARAVER_INPUT
  // cout << "Data Size = " << TraceDataSet->GetDataSetSize() << endl;
#endif

  if (TraceDataSet->GetClusteringBurstsSize() == 0)
  {
    SetError(true);
    SetErrorMessage("No bursts extracted, cluster analysis cannot proceed");
    return false;
  }
  else
  {
    ostringstream Message;
    Message << "Points to analyse " << TraceDataSet->GetClusteringBurstsSize() << endl;
    system_messages::silent_information(Message.str());
  }

  return true;
}

bool PRVEventsDataExtractor::CheckEvent(Event     *CurrentEvent,
                                        TraceData *TraceDataSet)
{
  TaskDataContainer& CurrentTaskData =
    TaskData[CurrentEvent->GetTaskId()][CurrentEvent->GetThreadId()];

  TaskDataContainer& NextTaskData =
    FutureTaskData[CurrentEvent->GetTaskId()][CurrentEvent->GetThreadId()];

  for (size_t i = 0; i < CurrentEvent->GetTypeValueCount(); i++)
  {
    event_type_t  CurrentType  = CurrentEvent->GetType(i);
    event_value_t CurrentValue = CurrentEvent->GetValue(i);

    if (BurstOpeningEvent(CurrentType, CurrentValue))
    {
      // cout << "Opening event Time = " << CurrentEvent->GetTimestamp() << endl;
      if (CurrentTaskData.OngoingBurst)
      {
        // cout << "Evt Time = " << CurrentEvent->GetTimestamp() << endl;
        if (CurrentTaskData.EndTime != 0)
        {
          // cout << "EndTime fixed!" << endl;
          if (CurrentEvent->GetTimestamp() > CurrentTaskData.EndTime)
          { /* Burst pending to be closed */
            // cout << "Generating Burst 1 Begin = " << CurrentTaskData.BeginTime << " End = " << CurrentTaskData.EndTime << endl;
            if (!GenerateBurst(TraceDataSet, CurrentTaskData))
            {
              return false;
            }

            /* New Burst opens */
            if (ConsecutiveEvts && NextTaskData.OngoingBurst)
            {
              CurrentTaskData = NextTaskData;
              CurrentTaskData.EndTime = CurrentEvent->GetTimestamp();
              NextTaskData.Clear();
              FillDataContainer (NextTaskData, CurrentEvent);
            }
            else
            {
              FillDataContainer (CurrentTaskData, CurrentEvent);
            }
          }
          else if (CurrentEvent->GetTimestamp() == CurrentTaskData.EndTime)
          { /* This is a 'future burst' */
            // cout << "Filling NextTaskData 1 Time = " << CurrentEvent->GetTimestamp() << endl;
            FillDataContainer (NextTaskData, CurrentEvent);
          }
        }
        else
        { /* Set the previous burst as finished, generate the future container
           * and push an element in the stack */
          // cout << "Filling NextTaskData 2 Time = " << CurrentEvent->GetTimestamp() << endl;

          // cout << "EndTime not fixed!" << endl;

          if (ConsecutiveEvts)
          {
            CurrentTaskData.EndTime = CurrentEvent->GetTimestamp();
          }

          FillDataContainer(NextTaskData, CurrentEvent);
        }
      }
      else
      {
        EventsStack[CurrentEvent->GetTaskId()][CurrentEvent->GetThreadId()].push(CurrentType);
        FillDataContainer(CurrentTaskData, CurrentEvent);
      }
    }
    else if (BurstClosingEvent(CurrentType, CurrentValue))
    {
      if (!CurrentTaskData.OngoingBurst)
      { /* Should never happen! But happens... */
        /* Should only happen in stacked events!
        ostringstream ErrorMessage;

        ErrorMessage << "closing stacked region (" << CurrentEvent->GetTimestamp();
        ErrorMessage << ") not implemented yet";

        SetError(true);
        SetErrorMessage(ErrorMessage.str());
        return false;
        */
      }
      else
      { /* Set the end time of the current burst */
        CurrentTaskData.EndTime = CurrentEvent->GetTimestamp();
      }
    }
    else
    {
      if (CurrentTaskData.OngoingBurst)
      {
        if (CurrentTaskData.EndTime != 0)
        {
          if (CurrentEvent->GetTimestamp() > CurrentTaskData.EndTime)
          { /* Add the information to the 'future burst', if it exists */
            if (NextTaskData.OngoingBurst)
            {
              UpdateTaskData (NextTaskData,
                              CurrentType,
                              CurrentValue,
                              CurrentEvent->GetTimestamp());
            }

            // cout << "Generating Burst 2 Begin = " << CurrentTaskData.BeginTime << " End = " << CurrentTaskData.EndTime << endl;
            /* Create the burst */
            if (!GenerateBurst(TraceDataSet, CurrentTaskData))
            {
              return false;
            }

            /* Move to next task data */
            CurrentTaskData = NextTaskData;
            NextTaskData.Clear();
          }
          else
          {
            UpdateTaskData (CurrentTaskData,
                            CurrentType,
                            CurrentValue,
                            CurrentEvent->GetTimestamp());
          }
        }
        else
        { /* The end of the current burst hasn't bet set, add the information */
          if (CurrentTaskData.BeginTime != CurrentEvent->GetTimestamp())
          {
            UpdateTaskData (CurrentTaskData,
                            CurrentType,
                            CurrentValue,
                            CurrentEvent->GetTimestamp());
          }
        }
      }
    }
  }

  return true;
}

void PRVEventsDataExtractor::FillDataContainer(TaskDataContainer &TaskData,
                                               Event             *CurrentEvent)
{
  TaskData.TaskId        = CurrentEvent->GetTaskId();
  TaskData.ThreadId      = CurrentEvent->GetThreadId();
  TaskData.Line          = CurrentEvent->GetLine();
  TaskData.OngoingBurst  = true;
  TaskData.BeginTime     = CurrentEvent->GetTimestamp();
}

bool PRVEventsDataExtractor::GenerateBurst(TraceData*         TraceDataSet,
                                           TaskDataContainer& Data)
{
  /* Set the burst  duration */
  Data.BurstDuration = Data.EndTime - Data.BeginTime;

  /* DEBUG
  cout << "Calling 'GenerateBurst'. Duration = " << Data.BurstDuration << endl; */

  set<event_type_t>::iterator it;

  for (it  = Data.NotCommonEvents.begin();
       it != Data.NotCommonEvents.end();
       ++it)
  {
    Data.EventsData.erase((*it));
  }


  /* Add it to the Trace Data Set */
  if (!TraceDataSet->NewBurst(Data.TaskId,
                              Data.ThreadId,
                              Data.Line,
                              Data.BeginTime,
                              Data.EndTime,
                              Data.BurstDuration,
                              Data.EventsData,
                              Data.BurstEndEvents)) /* DEBUG (2013/10/23): To be changed to correct the parsing */
  {

    SetError(true);
    SetErrorMessage("error storing burst data",
                    TraceDataSet->GetLastError());
    return false;
  }

  Data.Clear();

  return true;
}

/* This two methods consider the classic semantic of entering a of value != 0
   means entering a function/region, and value = means the end of
   function/region */

bool PRVEventsDataExtractor::BurstOpeningEvent(event_type_t  EventType,
                                               event_value_t EventValue)
{
  if (EventsToDealWith.count(EventType) == 1)
  {
    if (ConsecutiveEvts)
    {
      return true;
    }
    else
    {
      if (EventValue != 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  else
  {
    return false;
  }
}

bool PRVEventsDataExtractor::BurstClosingEvent(event_type_t  EventType,
                                               event_value_t EventValue)
{
  if (EventsToDealWith.count(EventType) == 1  )
  {
    if (ConsecutiveEvts)
    {
      return true;
    }
    else
    {
      if (EventValue == 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  else
  {
    return false;
  }
}

bool PRVEventsDataExtractor::UpdateTaskData(TaskDataContainer& DataContainer,
                                            event_type_t       EventType,
                                            event_value_t      EventValue,
                                            timestamp_t        EventTime)
{
   map<event_type_t, event_value_t>::iterator EventsDataIterator;

  EventsDataIterator = DataContainer.EventsData.find(EventType);

  /* Check if there is a HWC change. */
  if (EventType == HWC_GROUP_CHANGE_TYPE)
  {
    if (EventTime > DataContainer.BeginTime &&
        (DataContainer.EndTime == 0 ||
        (DataContainer.EndTime != 0 && EventTime < DataContainer.EndTime)))
    {
      DataContainer.IntermediateHWChange = true;
    }
  }

  if (EventsDataIterator == DataContainer.EventsData.end())
  { /* Add this event to 'EventsData' map */
    if (DataContainer.IntermediateHWChange)
    { /* As the HWC group changed, this is a type of event that never
         appear previously, so it is not common to all the burst so
         it won't be taken in consideration */
      DataContainer.NotCommonEvents.insert(EventType);
    }
    else
    { /* As the HWC did not change, we can store the event and value
       * normally */
      DataContainer.EventsData[EventType] = EventValue;

      if (DataContainer.EndTime != 0 && EventTime == DataContainer.EndTime)
      { /* Annotate that current burst has events at his end time */
        DataContainer.BurstEndEvents.insert(EventType);
      }
    }
  }
  else
  { /* This event was already stored in the map. If necessary, sum values */
    DataContainer.EventsData[EventType] += EventValue;

    if (EventTime == DataContainer.EndTime)
    { /* Annotate that current burst has events at his end time */
      DataContainer.BurstEndEvents.insert(EventType);
    }
  }

  return true;
}
