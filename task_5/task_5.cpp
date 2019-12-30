#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Event {
public:
    bool isFound;
    uint64_t line;
    uint64_t column;

    bool operator==(const Event &other) {
        return isFound == other.isFound and
               line == other.line and
               column == other.column;
    }
};

Event findPatternOneThread(ifstream &input, string &pattern) {
    string line;
    uint64_t i = 0;
    while(getline(input, line)) {
        uint64_t found = line.find(pattern);
        if (found != string::npos) {
            return Event{true, i, found};
        }
        ++i;
    }
    return Event{false, 0, 0};
}

Event findPatternParallel(ifstream &input, string &pattern) {
    vector<string> lines;
    string line;
    while (getline(input, line)) lines.emplace_back(line);
    input.clear();
    input.seekg(0, ios::beg);
    Event result{false, 0, 0};

#pragma omp parallel for schedule(auto)
    for (uint64_t i = 0; i < lines.size(); ++i) {
        if (result.isFound) continue;

        uint64_t found = lines[i].find(pattern);
        if (found != string::npos) {
            #pragma omp critical
            if (!result.isFound) {
                result = Event{true, i, found};
            }
        }
    }
    return result;
}

int main() {
    ifstream input("input.txt");

    string pattern = "hello";

    Event res1 = findPatternOneThread(input, pattern);
    input.seekg(0, input.beg);
    Event res2 = findPatternParallel(input, pattern);
    if (res1 == res2) {
        if (res1.isFound) {
            cout << "found - " << res1.line << ":" << res1.column << endl;
        }
        else {
            cout << "not found" << endl;
        }
    }
    else {
        cout << "RESULTS DON'T MATCH" << endl;
        cout << res1.line << ":" << res1.column << endl;
        cout << res2.line << ":" << res2.column << endl;

    }

    return 0;
}