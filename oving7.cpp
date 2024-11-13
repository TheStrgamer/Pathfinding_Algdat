#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>
#include "loadData.hpp"

using namespace std;

auto compare = [](const pair<long long, int>& a, const pair<long long, int>& b) {
    return a.first > b.first;
};

Result dijkstraShortestPath(vector<Node>& nodes, const int fromNode, const int toNode) {
    const auto setup_start = chrono::high_resolution_clock::now();

    vector<long long> driveTimes;
    vector<int> prev;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);

    for (int i = 0; i < nodes.size(); i++) {
        driveTimes.push_back(numeric_limits<long long>::max());
        prev.push_back(-1);
    }

    cout << endl << "Calculating shortest path from " << fromNode << " to " << toNode << endl;
    int nodesVisited = 0;
    const auto start = chrono::high_resolution_clock::now();

    driveTimes[fromNode] = 0;
    queue.emplace(0, fromNode);

    while (!queue.empty()) {
        const long long currentTime = queue.top().first;
        const int currentNode = queue.top().second;
        nodesVisited++;
        queue.pop();

        if (currentNode == toNode) break;

        for (const Edge& edge : nodes[currentNode].edges) {
            int neighbor = edge.endNode;
            long long newTime = currentTime + edge.driveTime;

            if (newTime < driveTimes[neighbor]) {
                driveTimes[neighbor] = newTime;
                prev[neighbor] = currentNode;
                queue.emplace(newTime, neighbor);
            }
        }
    }

    vector<int> path;
    for (int i = toNode; i != -1; i = prev[i]) {
        path.push_back(i);
    }
    reverse(path.begin(), path.end());

    if (path.front() != fromNode) {
        return Result({}, -1);
    }

    const auto end = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    const auto setupDuration = chrono::duration_cast<chrono::milliseconds>(start - setup_start);
    cout << "Setup time: " << setupDuration.count() << " milliseconds" << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    cout << "Nodes visited: " << nodesVisited << endl;

    return Result(path, driveTimes[toNode]);
}


void precomputeDriveTimes(const vector<Node>& nodes, const vector<int>& interestPoints, const string& outfilepath) {
    const auto start = chrono::high_resolution_clock::now();
    cout << "Precomputing drive times" << endl;

    ofstream file(outfilepath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << outfilepath << endl;
        return;
    }
    for (int landmark : interestPoints) {
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

vector<vector<long long>> loadPrecomputedData(const string& filepath, int landmarkCount, int nodeCount) {
    vector<vector<long long>> precomputedData(landmarkCount, vector<long long>(nodeCount, numeric_limits<long long>::max()));

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening precomputed data file: " << filepath << endl;
        return precomputedData;
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
                precomputedData[landmarkIndex][i] = time;
            } else {
                precomputedData[landmarkIndex][i] = numeric_limits<long long>::max();
            }
        }
        landmarkIndex++;
    }

    file.close();
    return precomputedData;
}


int main() {
    const string nodesFile = "noder.txt";
    const string edgesFile = "kanter.txt";


    vector<Node> nodes = readNodes(nodesFile);
    if (nodes.empty()) {
        return 1;
    }
    const vector<Edge> edges = readEdges(edgesFile, nodes);
    if (edges.empty()) {
        return 1;
    }

    //precomputeDriveTimes(nodes, { 435,8777,12356 }, "driveTimes.txt");

    for (int i = 1; i <= 13; i++) {
        const vector<int> toAndFrom = getTestCase(i);
        const int fromNode = toAndFrom[0];
        const int toNode = toAndFrom[1];

        const Result result = dijkstraShortestPath(nodes, fromNode, toNode);
        if (result.time == -1) {
            cout << "No path found" << endl;
        } else {
            cout << "Path has " << result.path.size() << " nodes" << endl;
            cout << endl;
            cout << "Time: " << result.hours << " hours, " << result.minutes << " minutes, " << result.seconds << " seconds" << endl;
        }
        cout << endl;
    }

    return 0;
}
