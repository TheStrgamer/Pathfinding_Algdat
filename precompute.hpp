#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <chrono>


using namespace std;
inline void precomputeDriveTimesFrom(const vector<Node>& nodes, const vector<int>& interestPoints, const string& outfilepathFrom) {
    const auto start = chrono::high_resolution_clock::now();
    cout << "Precomputing drive times from node" << endl;

    ofstream file(outfilepathFrom);
    if (!file.is_open()) {
        cerr << "Error opening file: " << outfilepathFrom << endl;
        return;
    }

    for (int landmark : interestPoints) {
        cout << "Calculating for landmark: " << landmark << endl;
        vector<long long> driveTimes(nodes.size(), numeric_limits<long long>::max());

        driveTimes[landmark] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);
        queue.emplace(0, landmark);

        while (!queue.empty()) {
            const long long currentTime = queue.top().first;
            const int currentNode = queue.top().second;
            queue.pop();

            for (const Edge& edge : nodes[currentNode].edges) {
                int neighbor = edge.endNode;
                long long newTime = currentTime + edge.driveTime;

                if (newTime < driveTimes[neighbor]) {
                    driveTimes[neighbor] = newTime;
                    queue.emplace(newTime, neighbor);
                }
            }
        }
        file << landmark;
        for (const long long time : driveTimes) {
            file << " " << time;
        }
        file << "\n";
    }

    const auto end = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Precomputation completed in " << duration.count() << " milliseconds" << endl;
    file.close();
}

inline void precomputeDriveTimesTo(const vector<Node>& nodes, const vector<int>& interestPoints, const string& outfilepathTo) {
    const auto start = chrono::high_resolution_clock::now();
    cout << "Precomputing drive times to node" << endl;

    ofstream file(outfilepathTo);
    if (!file.is_open()) {
        cerr << "Error opening file: " << outfilepathTo << endl;
        return;
    }

    for (int landmark : interestPoints) {
        cout << "Calculating for landmark: " << landmark << endl;
        vector<long long> driveTimes(nodes.size(), numeric_limits<long long>::max());

        driveTimes[landmark] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);
        queue.emplace(0, landmark);

        while (!queue.empty()) {
            //Todo
        }
        file << landmark;
        for (const long long time : driveTimes) {
            file << " " << time;
        }
        file << "\n";
    }

    const auto end = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Precomputation completed in " << duration.count() << " milliseconds" << endl;
    file.close();
}


inline int loadPrecomputedData(const string& filepath, int landmarkCount, int nodeCount, vector<Node>& nodes) {

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening precomputed data file: " << filepath << endl;
        return 1;
    }
    string line;
    int landmarkIndex = 0;
    while (getline(file, line) && landmarkIndex < landmarkCount) {
        stringstream ss(line);
        int landmark;
        ss >> landmark;

        for (int i = 0; i < nodeCount; ++i) {
            long long time;
            if (ss >> time) {
                nodes[i].addPrecomputedData(landmark, time);
            } else {
                nodes[i].addPrecomputedData(landmark, numeric_limits<long long>::max());
            }
        }
        landmarkIndex++;
    }

    file.close();
    return 0;
}


inline bool verifyPrecomputedDataExist(const string& precomputedDataFile) {
    ifstream file(precomputedDataFile);
    if (!file.is_open()) {
        cerr << "Error opening precomputed data file: " << precomputedDataFile << endl;
        return false;
    }
    return true;
}