#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <string>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <map>
#include <cmath>
#include <sstream>
using namespace std;

// struct that represents our implicants

struct Implicant;

// main functions

vector<vector<bool>> generateTruthTable(int num);
void SoP(const vector<vector<bool>>& table, string vnames, vector<string>& sop);
void PoS(const vector<vector<bool>>& table, string vnames, vector<string>& sop);
vector<vector<Implicant>> columnOne(vector<Implicant>& minterms);
vector<vector<Implicant>> compare(vector<vector<Implicant>>& prev, int& combine);
vector<Implicant> getPrimeImplicants(vector<Implicant>& minterms);
vector<string> epi(vector <Implicant>& prim, vector<int>& m);
map<string, int> Columns(vector<vector<string>>& matrix);
map<string, int> Rows(vector<vector<string>>& matrix, map<string, int> c);
void removeColumns(vector<vector<string>>& table, vector<int>& columnsToRemove);
vector<string> remove_dominating_rows_and_columns(vector<vector<string>>& matrix);
vector<string> minimizing(vector<string> EPI, vector<vector<string>> CC);

// helper functions

string decToBin(int n);
vector<Implicant> padding(vector<Implicant>& minterms);
int countOnes(string s);
bool isExist(string term, char var);
string returnVariables(string term);
vector<char> correspondingVariables(string term);
map<char, int> returnMap(string term);
string fromImplicantToTerm(string implicant);
string vectorToString(const vector<int>& vector);
string replace(string term, map<char, int> indexOf, vector<bool> table);
bool isOperator(char c);
int performOperation(int operand1, int operand2, char op);
int evaluateExpression(string expression);
void output(vector<vector<bool>>& table, string t);
vector<Implicant> getMinterms(const vector<vector<bool>>& table);
vector<int> getMintermsAsInt(const vector<vector<bool>>& table);
vector<Implicant> removeDuplicates(vector<Implicant>& implicant);
vector<int> merge(vector<int> a, vector<int> b);
vector<Implicant> generateMinterms(vector<int> m);
vector<vector<bool>> fillTableWithMinterms(vector<int> minterms);
string getTerm(vector<int> minterms);
bool findmin(Implicant test, string min);
vector<vector<string>> CoverageChart(vector <Implicant>& prim, vector<int>& m);
bool validateExpression(string input);
string modify(string input);
vector<int> stringToInt(string term);
bool check(string term);

// printing functions

void printTruthTable(vector<vector<bool>> table, string Term);
void printSoP(vector<string> exp);
void printPoS(vector<string> exp);
void printAllPrimeImplicants(vector<Implicant> implicants);
void PrintCoverageChart(vector<vector<string>> table);
void printReducedPITable(vector<vector<string>> table, string s);

struct Implicant {
    string term;
    vector<int> minterms;
    bool checked = false;
};

string decToBin(int n) {
    string r = "";
    while (n > 0) {
        r += to_string(n % 2);
        n /= 2;
    }
    reverse(r.begin(), r.end());
    return r;
}

vector<Implicant> padding(vector<Implicant>& minterms) {
    string extra = "0000000000";
    int MAX;
    if(minterms.size() != 0) MAX = minterms.back().term.size();
    for (int i = 0; i < minterms.size(); i++) {
        int need = MAX - minterms[i].term.size();
        minterms[i].term.insert(0, extra.substr(0, need));
    }
    return minterms;
}

int countOnes(string s) {
    int ones = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '1') ones++;
    }
    return ones;
}

bool isExist(string term, char var) {
    for (int i = 0; i < term.size(); i++) {
        if (term[i] == var)  return true;
    }
    return false;
}

// (a+c+d)*e --> a,c,d,e. We will know the # of variables and corresponding variable to each column in the truth table
string returnVariables(string term) {
    string variablesTerm;
    for (int i = 0; i < term.size(); i++) {
        if (term[i] == ' ') continue;
        if ((term[i] != '+') && (term[i] != '*') && (term[i] != '`') && term[i] != '(' && term[i] != ')' && isExist(variablesTerm, term[i]) == false)
            variablesTerm.push_back(term[i]);
    }
    return variablesTerm;
}

vector<char> correspondingVariables(string term) {
    vector<char> variables;
    for (int i = 0; i < term.size(); i++) {
        variables.push_back(term[i]);
    }
    return variables;
}

map<char, int> returnMap(string term) {
    map<char, int> m;
    for (int i = 0; i < term.size(); i++) {
        m[term[i]] = i;
    }
    return m;
}

string fromImplicantToTerm(string implicant) {
    string term;
    char letter = 'a';
    for (int i = 0; i < implicant.size(); i++) {
        if (implicant[i] == '-') {
            letter++;
            continue;
        }
        else if (implicant[i] == '1') {
            term += letter++;
        }
        else {
            term += letter++;
            term += '`';
        }
    }
    return term;
}

