#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include "valid.h"
#include "truthtable.h"
using namespace std;

int main() {
    cout << "Welcome to truth table maker!" << endl;
    cout << "Use ~ for 'not'" << endl;
    cout << "Use & for 'and'" << endl;
    cout << "Use | for 'or'" << endl;
    cout << "Use -> for 'implies'" << endl;
    cout << "Use <-> for 'iff'" << endl;
    cout << "List propositional symbols in lower case and exit with a period (eg. abc.):" << endl;
    char ch;
    set<char> symbols;
    while (cin >> ch) {
        if (ch == '.' || ch == ',') break;
        if ('a' <= ch && ch <= 'z') symbols.insert(ch);
    }
    cout << "Enter propositional formulas and exit with a period (ex: a->b):" << endl;
    vector<string> formulas;
    string form;
    while (cin >> form) {
        if (form == "." || form == ",") break;
        if (validForm(form, symbols)) {
            formulas.push_back(form);
        } else {
            cout << "Invalid formula" << endl;
        }
    }
    // for (auto s : symbols) { cout << s << endl; }
    // for (auto f : formulas) { cout << f << endl; }
    TruthTable tt{formulas, symbols};
    tt.evaluate();
    tt.printTruthTable();
}
