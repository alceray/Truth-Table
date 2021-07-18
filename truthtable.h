#ifndef _TRUTH_TABLE_H_
#define _TRUTH_TABLE_H_
#include <vector>
#include <set>
#include <string>
#include <map>
using namespace std;

class TruthTable {
    unsigned int symLen;
    int valLen;
    vector<string> formulas;
    set<char> symbols;
    set<string, greater<string>> valuations;
    map<char, vector<bool>> valMap;
    map<string, vector<bool>> formMap;

    void insertValToMap(); // insert valuations into symMap
    void permuteVal(string val, int l, int r); // permuate a certain valuation
    void simplify(string &form); // removes extra '~'s and some unnecessary brackets
    string applyVal(string form, int val); // changes all symbols to a certain valuation
    int getBkt(string form); // count number of bracket pairs
    string boolToStr(bool b); // convert boolean to string
    bool strToBool(string s); // convert string of "1" or "0" to boolean
    bool solveForm(string vform); // solves formula for each valuation

    public:
        TruthTable(vector<string> formulas, set<char> symbols);
        void printValMap();
        void evaluate();
        void printFormMap(); // must call evaluate first 
        void printTruthTable(); // must call evaluate first
        
};

#endif