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

void printSop(vector<string> exp){
    cout << "The Canonical Sum of Products is the follwing: " << endl;
    for (int i = 0; i < exp.size() - 1; i++)
    {
        cout << "(" << exp[i] << ")" << " + ";
    }
    cout << "(" << exp[exp.size() - 1] << ")" << endl;
}

void printPos(vector<string> exp){
    cout << "The Canonical Product of Sums is the follwing: " << endl;
    for (int i = 0; i < exp.size() - 1; i++)
    {
        cout << "(" << exp[i] << ")" << " * ";
    }
    cout << "(" << exp[exp.size() - 1] << ")" << endl;
}

void SoP(const vector<vector<bool>>& table, string vnames, vector<string>& sop){
    int nofvaribals = vnames.length();
    int x = table.size();
    int count = 0;
    for (int i = 0; i < x; i++)
    {
        if (table[i][nofvaribals] == 1)
        {
            count++;
        }
    }
    sop.resize(count);
    int count2 = -1;
    for (int i = 0; i < x; i++)
    {
        if (table[i][nofvaribals] == 1)
        {
            count2++;
            sop[count2] = "";
            for (int j = 0; j < nofvaribals; j++)
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

void PoS(const vector<vector<bool>>& table, string vnames, vector<string>& sop){
    int nofvaribals = vnames.length();
    int x = table.size();
    int count = 0;
    for (int i = 0; i < x; i++)
    {
        if (table[i][nofvaribals] == 0)
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
        if (table[i][nofvaribals] == 0)
        {
            count2++;
            sop[count2] = "";
            for (int j = 0; j < nofvaribals; j++)
            {
                if (table[i][j] == 0)
                {
                    sop[count2] = sop[count2] + vnames[j];
                    counter++;
                    if (counter < nofvaribals)
                        sop[count2] = sop[count2] + '+';
                }
                else
                {
                    sop[count2] = sop[count2] + vnames[j] + "'";
                    counter++;
                    if (counter < nofvaribals)
                        sop[count2] = sop[count2] + '+';
                }

            }
        }
    }

}

map<char, int> returnMap(string Term){
    map<char, int> m;
    for (int i = 0; i < Term.size(); i++)
    {
        m[Term[i]] = i;
    }
    return m;
}

bool isExist(string term, char var){
    for (int i = 0; i < term.size(); i++)
    {
        if (term[i] == var)
        {
            return true;
        }
    }
    return false;
}

string returnVariables(string term){
    string Term;
    for (int i = 0; i < term.size(); i++)
    {
        if ((term[i] != '+') && (term[i] != '*') && (term[i] != '`') && term[i] != '(' && term[i] != ')' && isExist(Term, term[i]) == false)
            Term.push_back(term[i]);
    }
    return Term;
}

void Print(vector<vector<bool>> table, string Term){
    for (int i = 0; i < Term.length(); i++)
        cout << Term[i] << "    ";
    if (Term.length() == 0)
    {
        char letter = 'A';
        for (int i = 0; i < table[0].size() - 1; i++)
            cout << letter++ << "    ";
    }
    cout << "F" << endl;
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[0].size(); j++)
        {
            cout << table[i][j] << "    ";
        }
        cout << endl;
    }
}

vector<vector<bool>> truthTable(int num){
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
string Replace(string term, map<char, int> indexOf, vector<bool> table){
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
                    (expression[i] == '+' && operators.top() == '+'))) {
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

void output(vector<vector<bool>>& table, string term){
    int rows = table.size();
    vector<bool> row;

    map<char, int> indexOf = returnMap(returnVariables(term));
    for (int k = 0; k < table.size(); k++)
    {
        table[k][table[k].size() - 1] = evaluateExpression(Replace(term, indexOf, table[k]));
    }
}

vector<vector<bool>> fillTableWithMinterms(vector<int> minterms){
    sort(minterms.begin(), minterms.end());
    int c = minterms.back();
    int size = log2(minterms.back()) + 1;
    vector<vector<bool>> table = truthTable(size);
    for (int i = 0; i < minterms.size(); i++)
        table[minterms[i]][table[i].size() - 1] = 1;
    return table;
}

struct Implicant {
    string term;
    vector<int> minterms;
    bool checked = false;
};

// convert decimal to binary
string decToBin(int n) {
    string r = "";
    while (n > 0) {
        r+= to_string(n % 2);
        n /= 2;
    }
    reverse(r.begin(), r.end());
    return r;
}

// make all binary values of equal length (add leading zeroee)
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

vector<int> getMintermsAsInt(const vector<vector<bool>>& table){
    vector<int> minterms;
    for (int i = 0; i < table.size(); i++)
        if (table[i].back() == 1)
            minterms.push_back(i);
    return minterms;
}

// create column 1 of the table (based on # of 1's)
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

// merge minterms of combined implicants
vector<int> merge(vector<int> a, vector<int> b) {
    vector<int> c;
    for (int i = 0; i < a.size(); i++) c.push_back(a[i]);
    for (int i = 0; i < b.size(); i++) c.push_back(b[i]);
    return c;
}

// removes any redundant combined implicants (optimization)
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

// compares every element to all elements one group below it
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

// manages each newly created column and checks whether it was combined or not
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

void Handlinginput(){
    bool test1 = 1;
    bool test2 = 1;
    string Term;
    string ex;
    vector<int> minterms;
    vector<int> donotCare;
    vector<vector<bool>> table;
    int temp;
    int size;
    int size2;
    while (test1)
    {
        test1 = 1;
        test2 = 1;
        cout << "Please enter your input type: " << endl;
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
            cout << "Please enter your expression: ";
            cin >> ex;
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
            cout << "5. All EPIs" << endl;
            cout << "6. All implicants that are not covered by EPIs" << endl;
            cout << "7. Minimized Function" << endl;
            cout << "8. Start from beginning" << endl;
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
                    table = truthTable(Term.size());
                    output(table, ex);
                    Print(table, Term);
                }
                else if (input_type == 2)
                {
                    table = fillTableWithMinterms(minterms);
                    Print(table, "");
                }
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:

                break;
            case 6:

                break;
            case 7:
                // Compute all implicants that are not covered with EPI
                // Call your implicants function with the appropriate input
                break;
            case 8:
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

int main(){
    
    Handlinginput();
    return 0;
}
