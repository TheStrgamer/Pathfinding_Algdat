#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>


using namespace std;
struct Result {
    vector<int> path;
    int time;
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

auto compare = [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first > b.first;
};

inline Result dijkstraShortestPath(vector<Node>& nodes, const int fromNode, const int toNode) {
    const auto start = chrono::high_resolution_clock::now();

    vector<int> driveTimes(nodes.size(), numeric_limits<int>::max());
    vector<int> prev(nodes.size(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> queue(compare);

    //cout << endl << "Calculating shortest path from " << fromNode << " to " << toNode << endl;
    int nodesVisited = 0;

    driveTimes[fromNode] = 0;
    queue.emplace(0, fromNode);

    while (!queue.empty()) {
        const int currentTime = queue.top().first;
        const int currentNode = queue.top().second;
        nodesVisited++;
        queue.pop();

        if (currentNode == toNode) break;

        for (const Edge& edge : nodes[currentNode].edges) {
            int neighbor = edge.endNode;
            int newTime = currentTime + edge.driveTime;

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

    cout << duration.count() << "ms\t\t" << nodesVisited << "\t\t";;

    return Result(path, driveTimes[toNode]);
}