#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>
#include <unordered_set>


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

inline vector<Result> dijkstraFindNearestInterestpoints(vector<Node>& nodes, const int fromNode, const vector<int>& toNodes, const int x) {
    vector<int> driveTimes(nodes.size(), numeric_limits<int>::max());
    vector<int> prev(nodes.size(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> queue(compare);

    int nodesVisited = 0;
    driveTimes[fromNode] = 0;
    queue.emplace(0, fromNode);

    unordered_set<int> toNodesSet(toNodes.begin(), toNodes.end());
    vector<Result> nearestResults;

    while (!queue.empty() && nearestResults.size() < x) {
        const int currentTime = queue.top().first;
        const int currentNode = queue.top().second;
        nodesVisited++;
        queue.pop();

        if (toNodesSet.count(currentNode)) {
            vector<int> path;
            for (int i = currentNode; i != -1; i = prev[i]) {
                path.push_back(i);
            }
            reverse(path.begin(), path.end());

            if (path.front() == fromNode) {
                nearestResults.emplace_back(path, driveTimes[currentNode]);
                toNodesSet.erase(currentNode);
            }

            if (nearestResults.size() >= x) {
                break;
            }
        }

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

    sort(nearestResults.begin(), nearestResults.end(), [](const Result& a, const Result& b) {
        return a.time < b.time;
    });
    return nearestResults;
}
