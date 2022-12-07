/* Boosted density-based Kernel to apply clustering analysis*/

#include <BackEnd.h>
#include "TDBSCANWorkerOffline.h"

#if defined(BACKEND_ATTACH)

#include <mpi.h>

int main(int argc, char *argv[])
{
   int rank;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   BackEnd *BE = new BackEnd();
   BE->Init(rank);

   BackProtocol *protClustering = new TDBSCANWorkerOffline();
   BE->LoadProtocol( protClustering );

   BE->Loop();

   MPI_Finalize();
   return 0;
}

#else

/**
 * The back-end application loads the TDBSCAN protocol and waits for 
 * the front-end to start the analysis.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return 0 on success; -1 otherwise.
 */ 
int main(int argc, char *argv[])
{
   BackEnd *BE = new BackEnd();
   BE->Init(argc, argv);

   BackProtocol *protClustering = new TDBSCANWorkerOffline();
   BE->LoadProtocol( protClustering );

   BE->Loop();

   return 0;
}

#endif