string vectorToString(const vector<int>& vector) {
    stringstream ss;
    for (int i = 0; i < vector.size(); i++) {
        ss << vector[i];
        if (i != vector.size() - 1) {
            ss << " ";  // Add space separator between elements
        }
    }
    return ss.str();
}

void printAllPrimeImplicants(vector<Implicant> implicants) {

    cout << "\nAll Prime Implicants\n\n";
    for (int j = 0; j < implicants.size(); j++) {
        cout << setw(6) << fromImplicantToTerm(implicants[j].term) << " : " << implicants[j].term << " : ";
        cout << "(";
        for (int i = 0; i < implicants[j].minterms.size(); i++) {
            cout << implicants[j].minterms[i];
            if (i != implicants[j].minterms.size() - 1)
                cout << ",";
        }
        cout << ")" << '\n';
    }
}

void printTruthTable(vector<vector<bool>> table, string Term) {
    cout << "\n";
    for (int i = 0; i < ((int)table[0].size() / 2 - 2) * 4; i++) cout << " ";
    cout << "Truth Table\n\n";

    for (int i = 0; i < Term.length(); i++)
        cout << Term[i] << "    ";

    if (Term.length() == 0) {
        char letter = 'a';
        for (int i = 0; i < (int)table[0].size() - 1; i++)
            cout << letter++ << "    ";
    }
    cout << "F" << '\n';

    for (int i = 0; i < ((int)table[0].size() - 1) * 5; i++) cout << "-";
    cout << "-\n";

    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[0].size(); j++) {
            cout << table[i][j] << "    ";
        }
        cout << '\n';
    }
}

void printSoP(vector<string> exp) {
    cout << "The Canonical Sum of Products (SoP) is: ";

    for (int i = 0; i < (int)exp.size() - 1; i++) {
        cout << "(" << exp[i] << ")" << " + ";
    }
    if (exp.size() != 0) cout << "(" << exp[exp.size() - 1] << ")" << '\n';
    else cout << '0' << '\n';
}

void printPoS(vector<string> exp) {
    cout << "The Canonical Product of Sums (PoS) is: ";

    for (int i = 0; i < (int)exp.size() - 1; i++) {
        cout << "(" << exp[i] << ")" << " * ";
    }
    if (exp.size() != 0) cout << "(" << exp[exp.size() - 1] << ")" << '\n';
    else cout << '1' << "\n";
}

vector<vector<bool>> generateTruthTable(int num) {
    int size = pow(2, num);
    int varing_c = size;
    bool logic = 1;
    vector<vector<bool>> table(size, vector<bool>(num + 1, 0));
    for (int i = 0; i < num; i++)
    {
        varing_c /= 2;
        for (int j = 0; j < size; j++)
        {
            if (varing_c == 0)break;
            if (j % varing_c == 0 && logic == 0)
                logic = 1;
            else if (j % varing_c == 0 && logic == 1)
                logic = 0;

            table[j][i] = logic;
        }
    }
    return table;
}

string demorgan(string term) {
    string demorgan_term;
    //term.pop_back();
    for (int i = 0; i < term.length(); i++) {
        if (term[i] == '(') {
            demorgan_term.push_back(term[i]);
        }
        else if (term[i] == '+') {
            demorgan_term.push_back('*');
        }
        else if (term[i] == '*') {
            demorgan_term.push_back('+');
        }
        else if (i != term.length() - 1 && term[i + 1] == '`') {
            demorgan_term.push_back(term[i++]);
        }
        else {
            demorgan_term.push_back(term[i]);
            demorgan_term.push_back('`');
        }
    }
    return demorgan_term;
}

string replace(string term, map<char, int> indexOf, vector<bool> table) {
    for (int i = 0; i < term.length(); i++)
    {

        if (term[i] == '(')
        {
            for (int j = i + 1; j < term.length(); j++)
            {
                if (j != term.length() - 1)
                {
                    if (term[j] == ')' && term[j + 1] == '`')
                    {
                        term.replace(i, j + 2, demorgan(term.substr(i, j + 2)));
                    }
                }

            }
        }
    }
    string newTerm;
    for (int i = 0; i < term.length(); i++)
    {

        if (i != term.length() - 1 && term[i + 1] == '`')
            newTerm.push_back((table[indexOf[term[i++]]] + '0') == '0' ? '1' : '0');
        else if ((term[i] >= 'a' && term[i] <= 'z') || (term[i] >= 'A' && term[i] <= 'Z'))
        {
            newTerm.push_back(table[indexOf[term[i]]] + '0');
        }
        else
            newTerm.push_back(term[i]);
    }
    return newTerm;
}

// Function to check if a character is an operator
bool isOperator(char c) {
    if (c == '+' || c == '*') {
        return true;
    }
    return false;
}

