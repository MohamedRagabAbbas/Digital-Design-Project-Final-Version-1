/*bool validateExpression(string input)
{

    bool hasplus = 0;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '+') hasplus = 1;
        if (input[i] == '*')
            return 0;
    }
    return hasplus;
}

bool Evaluate(string term, map<char, int> indexOf, vector<bool> table)
{
    string newTerm;
    vector<bool> res;
    bool temp = 1;
    bool result = 0;
    for (int i = 0; i < term.length(); i++)
    {
        if (i != term.length()-1)
        {
            if (term[i + 1] != '`')
            {
                newTerm += !table[indexOf[term[i]]];

                //int i = stoi(std::string(1, c));
                i = i + 1;
                continue;
            }
        }
        if (term[i] != '+')
        {
            temp *= table[indexOf[term[i]]];
        }
        else
        {
            res.push_back(temp);
            temp=1;
        }
    }
    res.push_back(temp);
    temp = 0;
    for (int i = 0; i < res.size(); i++)
        temp += res[i];
    return temp;
}*/



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

struct Implicant;

// functions required

vector<vector<bool>> generateTruthTable(int num);


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
vector<int> merge(vector<int> a, vector<int> b);


// printing functions

void printTruthTable(vector<vector<bool>> table, string Term);
void printSoP(vector<string> exp);
void printPoS(vector<string> exp);
void printAllPrimeImplicants(vector<Implicant> implicants);

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
    int MAX = minterms.back().term.size();
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
        cout << ")" << endl;
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
    cout << "F" << endl;

    for (int i = 0; i < ((int)table[0].size() - 1) * 5; i++) cout << "-";
    cout << "-\n";

    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[0].size(); j++) {
            cout << table[i][j] << "    ";
        }
        cout << endl;
    }
}

void printSoP(vector<string> exp) {
    cout << "The Canonical Sum of Products (SoP) is: ";

    for (int i = 0; i < (int)exp.size() - 1; i++) {
        cout << "(" << exp[i] << ")" << " + ";
    }
    if (exp.size() != 0) cout << "(" << exp[exp.size() - 1] << ")" << endl;
    else cout << '0' << endl;
}

void printPoS(vector<string> exp) {
    cout << "The Canonical Product of Sums (PoS) is: ";

    for (int i = 0; i < (int)exp.size() - 1; i++) {
        cout << "(" << exp[i] << ")" << " * ";
    }
    if (exp.size() != 0) cout << "(" << exp[exp.size() - 1] << ")" << endl;
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

string Replace(string term, map<char, int> indexOf, vector<bool> table) {
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
int evaluateExpression(string expression)
{
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
        table[k][table[k].size() - 1] = evaluateExpression(Replace(term, indexOf, table[k]));
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

vector<string> getMaxterms(const vector<vector<bool>>& table) {
    vector<string> maxterms;
    for (int i = 0; i < table.size(); i++) {
        if (table[i].back() == 0) maxterms.push_back(to_string(i));
    }
    return maxterms;
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

vector<Implicant> removeDuplicates(vector<Implicant>& impicant) {
    unordered_set<string> uniqueStrings;
    vector<Implicant> result;
    for (auto str : impicant)
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
vector<Implicant> generateMinterms(vector<int> m)
{
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
    int c = minterms.back();
    int size = log2(minterms.back()) + 1;
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
        cout << endl;
    }
    cout << endl;
}

vector<string> epi(vector <Implicant>& prim, vector<int>& m) {
    vector<int> deletedM;
    vector<string> EPI;
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
            EPI.push_back(espi_test);

            for (int j = 1; j < table[0].size(); j++)
            {
                if (table[row][j] == "x")
                {
                    deletedM.push_back(m[j - 1]);
                }
            }
        }
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

void remove_dominating_rows_and_columns(vector<vector<string>>& matrix) {
    set<int> col;
    set<int> row;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = i + 1; j < matrix.size(); j++) {
            bool dominates = true;
            bool dominated = true;
            for (int k = 0; k < matrix[i].size(); k++) {
                if (matrix[i][k] < matrix[j][k]) {
                    dominates = false;
                }
                if (matrix[i][k] > matrix[j][k]) {
                    dominated = false;
                }
            }
            if (dominates) {
                col.insert(j);
            }
            if (dominated) {
                row.insert(i);
            }
        }
    }

    for (int i = (int)matrix.size() - 1; i >= 0; i--) {
        if (row.count(i)) matrix.erase(matrix.begin() + i);
    }


    for (int i = (int)matrix.size() - 1; i >= 0; i--) {
        if (col.count(i)) matrix[i].erase(matrix[i].begin() + i);
    }

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
        cout << endl;
    }
    cout << endl;
}

vector<vector<string>> reduceTable(vector<string> EPI, vector<vector<string>> CC) {
    set<string> EPrimes;
    for (int i = 0; i < EPI.size(); i++) {
        EPrimes.insert(EPI[i]);
    }
    //    cout << "Essential Primes: \n";
    //    for(auto u : EPI) cout << u << "\n";

    set<int> removeRow, removeCol;
    for (int i = 0; i < CC.size(); i++) {
        if (EPrimes.count(CC[i][0])) {
            removeRow.insert(i);
            for (int j = 1; j < CC[i].size(); j++) {
                if (CC[i][j] == "x") removeCol.insert(j);
            }
        }
    }

    // remove rows covered by essential prime implicants
    for (int i = (int)CC.size() - 1; i >= 0; i--) {
        if (removeRow.count(i)) CC.erase(CC.begin() + i);
    }

    // remove minterms covered by essential prime implicants
    for (int i = (int)CC.size() - 1; i >= 0; i--) {
        if (removeCol.count(i)) CC[i].erase(CC[i].begin() + i);
    }

    string s1 = "\nReduced PI Table after removing Essential Prime Implicants";
    printReducedPITable(CC, s1);

    remove_dominating_rows_and_columns(CC);

    string s2 = "\nReduced PI Table after removing dominated rows and dominating columns";
    printReducedPITable(CC, s2);

    return CC;
}

void minimized_expression(vector<vector<string>> final_table, vector<string> EPI) {

    vector<string> answer;
    for (int i = 0; i < EPI.size(); i++) {
        answer.push_back(EPI[i]);
    }
    for (int i = 1; i < final_table.size(); i++) {
        answer.push_back(final_table[i][0]);
    }
    for (int i = 0; i < answer.size(); i++) {
        answer[i] = fromImplicantToTerm(answer[i]);
    }

    sort(answer.begin(), answer.end());
    cout << "\nMinimized expression: ";
    for (int i = 0; i < answer.size(); i++) {
        cout << answer[i];
        if (i != (int)answer.size() - 1) cout << " + ";
    }
    cout << "\n";
}



bool validateExpression(string input)
{
    for (int i = input.size() - 1; i >= 0; i--) {
        if (input[i] == ' ') input.pop_back();
    }
    int nonVariables = 0;
    for (int i = 0; i < input.size(); i++) {
        if ((input[i] < 'a' || input[i] > 'z') && input[i] != '`') nonVariables++;
    }
    if (nonVariables == 0) return true;

    bool hasplus = 0;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '+')
            hasplus = 1;
        if (input[i] == '*')
            return 0;
    }

    return hasplus;
}

