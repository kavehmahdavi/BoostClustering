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

  $Id:: ParaverApplicationDescription.cpp 23 2011#$:  Id
  $Rev:: 23                                       $:  Revision of last commit
  $Author:: jgonzale                              $:  Author of last commit
  $Date:: 2011-05-17 11:47:12 +0200 (Tue, 17 May #$:  Date of last commit

\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef PARAVERMETADATAMANAGER_
#define PARAVERMETADATAMANAGER_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <ostream>
using std::ostream;

#include <types.h>

class Metadata
{
  public:
    static int FIELD_COUNT;

  protected:
    bool   Error;
    string ErrorMessage;

    string Date;
    string Action;
    string Application;
    string OriginalTrace;

  public:
    Metadata (void) { Error = false; };

    Metadata (string Date,
              string Action,
              string Application,
              string OriginalTrace);

    string GetDate(void)          { return Date; };
    string GetAction(void)        { return Action; };
    string GetApplication(void)   { return Application; };
    string GetOriginalTrace(void) { return OriginalTrace; };

    bool   GetError(void)         { return Error; };
    string GetErrorMessage(void)  { return ErrorMessage; };

    void Write(ostream& os) const;

  private:

    virtual void FlushSpecificFields(ostream& os) const = 0;
};

ostream& operator<< (ostream& os, const Metadata& MetadataRecord);

class MetadataRecordCompare {
  public:
    bool operator()(Metadata* R1, Metadata* R2)
    {
      return (R1->GetDate().compare(R1->GetDate()) <= 0);
    }
};

class CutterMetadata: public Metadata
{
  public:
    static int    FIELD_COUNT;
    static string ACTION_ID;
    static string RUNAPP_APPLICATION_ID;
    static string ORIGINAL_APPLICATION_ID;

  private:
    UINT64 Offset;
    UINT64 BeginTime;
    UINT64 EndTime;

  public:
    CutterMetadata (vector<string>& CutterMetadataFields);

    /*
    CutterMetadata (string Date,
                    string Application,
                    string OriginalTrace,
                    UINT64 Offset,
                    UINT64 BeginTime,
                    UINT64 EndTime);
    */

    UINT64 GetOffset(void)    { return Offset; };
    UINT64 GetBeginTime(void) { return BeginTime; };
    UINT64 GetEndTime(void)   { return EndTime; };

  private:

    void FlushSpecificFields(ostream& os) const;
};

class ParaverMetadataManager
{
  private:

    bool   Error;
    string ErrorMessage;

    vector<Metadata*>       TraceMetadataStorage;
    vector<CutterMetadata*> CutterMetadataStorage;

  public:
    ParaverMetadataManager(void) : Error( false ), ErrorMessage( "" ) {};

    bool NewMetadata(string MetadataStr);

    vector<Metadata*>& GetMetadata(void);

    vector<CutterMetadata*>& GetCutterMetadata(void);

    bool   GetError       (void) { return Error; };
    string GetErrorMessage(void) { return ErrorMessage; };

    static string GetCurrentDate();

  private:

    void PopulateRecord (vector<string> &Record,
                         const string   &Line,
                         char            Delimiter);
};

#endif
