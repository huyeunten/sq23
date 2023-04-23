#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>

using namespace std;

int menu();
vector<pair<string, int>> getFunctions(vector<string> lines);
void longFunction();
void longParameterList();
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

    for (long unsigned int i = 0; i < funcList.size(); i++) {
        cout << "function " << funcList[i].first << " at line " << funcList[i].second << endl;
    }

    int choice = 0;
    while (choice != 4) {
        choice = menu();
        switch(choice) {
            case 1:
                longFunction();
                break;
            case 2:
                longParameterList();
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
    for (long unsigned int i = 0; i < lines.size(); i++) {
        string line = lines[i];
        stringstream ss;
        ss.str(line);
        string temp;
        while (ss >> temp) {
            string::size_type loc = temp.find('(');
            if (loc != string::npos) {
                string funcName = temp.substr(0, loc);
                pair<string, int> funcLinePair = make_pair(funcName, i);
                funcList.push_back(funcLinePair);
            }
        }
    }
    return funcList;
}

void longFunction() {
    cout << "long function" << endl;
}

void longParameterList() {
    cout << "long parameter" << endl;
}

void duplicateCode() {
    cout << "duplicate code" << endl;
}