string modify(string input) {

    for (int i = 0; i < input.size() - 1; i++)
    {
        if (((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z') || (input[i] >= '`')) && ((input[i + 1] >= 'A' && input[i + 1] <= 'Z') || (input[i + 1] >= 'a' && input[i + 1] <= 'z')))
        {
            input.insert(input.begin() + i + 1, '*');
        }
    }
    return input;
}


void Handlinginput() {
    bool test1 = 1;
    bool test2 = 1;
    string Term;
    string ex;
    vector<int> minterms;
    vector<Implicant> Minterms;
    vector<int> donotCare;
    vector<vector<bool>> table;
    vector<string> pos;
    vector<string> sop;
    vector<Implicant> all;
    vector<string> AllEPI;
    int temp;
    int size;
    int size2;
    while (test1)
    {
        test1 = 1;
        test2 = 1;
        cout << "Please enter your input type" << endl;
        cout << "1. Expression" << endl;
        cout << "2. Minterms" << endl;
        cout << "3. Minterms and Don't Care Terms" << endl;
        cout << "0. Exit" << endl;
        int input_type;
        cout << "Enter Your Choice : ";
        cin >> input_type;

        switch (input_type)
        {
        case 1:
            cout << "Please enter your expression (SoP form): ";
            cin.ignore();
            getline(cin, ex);
            if (!validateExpression(ex))
            {
                cout << "This Expression Is Not Valid" << endl;
                test2 = 0;
            }
            ex = modify(ex);
            cout << ex << endl;
            break;
        case 2:
            cout << "Please enter the number of minterms : ";
            cin >> size;
            for (int i = 0; i < size; i++)
            {
                cout << "Enter minterm " << i + 1 << " : ";
                cin >> temp;
                minterms.push_back(temp);
            }
            break;
        case 3:
            cout << "Please enter the number of minterms : ";
            cin >> size;
            cout << "Please enter the number of do not care terms : ";
            cin >> size2;
            for (int i = 0; i < size; i++)
            {
                cout << "Enter minterm " << i + 1 << " : ";
                cin >> temp;
                minterms.push_back(temp);
            }
            for (int i = 0; i < size2; i++)
            {
                cout << "Enter do not care minterm " << i + 1 << " : ";
                cin >> temp;
                donotCare.push_back(temp);
            }
            break;
        case 0:
            test1 = 0;
            break;
        default:
            cout << "Invalid input type. Exiting program." << endl;
            return;
        }

        // Perform operation based on the user's choice
        while (test2)
        {
            // Display operation options to the user

            cout << endl << "Please choose an operation: " << endl;
            cout << "1. Truth table" << endl;
            cout << "2. Canonical SOP" << endl;
            cout << "3. Canonical POS" << endl;
            cout << "4. All PIs" << endl;
            cout << "5. Print Coverage Chart" << endl;
            cout << "6. All EPIs" << endl;
            cout << "7. All implicants that are not covered by EPIs" << endl;
            cout << "8. Minimized Function" << endl;
            cout << "9. Start from beginning" << endl;
            cout << "0. Exit" << endl;
            int operation = 0;
            cout << "Enter Your Choice : ";
            cin >> operation;
            switch (operation)
            {
            case 1:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    printTruthTable(table, Term);
                }
                else if (input_type == 2)
                {
                    table = fillTableWithMinterms(minterms);
                    printTruthTable(table, "");
                }
                break;
            case 2:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    SoP(table, Term, sop);
                    printSoP(sop);
                }
                else if (input_type == 2)
                {
                    table = fillTableWithMinterms(minterms);
                    SoP(table, getTerm(minterms), sop);
                    printSoP(sop);
                }
                break;
            case 3:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    PoS(table, Term, pos);
                    printPoS(pos);
                }
                else if (input_type == 2)
                {
                    table = fillTableWithMinterms(minterms);
                    PoS(table, getTerm(minterms), pos);
                    printPoS(pos);
                }
                break;
            case 4:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    printAllPrimeImplicants(all);
                }
                else if (input_type == 2)
                {
                    vector<Implicant> mins = generateMinterms(minterms);
                    all = getPrimeImplicants(mins);
                    printAllPrimeImplicants(all);
                }
                else if (input_type == 3)
                {
                    vector<int> allterms = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(allterms);
                    all = getPrimeImplicants(mins);
                    printAllPrimeImplicants(all);
                }

                break;
            case 5:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    PrintCoverageChart(CoverageChart(all, allminterms));

                }
                else if (input_type == 2)
                {
                    vector<int> copy = minterms;
                    //vector<int> copy2 = donotCare;
                    //vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy);
                    all = getPrimeImplicants(mins);
                    PrintCoverageChart(CoverageChart(all, copy));
                }
                else if (input_type == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = donotCare;
                    vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    PrintCoverageChart(CoverageChart(all, copy));
                }

                break;
            case 6:
                if (input_type == 1)
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
                        cout << setw(6) << fromImplicantToTerm(AllEPI[i]) << " : " << AllEPI[i] << endl;
                }
                else if (input_type == 2 || input_type == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = donotCare;
                    vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);
                    cout << "\nAll Essential Prime Implicants:\n\n";
                    for (int i = 0; i < AllEPI.size(); i++)
                    {
                        cout << setw(5) << fromImplicantToTerm(AllEPI[i]) << " : " << AllEPI[i] << endl;
                    }
                    cout << endl;
                }
                break;
            case 7:
                if (input_type == 1)
                {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    AllEPI = epi(all, allminterms);
                    cout << endl;
                    PrintCoverageChart(CoverageChart(all, allminterms));
                    cout << "\nThe minterms that are not covered by EPIs: ";
                    for (int i = 0; i < allminterms.size(); i++)
                    {
                        if (i == 0) cout << "(";
                        cout << allminterms[i];
                        if (i != (int)allminterms.size() - 1) cout << ", ";
                        else cout << ")";
                    }
                    cout << endl;
                }
                else if (input_type == 2 || input_type == 3)
                {
                    vector<int> copy = minterms;
                    vector<int> copy2 = donotCare;
                    vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);
                    cout << endl;
                    PrintCoverageChart(CoverageChart(all, copy));
                    cout << "\nThe minterms that are not covered by EPIs : ";
                    for (int i = 0; i < copy.size(); i++)
                    {
                        if (i == 0) cout << "(";
                        cout << copy[i];
                        if (i != (int)copy.size() - 1) cout << ", ";
                        else cout << ")";
                    }
                    cout << endl;
                }
                break;
            case 8:
                if (input_type == 1) {
                    Term = returnVariables(ex);
                    table = generateTruthTable(Term.size());
                    output(table, ex);
                    vector<int> allminterms = getMintermsAsInt(table);
                    Minterms = getMinterms(table);
                    all = getPrimeImplicants(Minterms);
                    AllEPI = epi(all, allminterms);
                    vector<vector<string>> reducedPITable = reduceTable(AllEPI, CoverageChart(all, allminterms));

                    minimized_expression(reducedPITable, AllEPI);
                }
                else if (input_type == 2 || input_type == 3) {
                    vector<int> copy = minterms;
                    vector<int> copy2 = donotCare;
                    vector<int> copy3 = merge(minterms, donotCare);
                    vector<Implicant> mins = generateMinterms(copy3);
                    all = getPrimeImplicants(mins);
                    AllEPI = epi(all, copy);
                    //vector<Implicant> m=findMinimumCover(all, copy);
                    //for (int i = 0; i < m.size(); i++)
                    //{
                    //    cout << m[i].term << endl;
                    //}

                    vector<vector<string>> reducedPITable = reduceTable(AllEPI, CoverageChart(all, copy));
                    minimized_expression(reducedPITable, AllEPI);
                }
                break;
            case 9:
                test2 = 0;
                break;
            case 0:
                test1 = 0;
            default:
                cout << "Invalid operation. Exiting program." << endl;
                return;
            };
        }
    }


}
int main()
{
    Handlinginput();

}