// Function to perform the operation between two operands
int performOperation(int operand1, int operand2, char op) {


    if (op == '+') {

        return operand1 + operand2;

    }
    else if (op == '*') {

        return operand1 * operand2;
    }
    else {
        // Throw an error if an invalid operator is encountered
        throw "Invalid operator";
    }
}

// Function to evaluate a mathematical expression with brackets
int evaluateExpression(string expression) {
    stack<int> operands;  // Stack to store operands
    stack<char> operators;  // Stack to store operators

    for (int i = 0; i < expression.length(); i++)
    {
        // Skip spaces
        //if (expression[i] == ' ') {
        //    continue;
        //}

        // If the character is an operand, push it onto the operand stack
        if (isdigit(expression[i])) {
            int operand = expression[i] - '0';
            operands.push(operand);
        }
        // If the character is an opening bracket, push it onto the operator stack
        else if (expression[i] == '(') {
            operators.push(expression[i]);
        }
        // If the character is a closing bracket, evaluate the expression inside the bracket
        // and push the result onto the operand stack
        else if (expression[i] == ')')
        {

            while (!operators.empty() && operators.top() != '(') {
                // Pop the top two operands from the operand stack
                int operand2 = operands.top();
                operands.pop();
                int operand1 = operands.top();
                operands.pop();

                // Pop the top operator from the operator stack
                char op = operators.top();
                operators.pop();

                // Perform the operation and push the result onto the operand stack
                int result = performOperation(operand1, operand2, op);
                operands.push(result);
            }

            // Pop the opening bracket from the operator stack
            operators.pop();
        }
        // If the character is an operator, perform the operation
        else if (isOperator(expression[i]))
        {
            // Pop operators with higher or equal precedence and evaluate the expression
            // before pushing the new operator onto the operator stack
            while (!operators.empty() && operators.top() != '(' &&
                ((expression[i] == '*' && operators.top() == '*') ||
                    (expression[i] == '+' && operators.top() == '+') ||
                    (expression[i] == '+' && operators.top() == '*'))) {

                // Pop the top two operands from the operand stack
                int operand2 = operands.top();
                operands.pop();
                int operand1 = operands.top();
                operands.pop();

                // Pop the top operator from the operator stack
                char op = operators.top();
                operators.pop();

                // Perform the operation and push the result onto the operand stack
                int result = performOperation(operand1, operand2, op);
                operands.push(result);
            }

            // Push the new operator onto the operator stack
            operators.push(expression[i]);
        }
        // If the character is not a digit, operator, or bracket, throw an error
        else {
            throw "Invalid character";

        }

    }
    // Evaluate the remaining operators and operands
    while (!operators.empty()) {
        // Pop the top two operands from the operand stack
        int operand2 = operands.top();
        operands.pop();
        int operand1 = operands.top();
        operands.pop();

        // Pop the top operator from the operator stack
        char op = operators.top();
        operators.pop();

        // Perform the operation and push the result onto the operand stack
        int result = performOperation(operand1, operand2, op);

        operands.push(result);
    }
    // The final result is the top operand on the operand stack

    return operands.top();
}

void output(vector<vector<bool>>& table, string t) {
    string term = "";
    for (int i = 0; i < t.size(); i++) {
        if (t[i] != ' ') term += t[i];
    }
    int rows = table.size();
    vector<bool> row;

    map<char, int> indexOf = returnMap(returnVariables(term));
    for (int k = 0; k < table.size(); k++)
    {
        table[k][table[k].size() - 1] = evaluateExpression(replace(term, indexOf, table[k]));
    }
}

void SoP(const vector<vector<bool>>& table, string vnames, vector<string>& sop) {
    int numOfVariables = vnames.length();
    int x = table.size();
    int count = 0;
    for (int i = 0; i < x; i++)
    {
        if (table[i][numOfVariables] == 1)
        {
            count++;
        }
    }
    sop.resize(count);
    int count2 = -1;
    for (int i = 0; i < x; i++)
    {
        if (table[i][numOfVariables] == 1)
        {
            count2++;
            sop[count2] = "";
            for (int j = 0; j < numOfVariables; j++)
            {
                if (table[i][j] == 1)
                {
                    sop[count2] = sop[count2] + vnames[j];

                }
                else
                {
                    sop[count2] = sop[count2] + vnames[j] + "'";
                }

            }
        }
    }

}

void PoS(const vector<vector<bool>>& table, string vnames, vector<string>& sop) {
    int numOfVariables = vnames.length();
    int x = table.size();
    int count = 0;
    for (int i = 0; i < x; i++)
    {
        if (table[i][numOfVariables] == 0)
        {
            count++;
        }
    }
    sop.resize(count);
    int count2 = -1;
    int counter = 0;
    for (int i = 0; i < x; i++)
    {
        counter = 0;
        if (table[i][numOfVariables] == 0)
        {
            count2++;
            sop[count2] = "";
            for (int j = 0; j < numOfVariables; j++)
            {
                if (table[i][j] == 0)
                {
                    sop[count2] = sop[count2] + vnames[j];
                    counter++;
                    if (counter < numOfVariables)
                        sop[count2] = sop[count2] + '+';
                }
                else
                {
                    sop[count2] = sop[count2] + vnames[j] + "'";
                    counter++;
                    if (counter < numOfVariables)
                        sop[count2] = sop[count2] + '+';
                }

            }
        }
    }

}

