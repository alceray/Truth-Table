#include "truthtable.h"
#include <iostream>
using namespace std;

TruthTable::TruthTable(vector<string> formulas, set<char> symbols)
: formulas{formulas}, symbols{symbols}, symLen{symbols.size()} {
    string val;
    for (int i = 0; i < symLen; ++i) {
        val += "1";
    }
    valuations.insert(val);
    for (int i = 0; i < symLen-1; ++i) {
        val[i] = '0';
        permuteVal(val, 0, symLen-1);
    }
    val[symLen-1] = '0';
    valuations.insert(val);
    insertValToMap();
    // printValMap();
}

void TruthTable::insertValToMap() {
    valLen = valuations.size();
    for (auto val : valuations) {
        int i = 0;
        for (auto sym : symbols) {
            bool bval = val[i]-'0';
            valMap[sym].push_back(bval);
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

void TruthTable::printValMap() {
    for (auto sym : symbols) {
        for (int i = 0; i < valLen; ++i) {
            cout << valMap[sym][i] << " ";
        }
        cout << ": " << sym << endl;
    }
}

void TruthTable::printFormMap() {
    for (auto form : formulas) {
        for (int i = 0; i < valLen; ++i) {
            cout << formMap[form][i] << " ";
        }
        cout << ": " << form << endl;
    }
}

void TruthTable::printTruthTable() {
    printValMap();
    for (int i = 0; i < 2*valLen; ++i) {
        cout << "=";
    }
    cout << "\n";
    printFormMap();
}

void TruthTable::simplify(string &form) {
    bool running = true;
    while (running) {
        int len = form.size();
        if (len == 1 || len == 2) break;
        for (int i = 0; i < len-2; ++i) {
            char ch = form[i];
            if (ch == '(' && form[i+2] == ')') {
                string before = (i > 0 ? form.substr(0, i) : "");
                string after = (i+3 < len ? form.substr(i+3) : "");
                form = before + form[i+1] + after;
                break;
            }
            if (ch == '~' && form[i+1] == '~') {
                int j = i+1;
                while (j < len && form[j] == '~') ++j;
                int negCount = j-i;
                string before = (i > 0 ? form.substr(0, i) : "");
                if (negCount % 2 == 0) {
                    form = before + form.substr(j);
                } else {
                    form = before + form.substr(j-1);
                }
                break;
            }
            if (i == len-3) running = false;
        }
    }
}

string TruthTable::applyVal(string form, int val) {
    int len = form.size();
    for (auto sym : symbols) {
        for (int i = 0; i < len; ++i) {
            if (form[i] == sym) {
                form[i] = valMap[sym][val] + '0';      
            }
        }
    }
    return form;
}

int TruthTable::getBkt(string form) {
    int len = form.size();
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (form[i] == '(') ++count;
    }
    return count;
}

string TruthTable::boolToStr(bool b) {
    int temp = b;
    return to_string(temp);
}

bool TruthTable::strToBool(string s) {
    if (s == "0") return false;
    else if (s == "1") return true;
    cout << "not bool " << s << endl;
    return false;
}

bool TruthTable::solveForm(string vform) {
    if (getBkt(vform) == 0) {
        // cout << "nobkt " << vform << endl;
        int len = vform.size();
        if (len == 1) return strToBool(vform);
        while (vform.find('~') != string::npos) {
            int i = vform.find('~');
            if (vform.substr(i, 2) == "~0") vform = vform.substr(0, i) + "1" + vform.substr(i+2);
            if (vform.substr(i, 2) == "~1") vform = vform.substr(0, i) + "0" + vform.substr(i+2);
        }
        while (vform.find('&') != string::npos) {
            int i = vform.find('&');
            string atom = vform.substr(i-1, 3);
            if (atom == "1&1") vform = vform.substr(0, i-1) + "1" + vform.substr(i+2);
            else vform = vform.substr(0, i-1) + "0" + vform.substr(i+2);
        }
        while (vform.find('|') != string::npos) {
            int i = vform.find('|');
            string atom = vform.substr(i-1, 3);
            if (atom == "0|0") vform = vform.substr(0, i-1) + "0" + vform.substr(i+2);
            else vform = vform.substr(0, i-1) + "1" + vform.substr(i+2);
        }
        int pos = 0;
        while (pos < len && vform.find("->", pos) != string::npos) {
            int i = vform.find("->");
            if (vform[i-1] == '<') {pos = i+1; continue;}
            string atom = vform.substr(i-1, 4);
            if (atom == "1->0") vform = vform.substr(0, i-1) + "0" + vform.substr(i+3);
            else vform = vform.substr(0, i-1) + "1" + vform.substr(i+3);
        }
        while (vform.find("<->") != string::npos) {
            int i = vform.find("<->"); 
            string atom = vform.substr(i-1, 5);
            if (atom == "1<->1" || atom == "0<->0") vform = vform.substr(0, i-1) + "1" + vform.substr(i+4);
            else vform = vform.substr(0, i-1) + "0" + vform.substr(i+4);
        }
        return strToBool(vform);
    } else {
        int i = 0;
        while (i < vform.size()) {
            if (vform[i] == ')') {
                int j = i-1;
                while (j >= 0 && vform[j] != '(') --j;
                string before = vform.substr(0, j);
                // cout << "i " << i << endl; 
                // cout << "j " << j << endl; 
                // cout << "substr " << vform.substr(j+1, i-j-1) << endl;
                string solved = boolToStr(solveForm(vform.substr(j+1, i-j-1)));
                string after = vform.substr(i+1);
                vform = before + solved + after;
                // cout << "vform " << vform << endl;
                i = before.size() + 1;
            } else {
                ++i;
            }
        }
        return solveForm(vform);
    } 
}

void TruthTable::evaluate() {
    vector<string> newFormulas;
    for (string form : formulas) {
        simplify(form);
        newFormulas.push_back(form);
        for (int val = 0; val < valLen; ++val) {
            string vform = applyVal(form, val);
            bool res = solveForm(vform);
            formMap[form].push_back(res);
        }
    }
    formulas = newFormulas;
}