#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include "valid.h"
using namespace std;

/*
Actual      Code       Num
¬           ~          1
∧           &          2
∨           |          3
−→          ->         4
←→          <->        5
*/


static bool validSym(unordered_set<char> symbols, char ch) {
    for (auto sym : symbols) {
        if (ch == sym) return true;
    }
    return false;
}

// assumes all letters are symbols 
static bool isSym(char ch) {
    return ch >= 'a' && ch <= 'z';
}

static int validConn(string form, int &i) {
    char ch = form[i];
    if (ch == '~') return 1; 
    if (ch == '&') return 2; 
    if (ch == '|') return 3;
    if (i > 0 && i + 2 < form.size() && form.substr(i, 2) == "->") {
        ++i;
        return 4;
    } else if (i > 0 && i + 3 < form.size() && form.substr(i, 3) == "<->") {
        i += 2;
        return 5;
    }
    return 0;
}

// checks if symbols and () are not adjacent
// checks if symbols are correctly positioned relative to brackets
// assumes all letters are symbols 
static bool checkSymAndBkt(string form, int len) {
    for (int i = 0; i < len-1; ++i) {
        char curr = form[i];
        char next = form[i+1];
        if (isSym(curr) && isSym(next)) return false;
        if ((curr == '(' && next == ')') || (curr == ')' && next == '(')) return false;
        if ((isSym(curr) && next == '(') || (curr == ')' && isSym(next))) return false;
    }
    return true;
}

// checks if connectives are correctly positioned relative to symbols
// assumes all letters are symbols 
static bool validConnPos(string form, int len, int conn_count) {
    int i = 0;
    while (conn_count > 0) {
        int conn = validConn(form, i);
        int j, k;
        if (conn == 0) {
            ++i;
            continue;
        } else if (conn == 1) {
            j = i-1;
            k = i+1;
            if (j >= 0 && (form[j] == ')' || isSym(form[j]))) return false;
            if (k >= len || !(form[k] == '(' || isSym(form[k]))) return false;
        } else {
            if (conn == 2 || conn == 3) {
                j = i-1;
            } else if (conn == 4) {
                j = i-2;
            } else {
                j = i-3;
            }
            k = i+1;
            if (j < 0 || !(form[j] == ')' || isSym(form[j]))) return false;
            if (k >= len || !(k < len || form[k] == '(' || isSym(form[k]) || (validConn(form, k) == 1))) return false;
        } 
        ++i;
        --conn_count;
    }
    return true;
}

bool validForm(string form, unordered_set<char> symbols) {
    int len = form.size();
    stack<char> stk;
    int i = 0, sym_count = 0, bracket_pairs = 0, conn_count = 0;
    while (i < len) {
        char ch = form[i];
        // ensures all char are valid 
        if (!(ch == '(' || ch == ')' || validConn(form, i) > 0 || validSym(symbols, ch))) return false;
        if (ch == '(') {
            stk.push('(');
            ++bracket_pairs;
        } else if (ch == ')') {
            if (stk.empty() || stk.top() != '(') return false;
            stk.pop();
        } else if (isSym(ch)) {
            ++sym_count;
        } else {
            ++conn_count;
        }
        ++i;
    }
    // checks brackets are balanced, brackets aren't equal to the length, and there's at least 1 symbol
    if (!stk.empty() || 2*bracket_pairs == len || sym_count == 0) return false; 
    if (!checkSymAndBkt(form, len)) return false;
    if (!validConnPos(form, len, conn_count)) return false;
    return true;
}
