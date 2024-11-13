#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>


using namespace std;
struct Result {
    vector<int> path;
    long long time;
    int hours;
    int minutes;
    int seconds;
    Result(const vector<int> &path, const int time) {
        this->path = path;
        this->time = time;
        this->hours = (time/100) / 3600;
        this->minutes = ((time/100) % 3600) / 60;
        this->seconds = (time/100) % 60;
    }
};

auto compare = [](const pair<long long, int>& a, const pair<long long, int>& b) {
    return a.first > b.first;
};

inline Result dijkstraShortestPath(vector<Node>& nodes, const int fromNode, const int toNode) {
    const auto setup_start = chrono::high_resolution_clock::now();

    vector<long long> driveTimes(nodes.size(), numeric_limits<long long>::max());
    vector<int> prev(nodes.size(), -1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);

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