vector<Implicant> getMinterms(const vector<vector<bool>>& table) {
    vector<Implicant> minterms;
    for (int i = 0; i < table.size(); i++) {
        if (table[i].back() == 1) {
            Implicant im;
            im.term = decToBin(i);
            im.minterms.push_back(i);
            minterms.push_back(im);
        }
    }
    padding(minterms);
    return minterms;
}

vector<int> getMintermsAsInt(const vector<vector<bool>>& table) {
    vector<int> minterms;
    for (int i = 0; i < table.size(); i++)
        if (table[i].back() == 1)
            minterms.push_back(i);
    return minterms;
}

vector<vector<Implicant>> columnOne(vector<Implicant>& minterms) {
    if (minterms.empty()) {
        return {};
    }

    map<int, vector<Implicant>> m{};
    for (int i = 0; i < minterms.size(); i++) {
        int ones = countOnes(minterms[i].term);
        m[ones].push_back(minterms[i]);
    }

    vector<vector<Implicant>> result;

    for (auto it : m) {
        result.push_back(it.second);
    }
    return result;
}

vector<int> merge(vector<int> a, vector<int> b) {
    vector<int> c;
    for (int i = 0; i < a.size(); i++) c.push_back(a[i]);
    for (int i = 0; i < b.size(); i++) c.push_back(b[i]);
    return c;
}

vector<vector<Implicant>> compare(vector<vector<Implicant>>& prev, int& combine) {
    int counter = 0;
    int index = 0;
    string newString;
    vector<vector<Implicant>> next;
    Implicant im;
    set<string> combinations; // set to keep track of added combinations
    for (int m = 0; m < (int)prev.size() - 1; m++) {
        vector<Implicant> temp;
        for (int i = 0; i < (int)prev[m].size(); i++) {
            bool flag = false;
            for (int j = 0; j < (int)prev[m + 1].size(); j++) {
                counter = 0;
                for (int k = 0; k < prev[m][i].term.size(); k++) {
                    if (prev[m][i].term[k] != prev[m + 1][j].term[k]) {
                        counter++;
                        index = k;
                    }
                }
                if (counter == 1) {
                    flag = true;
                    combine++;
                    prev[m][i].checked = true;
                    prev[m + 1][j].checked = true;
                    newString = prev[m][i].term;
                    newString[index] = '-';
                    im.term = newString;
                    im.minterms = merge(prev[m][i].minterms, prev[m + 1][j].minterms);
                    // check if combination already exists in set
                    if (combinations.find(newString) == combinations.end()) {
                        temp.push_back(im);
                        combinations.insert(newString); // add combination to set
                    }
                }
            }
        }
        next.push_back(temp);
    }
    return next;
}

vector<Implicant> removeDuplicates(vector<Implicant>& implicant) {
    unordered_set<string> uniqueStrings;
    vector<Implicant> result;
    for (auto str : implicant)
    {
        if (uniqueStrings.count(str.term) == 0)
        {
            uniqueStrings.insert(str.term);
            result.push_back(str);
        }
    }
    return result;
}
vector<Implicant> getPrimeImplicants(vector<Implicant>& minterms) {

    vector<vector<Implicant>> next = columnOne(minterms);
    vector<Implicant> allPrimeImplicants;
    bool finished = false;
    int combine = 0;
    vector<vector<Implicant>> next1 = next;
    while (!finished) {
        combine = 0;
        next = compare(next1, combine);
        if (combine == 0) { finished = true; }
        for (int i = 0; i < next1.size(); i++)
        {
            for (int j = 0; j < next1[i].size(); j++)
            {
                if (!next1[i][j].checked)
                {
                    allPrimeImplicants.push_back(next1[i][j]);
                }
            }
        }
        next1 = next;
    }

    allPrimeImplicants = removeDuplicates(allPrimeImplicants);
    return allPrimeImplicants;
}

vector<Implicant> generateMinterms(vector<int> m) {
    vector<Implicant> Minterms;
    for (int i = 0; i < m.size(); i++)
    {
        Implicant temp;
        temp.minterms.push_back(m[i]);
        temp.term = decToBin(m[i]);
        Minterms.push_back(temp);
    }
    padding(Minterms);
    return Minterms;
}

