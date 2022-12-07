/* Boosted density-based Kernel to apply clustering analysis*/

#ifndef _BURSTSDB_HPP_
#define _BURSTSDB_HPP_

#include "clustering_types.h"

#include <Error.hpp>
using cepba_tools::Error;

#include <ParametersManager.hpp>
#include <CPUBurst.hpp>

#include <iostream>
using std::ostream;
#include <iterator>

#include <sqlite3.h>

class BurstsDB: public Error
{
  private:
    sqlite3           *DB;

    string             DBFileName;
    string             AllBurstsTable;
    string             CompleteBurstsView;

    size_t             FieldsCount;

    string             TableCreationQuery;
    string             BurstInsertQuery;

    sqlite3_stmt*      BurstInsertStatement;

    bool               DBActive;

    vector<string>     ParamsNames;
    vector<string>     ExtraParamsNames;

    vector<instance_t> AllBurstsInstances;

    vector<instance_t> CompleteBurstsInstances;

    bool               NormalizedBursts;

  public:
    BurstsDB(void);
    BurstsDB(ParametersManager* Parameters);

    ~BurstsDB(void);

    void CloseDB(void);

    bool BeginInserts(void);
    bool CommitInserts(void);

    bool NewBurst(CPUBurst* Burst);

    bool NormalizeBursts(const vector<double>& MaxValues,
                         const vector<double>& MinValues,
                         const vector<double>& Factors);

    size_t AllBurstsSize(void) const      { return AllBurstsInstances.size(); };
    size_t CompleteBurstsSize(void) const { return CompleteBurstsInstances.size();};

  private:

    bool CreateDB(ParametersManager* Parameters);

    vector<vector <string> > Query(string QueryStr);

    CPUBurst* GetBurst(vector<instance_t>::iterator instance);

    static string       BurstTypeStr(burst_type_t typeVal);
    static burst_type_t BurstTypeVal(string typeStr);


  public:

    class iterator
    {
      private:
        BurstsDB*                    _db;
        vector<instance_t>::iterator _cur;

      public:

        typedef iterator   self_type;
        typedef CPUBurst*  value_type;
        typedef CPUBurst*& reference;
        typedef CPUBurst** pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;

        iterator(void)        : _db(NULL) {};
        iterator(vector<instance_t>::iterator cur,
                 BurstsDB*                    db): _cur(cur), _db(db)  {};
        iterator(const self_type& rhs): _db(rhs._db), _cur(rhs._cur) {};

        self_type  operator++() { ++_cur; return (*this); };
        self_type  operator++(int junk) { self_type tmp(*this); operator++(); return tmp; };
        value_type operator*()
        {
          if (_db == NULL)
          {
            return NULL;
          }
          else
          {
            return (_db->GetBurst(_cur));
          }
        };

        // pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return _db == rhs._db && _cur == rhs._cur; };
        bool operator!=(const self_type& rhs) { return _db != rhs._db || _cur != rhs._cur; };


    };

    iterator all_bursts_begin() { iterator iter(AllBurstsInstances.begin(), this); return iter; };
    iterator all_bursts_end()   { iterator iter(AllBurstsInstances.end(), this); return iter; };

    iterator complete_bursts_begin() { iterator iter(CompleteBurstsInstances.begin(), this); return iter; };
    iterator complete_bursts_end()   { iterator iter(CompleteBurstsInstances.end(), this); return iter; };
};

#endif

