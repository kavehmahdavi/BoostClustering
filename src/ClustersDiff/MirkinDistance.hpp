#ifndef _MIRKINDISTANCE_HPP_
#define _MIRKINDISTANCE_HPP_

#include <clustering_types.h>
#include <Error.hpp>
using cepba_tools::Error;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <set>
using std::set;

class MirkinDistance: public Error
{
  string   ClustersFileName1;
  ifstream ClustersFile1;

  string   ClustersFileName2;
  ifstream ClustersFile2;

  public:

    bool GetMirkinDistance(string  ClustersFileName1,
                           string  ClustersFileName2,
                           bool&   UseNoise,
                           double& Distance);
  private:

    bool GenerateSets(map<cluster_id_t, vector<instance_t> >& ClustersSet1,
                      map<cluster_id_t, vector<instance_t> >& ClustersSet2);

    void PopulateRecord(vector<string> &Record,
                        const string   &Line,
                        char            Delimiter);

    bool CheckHeader(vector<string> &Record,
                     string         &CurrentFileName);

};



#endif