vector<vector<bool>> fillTableWithMinterms(vector<int> minterms) {
    sort(minterms.begin(), minterms.end());
    if (minterms.size() != 0) int c = minterms.back();
    int size = 0;
    if(minterms.size() != 0) size = log2(minterms.back()) + 1;
    vector<vector<bool>> table = generateTruthTable(size);
    for (int i = 0; i < minterms.size(); i++)
        table[minterms[i]][table[i].size() - 1] = 1;
    return table;
}

string getTerm(vector<int> minterms) {
    string r;
    char letter = 'a';
    sort(minterms.begin(), minterms.end());
    for (int i = 0; i < (int)log2(minterms.back()) + 1; i++) {
        r += letter++;
    }
    return r;
}

bool findmin(Implicant test, string min) {
    for (int i = 0; i < test.minterms.size(); i++)
    {
        if (to_string(test.minterms[i]) == min)
        {
            return true;
        }
    }
    return false;
}

vector<vector<string>> CoverageChart(vector <Implicant>& prim, vector<int>& m) {
    vector<vector<string>> table;
    vector<string> minterms;
    vector<int> deletedM;
    for (int i = 0; i < m.size(); i++)
    {
        minterms.push_back(to_string(m[i]));
    }
    vector<string> EPI;
    table.resize(prim.size() + 1);
    for (int i = 0; i < table.size(); i++)
    {
        table[i].resize(minterms.size() + 1);
    }
    table[0][0] = "  Z  ";
    for (int i = 0; i < minterms.size(); i++)
    {
        table[0][i + 1] = minterms[i];
    }

    for (int i = 0; i < prim.size(); i++)
    {
        table[i + 1][0] = prim[i].term;
        for (int k = 1; k < table[0].size(); k++)
        {
            if (findmin(prim[i], table[0][k]))
                table[i + 1][k] = "x";
            else table[i + 1][k] = "0";
        }
    }

    return table;

}

void PrintCoverageChart(vector<vector<string>> table) {
    cout << "\n";
    for (int i = 0; i < (int)table[0].size() / 2 * 6 - 1; i++) cout << " ";
    cout << "Coverage Table\n";
    for (int i = 0; i < table.size(); i++)
    {
        cout << "\t\t  ";
        if (i == 1) {
            for (int i = 0; i < ((int)table[0].size() - 2) * 7; i++) cout << "-";
            cout << "---";
        }
        cout << "\n";
        for (int k = 0; k < table[0].size(); k++)
        {
            if (k == 1 && i != 0) cout << setw(4) << "|" << table[i][k] << "  ";
            else  cout << setw(5) << table[i][k] << "  ";
        }
        cout << '\n';
    }
    cout << '\n';
}

vector<string> epi(vector <Implicant>& prim, vector<int>& m) {
    vector<int> deletedM;
    vector<string> EPI;
    set<string> EPI1;
    vector<vector<string>> table = CoverageChart(prim, m);
    table.resize(prim.size() + 1);
    int counter = 0;
    int row;
    string espi_test;

    for (int i = 1; i < table[0].size(); i++)
    {
        counter = 0;
        for (int j = 1; j < table.size(); j++)
        {
            if (table[j][i] == "x")
            {
                counter++;
                row = j;
                espi_test = table[j][0];
            }
        }
        if (counter == 1)
        {

            EPI1.insert(espi_test);

            for (int j = 1; j < table[0].size(); j++)
            {
                if (table[row][j] == "x")
                {
                    deletedM.push_back(m[j - 1]);
                }
            }
        }
    }
    for (const auto& element : EPI1) {
        EPI.push_back(element);
    }

    vector<int> newMinterms;
    bool flag = false;
    for (int i = 0; i < m.size(); i++)
    {
        flag = 0;
        for (int j = 0; j < deletedM.size(); j++)
        {
            if (m[i] == deletedM[j])
            {
                flag = 1;
                break;
            }

        }
        if (!flag)
        {
            newMinterms.push_back(m[i]);
        }
    }
    m = newMinterms;

    bool flag2;
    vector<Implicant> newImplicants;
    for (int i = 0; i < prim.size(); i++)
    {
        flag2 = 0;
        for (int j = 0; j < EPI.size(); j++)
        {
            if (prim[i].term == EPI[j])
            {
                flag2 = 1;
                break;
            }
        }
        if (!flag2)
        {
            newImplicants.push_back(prim[i]);
        }
    }
    prim = newImplicants;
    return EPI;
}



map<string, int> Columns(vector<vector<string>>& matrix) {
    map<string, int> m{};
    for (int i = 1; i < matrix.size(); i++)
    {
        for (int j = 1; j < matrix[i].size(); j++)
        {
            if (matrix[i][j] == "x")
            {
                m[matrix[0][j]]++;
            }
        }
    }
    vector<pair<string, int>> v(m.begin(), m.end());
    sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
        });
    reverse(v.begin(), v.end());

    return m;

}

