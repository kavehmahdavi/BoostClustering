/* Boosted density-based Kernel to apply clustering analysis*/

#include <iostream>
#include <fstream>
using std::ofstream;
using std::cerr;
using std::endl;

#include <sstream>
using std::ostringstream;

#include <SystemMessages.hpp>
using cepba_tools::system_messages;

#include "TDBSCANWorkerOnline.h"
#include "TDBSCANTags.h"
#include "ClustersInfo.h"
#include "Utils.h"

#include "ClustersInfo.h"

/**
 * Constructor sets-up a callback to extract the clustering data from the tracing runtime.
 */
TDBSCANWorkerOnline::TDBSCANWorkerOnline(FetchCallback DataExtractCallback, FeedCallback DataFeedCallback)
{
    this->DataExtractCallback = DataExtractCallback;
    this->DataFeedCallback = DataFeedCallback;
}


/**
 * Initializes the clustering library for on-line use.
 * @param libClustering Instance of the clustering library.
 * @return true on success; false otherwise.
 */
bool TDBSCANWorkerOnline::InitLibrary(void)
{
  if (!libClustering->InitClustering (ClusteringDefinitionXML,
/*
                                      Epsilon,
                                      MinPoints,
*/
                                      (Protocol::WhoAmI() == 0), // true == Root task
                                      Protocol::WhoAmI(),
                                      Protocol::NumBackEnds()))
  {
    cerr << "[BE " << WhoAmI() << "] Error setting up clustering library: " << libClustering->GetErrorMessage() << endl;
    return false;
  }
  cepba_tools::system_messages::verbose                 = true;
  cepba_tools::system_messages::messages_from_all_ranks = false;
  cepba_tools::system_messages::print_timers            = true;
  return true;
}


/**
 * Feeds the clustering library with points from the on-line tracing buffers.
 * @param libClustering Instance of the clustering library.
 * @return true on success; false otherwise.
 */
bool TDBSCANWorkerOnline::ExtractData(void)
{
  DataExtractCallback(libClustering);

  return true;
}


/**
 * Send the clusters information to the tracing library
 * @return true on success; false otherwise.
 */
bool TDBSCANWorkerOnline::ProcessResults(Support &GlobalSupport)
{
  ostringstream Messages;

  vector<Point *>      Points;
  vector<timestamp_t>  BeginTimes;
  vector<timestamp_t>  EndTimes;
  vector<cluster_id_t> ClusterIDs;

  libClustering->GetFullBurstsInformation(Points, BeginTimes, EndTimes, ClusterIDs);

  vector<int> BurstsSupport;
  GlobalSupport.GetSupport( BurstsSupport );

  DataFeedCallback( BeginTimes, EndTimes, ClusterIDs, BurstsSupport );

  return true;
}

