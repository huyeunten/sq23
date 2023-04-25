#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <unordered_set>

using namespace std;

const int LONG_FUNCTION_LENGTH = 15;
const int LONG_PARAMETER_LIST_COUNT = 3;

int menu();
vector<pair<string, int>> getFunctions(vector<string> lines);
void longFunction(vector<pair<string, int>> funcList, vector<string> lines);
void longParameterList(vector<pair<string, int>> funcList, vector<string> lines);
void duplicateCode();

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./hw2 file_name.cpp" << endl;
        return -1;
    }

    string file = argv[1];
    ifstream inputFile;
    inputFile.open(file);
    if (inputFile.fail()) {
        cout << "File failed to open" << endl;
        return -1;
    }

    vector<string> lines;
    string line;

    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    vector<pair<string, int>> funcList = getFunctions(lines);
    int funcCount = (int)funcList.size();

    cout << "Welcome to Code Smell Detection! The input file contains the "
         << "following functions: ";

    for (int i = 0; i < funcCount; i++) {
        if (i == funcCount - 1)
            cout << funcList[i].first << "." << endl;
        else 
            cout << funcList[i].first << ", ";
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
                duplicateCode();
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
    cin >> input;
    while (!(input > 0 && input < 5)) {
        cout << "Invalid choice. Please enter a number from 1 to 4: ";
        cin >> input;
    }
    return input;
}

vector<pair<string, int>> getFunctions(vector<string> lines) {
    vector<pair<string, int>> funcList;
    unordered_set<string> funcSet;
    for (long unsigned int i = 0; i < lines.size(); i++) {
        string line = lines[i];
        string::size_type loc = line.find('(');
        if (loc != string::npos && line[line.size() - 1] != ';') {
            string possibleFunc = line.substr(0, loc);
            loc = line.find(' ');
            if (loc != string::npos && loc != 0) {
                string funcName = possibleFunc.substr(loc + 1);
                if (funcSet.count(funcName) == 0) {
                    pair<string, int> funcLinePair = make_pair(funcName, i);
                    funcList.push_back(funcLinePair);
                    funcSet.insert(funcName);
                }
            }

        }
    }
    return funcList;
}

void longFunction(vector<pair<string, int>> funcList, vector<string> lines) {
    int longFuncCount = 0;
    int funcCount = (int)funcList.size();
    int maxLines = (int)lines.size();
    for (int i = 0; i < funcCount; i++) {
        int lineCount = 1;
        int lineIndex = funcList[i].second;
        string currentLine = lines[lineIndex];
        while (currentLine != "}" && lineIndex < maxLines) {
            if (!currentLine.empty()) {
                lineCount++;
            }
            lineIndex++;
            currentLine = lines[lineIndex];
        }
        if (lineCount > LONG_FUNCTION_LENGTH) {
            cout << funcList[i].first << " is a long function. It contains "
                 << lineCount << " lines of code." << endl;
            longFuncCount++;
        }
    }

    if (longFuncCount == 0)
        cout << "There are no long functions." << endl;
}

void longParameterList(vector<pair<string, int>> funcList, vector<string> lines) {
    int longParamListCount = 0;
    int funcCount = (int)funcList.size();
    for (int i = 0; i < funcCount; i++) {
        int parameterCount = 0;
        int funcLoc = funcList[i].second;
        string function = lines[funcLoc];
        string::size_type start = function.find('(');
        string::size_type end = function.find(')');
        if (start != string::npos && end != string::npos) {
            string paramList = function.substr(start, end);
            stringstream paramStream;
            paramStream.str(paramList);
            string temp;
            while (paramStream >> temp) {
                if (isalpha(temp[0]))
                    parameterCount++;
            }
            // ??????            
            parameterCount = (parameterCount / 2) + 1;
            if (parameterCount > LONG_PARAMETER_LIST_COUNT) {
                cout << funcList[i].first << " has a long parameter list. Its "
                     << "parameter list contains " << parameterCount
                     << " parameters." << endl;
                longParamListCount++;
            }
        }
    }
    if (longParamListCount == 0)
        cout << "No function has a long parameter list." << endl;
}

void duplicateCode() {
    cout << "duplicate code" << endl;
}