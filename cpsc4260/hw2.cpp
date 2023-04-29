#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <unordered_set>

using namespace std;

const int LONG_FUNCTION_LENGTH = 15;
const int LONG_PARAMETER_LIST_COUNT = 3;

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

vector<function> getFunctions(vector<string> lines) {
    vector<function> funcList;
    function func;
    unordered_set<string> funcSet;
    for (long unsigned int i = 0; i < lines.size(); i++) {
        string line = lines[i];
        string::size_type loc = line.find('(');
        if (loc != string::npos && line[line.size() - 1] != ';') {
            string possibleFuncName = line.substr(0, loc);
            loc = line.find(' ');
            if (loc != string::npos && loc != 0) {
                string funcName = possibleFuncName.substr(loc + 1);
                if (funcSet.count(funcName) == 0) {
                    func.name = funcName;
                    func.start = i;
                    funcSet.insert(funcName);
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
        int length = funcList[i].end - funcList[i].start;
        if (length > LONG_FUNCTION_LENGTH) {
            cout << funcList[i].name << " is a long function. It contains "
                 << length << " lines of code." << endl;
            longFuncCount++;
        }
    }
    // int maxLines = (int)lines.size();
    // for (int i = 0; i < funcCount; i++) {
    //     int lineCount = 1;
    //     int lineIndex = funcList[i].second;
    //     string currentLine = lines[lineIndex];
    //     while (currentLine != "}" && lineIndex < maxLines) {
    //         if (!currentLine.empty()) {
    //             lineCount++;
    //         }
    //         lineIndex++;
    //         currentLine = lines[lineIndex];
    //     }
    //     if (lineCount > LONG_FUNCTION_LENGTH) {
    //         cout << funcList[i].first << " is a long function. It contains "
    //              << lineCount << " lines of code." << endl;
    //         longFuncCount++;
    //     }
    // }

    if (longFuncCount == 0)
        cout << "There are no long functions." << endl;
}

void longParameterList(vector<function> funcList, vector<string> lines) {
    cout << "long parameter list" << endl;
    // int longParamListCount = 0;
    // int funcCount = (int)funcList.size();
    // for (int i = 0; i < funcCount; i++) {
    //     int parameterCount = 0;
    //     int funcLoc = funcList[i].second;
    //     string function = lines[funcLoc];
    //     string::size_type start = function.find('(');
    //     string::size_type end = function.find(')');
    //     if (start != string::npos && end != string::npos) {
    //         string paramList = function.substr(start + 1, end);
    //         stringstream paramStream;
    //         paramStream.str(paramList);
    //         string temp;
    //         while (paramStream >> temp) {
    //             if (isalpha(temp[0]))
    //                 parameterCount++;
    //         }
    //         // ??????            
    //         parameterCount = (parameterCount / 2) - 1;
    //         if (parameterCount > LONG_PARAMETER_LIST_COUNT) {
    //             cout << funcList[i].first << " has a long parameter list. Its "
    //                  << "parameter list contains " << parameterCount
    //                  << " parameters." << endl;
    //             longParamListCount++;
    //         }
    //     }
    // }
    // if (longParamListCount == 0)
    //     cout << "No function has a long parameter list." << endl;
}

void duplicateCode() {
    cout << "duplicate code" << endl;
}