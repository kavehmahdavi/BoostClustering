/* Boosted density-based Kernel to apply clustering analysis*/

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <sstream>
using std::ostringstream;

#include <Timer.hpp>
using cepba_tools::Timer;

#include <SystemMessages.hpp>
using cepba_tools::system_messages;

#include "TDBSCANWorkerOffline.h"
#include "TDBSCANTags.h"

/**
 * Initializes the clustering library for interactive use.
 * @param libClustering Instance of the clustering library.
 */
bool TDBSCANWorkerOffline::InitLibrary (void)
{

  /* Initialize the clustering library */
  if (!libClustering->InitClustering (ClusteringDefinitionXML,
                                      Epsilon,
                                      MinPoints,
                                      (Protocol::WhoAmI() == 0), // true == Root task
                                      Protocol::WhoAmI(),
                                      Protocol::NumBackEnds())) 
  {
    ostringstream Messages;
    Messages << "Error setting up clustering library: " << libClustering->GetErrorMessage() << endl;
    system_messages::information (Messages.str(), stderr);
    return false;
  }

  cepba_tools::system_messages::verbose                 = true;
  cepba_tools::system_messages::messages_from_all_ranks = false;
  cepba_tools::system_messages::print_timers            = true;
  return true;
}


/**
 * Feeds the clustering library with points from a Paraver trace.
 * @param libClustering Instance of the clustering library.
 * @return true on success; false otherwise.
 */
bool TDBSCANWorkerOffline::ExtractData (void)
{
  ostringstream Messages;

  cepba_tools::Timer t;
  t.begin();

  if (!libClustering->ExtractData (InputTraceName) )
  {
    Messages << "Error extracting data: " << libClustering->GetErrorMessage() << endl;
    system_messages::information (Messages.str(), stderr);
    return false;
  }

  return true;
}


/**
 * Reconstructs the trace if necessary.
 * @return true on success; false otherwise.
 */
bool TDBSCANWorkerOffline::ProcessResults (Support &GlobalSupport)
{
  ostringstream Messages;
  cepba_tools::Timer t;

  if (WhoAmI() == 0)
  {
    /* Reconstruct the trace */
    if (ReconstructTrace)
    {
      system_messages::information ("RECONSTRUCTING TRACE\n");

      t.begin();

      if (!libClustering->ReconstructInputTrace (OutputFileName) )
      {
        Messages.str ("");
        Messages << "Error writing output trace: " << libClustering->GetErrorMessage() << endl;
        system_messages::information (Messages.str(), stderr);
        return false;
      }

      system_messages::show_timer ("Trace reconstruction time", t.end() );
    }
  }

  return true;
}

