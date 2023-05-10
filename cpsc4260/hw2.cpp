#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <limits>

using namespace std;

const int LONG_FUNCTION_LENGTH = 15;
const int LONG_PARAMETER_LIST_COUNT = 3;
const double DUPLICATED_THRESHOLD = 0.75;

struct function {
    string name;
    int start;
    int end;
};

int menu();
vector<function> getFunctions(vector<string> lines);
void getFunctionLength(vector<function> &funcList, vector<string> lines);
void longFunction(vector<function> funcList, vector<string> lines);
void longParameterList(vector<function> funcList, vector<string> lines);
set<char> getFunctionSet(function func, vector<string> lines);
set<char> getIntersection(set<char> first, set<char> second);
set<char> getUnion(set<char> first, set<char> second);
void print(set<char> set);
void duplicateCode(vector<function> funcList, vector<string> lines);

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./hw2 file_name.cpp" << endl;
        exit(1);
    }

    string file = argv[1];
    ifstream inputFile;
    inputFile.open(file);
    if (inputFile.fail()) {
        cout << "File could not be opened" << endl;
        exit(1);
    }

    vector<string> lines;
    string line;

    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    vector<function> funcList = getFunctions(lines);
    getFunctionLength(funcList, lines);
    int funcCount = (int)funcList.size();

    cout << "Welcome to Code Smell Detection! The input file contains the "
         << "following functions: ";

    for (int i = 0; i < funcCount; i++) {
        if (i == funcCount - 1)
            cout << funcList[i].name << "." << endl;
        else 
            cout << funcList[i].name << ", ";
    }

    int choice = 0;
    while (choice != 4) {
        choice = menu();
        switch(choice) {
            case 1:
                longFunction(funcList, lines);
                break;
            case 2:
                longParameterList(funcList, lines);
                break;
            case 3:
                duplicateCode(funcList, lines);
                break;
            case 4:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
}

int menu() {
    cout << endl;
    cout << "Please choose what you want to do now:" << endl
         << "1. Long Method/Function Detection" << endl
         << "2. Long Parameter List Detection" << endl
         << "3. Duplicated Code Detection" << endl
         << "4. Quit" << endl;
    int input;
    bool validChoice = false;
    while (!validChoice) {
        cin >> input;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number from 1 to 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (input > 0 && input < 5)
            validChoice = true;
        else {
            cout << "Invalid number. Please enter a number from 1 to 4: ";
        }
    }
    cout << endl;
    return input;
}

vector<function> getFunctions(vector<string> lines) {
    vector<function> funcList;
    function func;
    unordered_set<string> funcSet;
    for (long unsigned int i = 0; i < lines.size(); i++) {
        string line = lines[i];
        string::size_type loc = line.find('(');
        if (loc != string::npos && line[line.size() - 1] != ';') {
            string possibleFuncName = line.substr(0, loc);
            string::size_type possibleCtor = line.find("::");
            loc = possibleFuncName.find(' ');
            if (loc != string::npos && loc != 0) {
                string funcName = possibleFuncName.substr(loc + 1);
                if (funcSet.count(funcName) == 0) {
                    func.name = funcName;
                    func.start = i;
                    funcSet.insert(funcName);
                    funcList.push_back(func);
                }
            }
            else if (possibleCtor != string::npos) {
                if (funcSet.count(possibleFuncName) == 0) {
                    func.name = possibleFuncName;
                    func.start = i;
                    funcSet.insert(possibleFuncName);
                    funcList.push_back(func);
                }
            }

        }
    }
    return funcList;
}

void getFunctionLength(vector<function> &funcList, vector<string> lines) {
    int funcCount = (int)funcList.size();
    string currentLine;
    for (int i = 0; i < funcCount; i++) {
        int count = 1;
        int lineIndex = funcList[i].start;
        int bracketCount = 0;
        bool foundEnd = false;
        while (!foundEnd) {
            currentLine = lines[lineIndex];
            if (!currentLine.empty()) {
                count++;
                string::size_type openBracket = currentLine.find('{');
                if (openBracket != string::npos)
                    bracketCount++;
                string::size_type closeBracket = currentLine.find('}');
                if (closeBracket != string::npos)
                    bracketCount--;
            }
            lineIndex++;
            if (bracketCount == 0)
                foundEnd = true;
        }
        funcList[i].end = lineIndex;
    }
}

void longFunction(vector<function> funcList, vector<string> lines) {
    int longFuncCount = 0;
    int funcCount = (int)funcList.size();
    for (int i = 0; i < funcCount; i++) {
        int length = 0;
        for (int j = funcList[i].start; j < funcList[i].end; j++) {
            if (!lines[j].empty())
                length++;
        }
        if (length > LONG_FUNCTION_LENGTH) {
            cout << funcList[i].name << " is a long function. It contains "
                 << length << " lines of code." << endl;
            longFuncCount++;
        }
    }
    if (longFuncCount == 0)
        cout << "There are no long functions." << endl;
}

void longParameterList(vector<function> funcList, vector<string> lines) {
    int longParamListCount = 0;
    int funcCount = (int)funcList.size();
    for (int i = 0; i < funcCount; i++) {
        int parameterCount = 0;
        int funcLoc = funcList[i].start;
        string function = lines[funcLoc];
        string::size_type start = function.find('(');
        string::size_type end = function.find(')');
        if (start != string::npos && end != string::npos) {
            string paramList = function.substr(start + 1, end - start);
            stringstream paramStream;
            paramStream.str(paramList);
            string temp;
            while (paramStream >> temp)
                parameterCount++;
            parameterCount /= 2;
            if (parameterCount > LONG_PARAMETER_LIST_COUNT) {
                cout << funcList[i].name << " has a long parameter list. Its "
                     << "parameter list contains " << parameterCount
                     << " parameters." << endl;
                longParamListCount++;
            }
        }
    }
    if (longParamListCount == 0)
        cout << "No function has a long parameter list." << endl;
}

set<char> getFunctionSet(function func, vector<string> lines) {
    set<char> funcChars;
    for (int i = func.start; i < func.end; i++) {
        string line = lines[i];
        for (int j = 0; j < (int)line.size(); j++) {
            char current = line[j];
            funcChars.insert(current);
        }
    }
    return funcChars;
}

set<char> getIntersection(set<char> first, set<char> second) {
    set<char> intersection;
    set_intersection(first.begin(), first.end(), second.begin(), second.end(),
                     inserter(intersection, intersection.begin()));
    return intersection;
}

set<char> getUnion(set<char> first, set<char> second) {
    set<char> unionSet;
    set_union(first.begin(), first.end(), second.begin(), second.end(),
              inserter(unionSet, unionSet.begin()));
    return unionSet;
}

void print(set<char> set) {
    for (auto const &i: set) {
        cout << i << " ";
    }
    cout << endl;
}

void duplicateCode(vector<function> funcList, vector<string> lines) {
    int similarCount = 0;
    vector<set<char>> functionSets;
    int funcCount = (int)funcList.size();
    for (int i = 0; i < funcCount; i++) {
        functionSets.push_back(getFunctionSet(funcList[i], lines));
    }
    for (int i = 0; i < funcCount; i++) {
        for (int j = i + 1; j < funcCount; j++) {
            set<char> intersection = getIntersection(functionSets[i], functionSets[j]);
            set<char> unionSet = getUnion(functionSets[i], functionSets[j]);
            double similarity = (double)intersection.size() / unionSet.size();
            if (similarity >= DUPLICATED_THRESHOLD) {
                cout << funcList[i].name << " and " << funcList[j].name
                     << " are duplicated. The Jaccard similarity is "
                     << similarity << "." << endl;
                similarCount++;
            }
        }
    }
    if (similarCount == 0) {
        cout << "No two functions are duplicated." << endl;
    }
}