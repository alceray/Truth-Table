#ifndef _TRUTH_TABLE_H_
#define _TRUTH_TABLE_H_
#include <vector>
#include <set>
#include <string>
#include <map>
using namespace std;

class TruthTable {
    unsigned int formLen;
    unsigned int symLen;
    vector<string> formulas;
    set<char> symbols;
    set<string, greater<string>> valuations;
    map<char, vector<bool>> symMap;
    void insertValToMap(); // insert valuations into symMap
    void permuteVal(string val, int l, int r); // permuate a certain valuation
    void printMap();
    public:
        TruthTable(vector<string> formulas, set<char> symbols);
};

#endif