map<string, int> Rows(vector<vector<string>>& matrix, map<string, int> c) {
    map<string, int> r{};
    for (int i = 1; i < matrix.size(); i++)
    {
        for (int j = 1; j < matrix[i].size(); j++)
        {
            if (matrix[i][j] == "x")
            {
                r[matrix[i][0]] += c[matrix[0][j]];
            }
        }
    }

    return r;

}

void removeColumns(vector<vector<string>>& table, vector<int>& columnsToRemove) {
    // Sort the vector of columns to remove in descending order
    vector<int> sortedColumnsToRemove = columnsToRemove;
    sort(sortedColumnsToRemove.begin(), sortedColumnsToRemove.end(), greater<int>());

    // Remove the specified columns from each row
    for (auto& row : table) {
        for (const auto& colIndex : sortedColumnsToRemove) {
            row.erase(row.begin() + colIndex);
        }
    }
}


vector<string> remove_dominating_rows_and_columns(vector<vector<string>>& matrix) {
    set<int> col;
    set<int> row;

    vector<string>sol;

    while (matrix.size() > 1 && (matrix[0].size() > 1))
    {
        map<string, int> rows = Rows(matrix, Columns(matrix));
        vector<pair<string, int>> v(rows.begin(), rows.end());
        vector<int>col;
        sort(v.begin(), v.end(), [](const auto& a, const auto& b)
            {
                return a.second < b.second;
            });
        reverse(v.begin(), v.end());
        sol.push_back(v[0].first);
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][0] == v[0].first)
            {
                for (int k = 0; k < matrix[i].size(); k++)
                {
                    if (matrix[i][k] == "x")
                    {
                        col.push_back(k);
                    }
                }
                matrix.erase(matrix.begin() + i);
                removeColumns(matrix, col);
            }
        }
    }

    return sol;
}

vector<string> minimizing(vector<string> EPI, vector<vector<string>> CC) {
    set<string> EPrimes;
    for (int i = 0; i < EPI.size(); i++) {
        EPrimes.insert(EPI[i]);
    }

    set<int> removeRow, removeCol;
    for (int i = 0; i < CC.size(); i++) {
        if (EPrimes.count(CC[i][0])) {
            removeRow.insert(i);
            for (int j = 1; j < CC[i].size(); j++) {
                if (CC[i][j] == "x") removeCol.insert(j);
            }
        }
    }

    for (int i = (int)CC.size() - 1; i >= 0; i--) {
        if (removeRow.count(i)) CC.erase(CC.begin() + i);
    }

    for (int i = (int)CC.size() - 1; i >= 0; i--) {
        if (removeCol.count(i)) CC[i].erase(CC[i].begin() + i);
    }



    vector<string> sol = remove_dominating_rows_and_columns(CC);

    return sol;
}

void printReducedPITable(vector<vector<string>> table, string s) {

    for (int i = 0; i < (int)table[0].size() / 2 * 6 - 1; i++) cout << " ";
    cout << s << "\n";
    for (int i = 0; i < table.size(); i++)
    {
        cout << "\t\t  ";
        if (i == 1) {
            for (int i = 0; i < (int)(table[0].size() - 2) * 7; i++) cout << "-";
            cout << "---";
        }
        cout << "\n";
        for (int k = 0; k < (int)table[0].size(); k++)
        {
            if (k == 1 && i != 0) cout << setw(4) << "|" << table[i][k] << "   ";
            else  cout << setw(5) << table[i][k] << "  ";
        }
        cout << '\n';
    }
    cout << '\n';
}

void printMinimizedExpression(vector<string> answer) {
    cout << "\nMinimized Expression: ";
    for (int i = 0; i < (int)answer.size() - 1; i++) cout << answer[i] << " + ";
    if (answer.size() != 0) cout << answer.back() << "\n";

}

bool validateExpression(string input) {
    for (int i = input.size() - 1; i >= 0; i--) {
        if (input[i] == ' ') input.erase(input.begin() + i);
    }
    //int noOfVariables = 0;
    for (int i = 0; i < input.size(); i++) {
        if (!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || input[i] == '+')) return 0;
    }
    /*if (noOfVariables == 0) return false*/

    //bool hasplus = 0;
    //for (int i = 0; i < input.length(); i++)
    //{
    //    if (input[i] == '+')
    //        hasplus = 1;
    //    if (input[i] == '*')
    //        return 0;
    //}

    return 1;
}

string modify(string input) {

    for (int i = 0; i < input.size() - 1; i++) {
        if (((input[i] >= 'a' && input[i] <= 'z') || (input[i] == '`')) && ((input[i + 1] >= 'a' && input[i + 1] <= 'z'))) {
            input.insert(input.begin() + i + 1, '*');
        }
    }
    return input;
}

vector<int> stringToInt(string term) {
    vector<int> answer;
    term += " ";
    string temp = "";
    for (int i = 0; i < term.size(); i++) {
        if (term[i] == ' ') {
            if (temp.size() != 0) {
                answer.push_back(stoi(temp));
                temp = "";
            }
        }
        else temp += term[i];
    }
    return answer;
}

