#include "truthtable.h"
#include <iostream>
using namespace std;

TruthTable::TruthTable(vector<string> formulas, set<char> symbols)
: formulas{formulas}, symbols{symbols}, formLen{formulas.size()}, symLen{symbols.size()} {
    string val;
    for (int i = 0; i < symLen; ++i) {
        val += "1";
    }
    valuations.insert(val);
    for (int i = 0; i < symLen; ++i) {
        val[i] = '0';
        permuteVal(val, 0, symLen-1);
    }
    insertValToMap();
    printMap();
}

void TruthTable::insertValToMap() {
    for (auto val : valuations) {
        int i = 0;
        for (auto sym : symbols) {
            bool bval = val[i]-'0';
            symMap[sym].push_back(bval);
            ++i;
        }
    }
}

void TruthTable::permuteVal(string val, int l, int r) {
    if (l == r) {
        valuations.insert(val);
    } else {
        for (int i = l; i <= r; ++i) {
            swap(val[l], val[i]);
            permuteVal(val, l+1, r);
            swap(val[l], val[i]);
        }
    }
}

void TruthTable::printMap() {
    int valLen = valuations.size();
    for (auto sym : symbols) {
        cout << sym << ": ";
        for (int i = 0; i < valLen; ++i) {
            cout << symMap[sym][i] << " ";
        }
        cout << endl;
    }
}