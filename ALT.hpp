#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <cmath>

using namespace std;

auto alt_compare = [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first > b.first;
};

inline int calculateALTHeuristic(const int currentNode, const int toNode, const vector<int>& landmarks,
                                 const vector<vector<int>>& fromLandmarks, const vector<vector<int>>& toLandmarks) {
    int maxHeuristic = 0;

    for (int i = 0; i < landmarks.size(); i++) {
        const int lToTarget = fromLandmarks[i][toNode];
        const int lToCurrent = fromLandmarks[i][currentNode];
        const int currentToL = toLandmarks[i][currentNode];
        const int targetToL = toLandmarks[i][toNode];

        int estimate1 = max(0, lToTarget - lToCurrent);
        int estimate2 = currentToL - targetToL;

        int landmarkHeuristic = max(estimate1, estimate2);
        maxHeuristic = max(maxHeuristic, landmarkHeuristic);
    }

    return maxHeuristic;
}

inline Result altShortestPath(vector<Node>& nodes, const int fromNode, const int toNode,
                              const vector<int>& landmarks, const vector<vector<int>>& fromLandmarks,
                              const vector<vector<int>>& toLandmarks) {
    const auto start = chrono::high_resolution_clock::now();

    vector<int> driveTimes(nodes.size(), numeric_limits<int>::max());
    vector<int> prev(nodes.size(), -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(alt_compare)> queue(alt_compare);

    driveTimes[fromNode] = 0;
    queue.emplace(0, fromNode);
    int nodesVisited = 0;

    while (!queue.empty()) {
        const int currentNode = queue.top().second;
        const int currentTime = driveTimes[currentNode];
        nodesVisited++;
        queue.pop();

        if (currentNode == toNode) break;

        for (const Edge& edge : nodes[currentNode].edges) {
            int neighbor = edge.endNode;
            const int newTime = currentTime + edge.driveTime;

            if (newTime < driveTimes[neighbor]) {
                driveTimes[neighbor] = newTime;
                prev[neighbor] = currentNode;

                int estimatedCost = newTime + calculateALTHeuristic(neighbor, toNode, landmarks, fromLandmarks, toLandmarks);
                queue.emplace(estimatedCost, neighbor);
            }
        }
    }

    vector<int> path;
    unordered_set<int> visitedSet;
    for (int i = toNode; i != -1; i = prev[i]) {
        if (visitedSet.count(i)) {
            cout << "Loop detected in path reconstruction at node: " << i << endl;
            return Result({}, -1);
        }
        visitedSet.insert(i);
        path.push_back(i);
    }

    reverse(path.begin(), path.end());
    if (path.front() != fromNode) {
        return Result({}, -1);
    }

    const auto end = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << duration.count() << "ms\t\t" << nodesVisited << "\t\t";

    return Result(path, driveTimes[toNode]);
}
