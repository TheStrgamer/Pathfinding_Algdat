#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm>

using namespace std;

inline void precomputeDriveTimesFrom(const vector<Node>& nodes, const vector<int>& interestPoints, const string& outfilepathFrom) {
    ofstream file(outfilepathFrom, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << outfilepathFrom << endl;
        return;
    }

    cout << "Calculating for landmark: ";
    for (int landmark : interestPoints) {
        cout << landmark << " ";
        vector<int> driveTimes(nodes.size(), numeric_limits<int>::max());
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> queue(compare);

        driveTimes[landmark] = 0;
        queue.emplace(0, landmark);
        int nodesVisited = 0;

        while (!queue.empty()) {
            const int currentTime = queue.top().first;
            const int currentNode = queue.top().second;
            queue.pop();
            nodesVisited++;

            for (const Edge& edge : nodes[currentNode].edges) {
                int neighbor = edge.endNode;
                int newTime = currentTime + edge.driveTime;

                if (newTime < driveTimes[neighbor]) {
                    driveTimes[neighbor] = newTime;
                    queue.emplace(newTime, neighbor);
                }
            }
        }
        cout << " visited: " << nodesVisited << " " ;

        file.write(reinterpret_cast<const char*>(&landmark), sizeof(landmark));
        for (int32_t time : driveTimes) {
            file.write(reinterpret_cast<const char*>(&time), sizeof(time));
        }
    }

    file.close();
    cout << "Precomputation completed.\n";
}


inline int loadPrecomputedData(const string& filepathFrom, const string& filepathTo, const int landmarkCount, const int nodeCount,
                               vector<vector<int>>& fromLandmarks, vector<vector<int>>& toLandmarks) {
    ifstream fileFrom(filepathFrom, ios::in | ios::binary);
    ifstream fileTo(filepathTo, ios::in | ios::binary);

    if (!fileFrom.is_open() || !fileTo.is_open()) {
        cerr << "Error opening precomputed data files" << endl;
        return 1;
    }

    fromLandmarks.resize(landmarkCount, vector<int>(nodeCount, numeric_limits<int>::max()));
    toLandmarks.resize(landmarkCount, vector<int>(nodeCount, numeric_limits<int>::max()));

    for (int i = 0; i < landmarkCount; i++) {
        int landmark;
        fileFrom.read(reinterpret_cast<char*>(&landmark), sizeof(landmark));
        for (int j = 0; j < nodeCount; j++) {
            int time;
            fileFrom.read(reinterpret_cast<char*>(&time), sizeof(time));
            fromLandmarks[i][j] = time;
        }

        fileTo.read(reinterpret_cast<char*>(&landmark), sizeof(landmark));
        for (int j = 0; j < nodeCount; j++) {
            int time;
            fileTo.read(reinterpret_cast<char*>(&time), sizeof(time));
            toLandmarks[i][j] = time;
        }
    }
    fileFrom.close();
    fileTo.close();

    return 0;
}

inline void precomputeDriveTimesTo(const vector<Node>& nodesReversed, const vector<int>& interestPoints, const string& outfilepathTo) {
    precomputeDriveTimesFrom(nodesReversed, interestPoints, outfilepathTo);
}