bool check(string term) {
    term += " ";
    string temp = "";
    for (int i = 0; i < term.size(); i++) {
        if (term[i] == ' ') {
            if (temp.size() != 0) {
                for (int j = 0; j < temp.size(); j++) {
                    if (!isdigit(temp[j])) return false;
                }
                temp = "";
            }
        }
        else temp += term[i];
    }
    return true;
}

bool alwaysTrue(string ans){
    bool flag = true;
    for(char u : ans) if(u != '-') flag = false;
    return flag;
}


void Handlinginput() {
    bool test1 = 1;
    bool test2 = 1;
    string Term;
    string ex;
    vector<int> minterms;
    vector<Implicant> Minterms;
    vector<int> dontCare;
    vector<vector<bool>> table;
    vector<string> pos;
    vector<string> sop;
    vector<Implicant> all;
    vector<string> AllEPI;
    int temp;
    while (test1)
    {
        test1 = 1;
        test2 = 1;
        cout << "Please enter your input type" << '\n';
        cout << "1. Expression" << '\n';
        cout << "2. Minterms" << '\n';
        cout << "3. Minterms and Don't Care Terms" << '\n';
        cout << "0. Exit" << '\n';
        int input;
        cout << "Enter Your Choice : ";
        cin >> input;

        switch (input)
        {
        case 1: {
            cout << "Please enter your expression (SoP form without brackets): ";
            cin.ignore();
            getline(cin, ex);
            if (!validateExpression(ex))
            {
                cout << "This Expression Is Not Valid" << '\n';
                test2 = 0;
            }
            ex = modify(ex);
            cout << ex << '\n';
            break;
        }
        case 2: {
            cout << "Please enter the minterms: ";
            string min = "";
            cin.ignore();
            getline(cin, min);
            if (!check(min)) {
                cout << "Invalid Inputs..." << '\n';
                test2 = 0;
            }
            else
            {
                minterms = stringToInt(min);
            }
            break;
        }
        case 3: {
            cout << "Please enter the minterms: ";
            string min = "", dc = "";
            cin.ignore();
            getline(cin, min);
            if (!check(min)) {
                cout << "Invalid Inputs..." << '\n';
                test2 = 0;
            }
            minterms = stringToInt(min);
            cout << "Please enter the dont care terms: ";
            //cin.ignore();
            getline(cin, dc);
            if (!check(dc)) {
                cout << "Invalid Inputs..." << '\n';
                test2 = 0;
            }
            dontCare = stringToInt(dc);
            break;
        }
        case 0: {
            return;
        }
        default: {
            cout << "Invalid input type. Exiting program." << "\n";
            test2 = 0;
        }
        }

        // Perform operation based on the user's choice
        while (test2)
        {
            // Display operation options to the user

            cout << "\n" << "Please choose an operation: " << "\n";
            cout << "1. Truth Table" << "\n";
            cout << "2. Canonical SoP" << "\n";
            cout << "3. Canonical PoS" << "\n";
            cout << "4. All PIs" << "\n";
            cout << "5. Print Coverage Chart" << "\n";
            cout << "6. All the EPIs" << "\n";
            cout << "7. Minterms that are not covered by the EPIs" << "\n";
            cout << "8. Minimized Expression" << "\n";
            cout << "9. Start from beginning" << "\n";
            cout << "0. Exit" << "\n";
            int operation = 0;
            cout << "Enter Your Choice : ";
            cin >> operation;
            switch (operation)
            {
            case 1: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    printTruthTable(table, Term);
                }
                else if (input == 2 || input == 3)
                {
                    table = fillTableWithMinterms(minterms);
                    printTruthTable(table, "");
                }
                break;
            }
            case 2: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    SoP(table, Term, sop);
                    printSoP(sop);
                }
                else if (input == 2 || input == 3)
                {

                    table = fillTableWithMinterms(minterms);
                    SoP(table, getTerm(minterms), sop);
                    printSoP(sop);
                }
                break;
            }
            case 3: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    PoS(table, Term, pos);
                    printPoS(pos);
                }
                else if (input == 2 || input == 3)
                {
                    table = fillTableWithMinterms(minterms);
                    PoS(table, getTerm(minterms), pos);
                    printPoS(pos);
                }
                break;
            }
            case 4: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    printAllPrimeImplicants(all);
                }
                else if (input == 2)
                {
                    vector<Implicant> mins = generateMinterms(minterms);
                    all = getPrimeImplicants(mins);
                    printAllPrimeImplicants(all);
                }
                else if (input == 3)
                {
                    vector<int> allterms = merge(minterms, dontCare);
                    vector<Implicant> mins = generateMinterms(allterms);
                    all = getPrimeImplicants(mins);
                    printAllPrimeImplicants(all);
                }

                break;
            }
            case 5: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    PrintCoverageChart(CoverageChart(all, allminterms));

                }
                else if (input == 2)
                {
                    vector<int> copy = minterms;
                    //vector<int> copy2 = donotCare;
                    //vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy);
                    all = getPrimeImplicants(mins);
                    PrintCoverageChart(CoverageChart(all, copy));
                }
                else if (input == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = dontCare;
                    vector<int> copy3 = merge(minterms, dontCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    PrintCoverageChart(CoverageChart(all, copy));
                }

                break;
            }
            case 6: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    AllEPI = epi(all, allminterms);
                    cout << "\nAll Essential Prime Implicants:\n\n";
                    for (int i = 0; i < AllEPI.size(); i++)
                    {

                        cout << setw(6) << fromImplicantToTerm(AllEPI[i]) << " : " << AllEPI[i] << '\n';
                    }
                }
                else if (input == 2 || input == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = dontCare;
                    vector<int> copy3 = merge(minterms, dontCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);
                    cout << "\nAll Essential Prime Implicants:\n\n";
                    for (int i = 0; i < AllEPI.size(); i++)
                    {

                        cout << setw(5) << fromImplicantToTerm(AllEPI[i]) << " : " << AllEPI[i] << '\n';
                    }
                    cout << '\n';
                }
                break;
            }
            case 7: {
                if (input == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    AllEPI = epi(all, allminterms);
                    cout << '\n';
                    PrintCoverageChart(CoverageChart(all, allminterms));
                    cout << "\nThe minterms that are not covered by EPIs: ";
                    for (int i = 0; i < allminterms.size(); i++)
                    {
                        if (i == 0) cout << "(";
                        cout << allminterms[i];
                        if (i != (int)allminterms.size() - 1) cout << ", ";
                        else cout << ")";
                    }
                    cout << '\n';
                }
                else if (input == 2 || input == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = dontCare;
                    vector<int> copy3 = merge(minterms, dontCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);
                    cout << '\n';
                    PrintCoverageChart(CoverageChart(all, copy));
                    cout << "\nThe minterms that are not covered by EPIs : ";
                    for (int i = 0; i < copy.size(); i++)
                    {
                        if (i == 0) cout << "(";
                        cout << copy[i];
                        if (i != (int)copy.size() - 1) cout << ", ";
                        else cout << ")";
                    }
                    cout << '\n';
                }
                break;
            }
            case 8: {
                if (input == 1) {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    AllEPI = epi(all, allminterms);

                    vector<string> minimized_expression = minimizing(AllEPI, CoverageChart(all, allminterms));
                    vector<string> answer;
                    
                    if(minimized_expression.size() == 0 && AllEPI.size() == 0){
                        cout << "Minimized Expression: " << 0 << "\n";

                    }
                    else if(minimized_expression.size() == 0 && AllEPI.size() == 1){
                        if(alwaysTrue(AllEPI[0])) {
                            cout << "Minimized Expression: " << 1 << "\n";
                        }
                        else{
                            for (string u : AllEPI) answer.push_back(fromImplicantToTerm(u));
                            for (string u : minimized_expression) answer.push_back(fromImplicantToTerm(u));
                            printMinimizedExpression(answer);
                        }
                    }
                    else{
                        for (string u : AllEPI) answer.push_back(fromImplicantToTerm(u));
                        for (string u : minimized_expression) answer.push_back(fromImplicantToTerm(u));
                        printMinimizedExpression(answer);
                    }

                }
                else if (input == 2 || input == 3) {
                    vector<int> copy = minterms;
                    vector<int> copy2 = dontCare;
                    vector<int> copy3 = merge(minterms, dontCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);

                    vector<string> minimized_expression = minimizing(AllEPI, CoverageChart(all, copy));
                    vector<string> answer;
                    if(minimized_expression.size() == 0 && AllEPI.size() == 0){
                        cout << "Minimized Expression: " << 0 << "\n";

                    }
                    else if(minimized_expression.size() == 0 && AllEPI.size() == 1){
                        if(alwaysTrue(AllEPI[0])) {
                            cout << "Minimized Expression: " << 1 << "\n";
                        }
                        else{
                            for (string u : AllEPI) answer.push_back(fromImplicantToTerm(u));
                            for (string u : minimized_expression) answer.push_back(fromImplicantToTerm(u));
                            printMinimizedExpression(answer);
                        }
                    }
                    else{
                        for (string u : AllEPI) answer.push_back(fromImplicantToTerm(u));
                        for (string u : minimized_expression) answer.push_back(fromImplicantToTerm(u));
                        printMinimizedExpression(answer);
                    }
                }
                break;
            }
            case 9: {
                test2 = 0;
                break;
            }
            case 0: {
                test1 = 0;
            }
            default: {
                cout << "Invalid operation. Exiting program." << '\n';
                return;
            }
            };
        }
    }


}


int main()
{
    Handlinginput();
}
