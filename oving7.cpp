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

Result dijkstraShortestPath(unordered_map<int, Node>& nodes, const int fromNode, const int toNode) {
    const auto setup_start = chrono::high_resolution_clock::now();

    unordered_map<int, long long> driveTimes;
    unordered_map<int, int> prev;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);

    for (const auto& [num, node] : nodes) {
        driveTimes[num] = numeric_limits<long long>::max();
        prev[num] = -1;
    }

    cout << endl << "Calculating shortest path from " << fromNode << " to " << toNode << endl;
    int nodesVisited = 0;
    const auto start = chrono::high_resolution_clock::now();

    driveTimes[fromNode] = 0;
    queue.emplace(0, fromNode);

    while (!queue.empty()) {
        const long long currentTime = queue.top().first;
        int currentNode = queue.top().second;
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

int main() {
    const string nodesFile = "noder.txt";
    const string edgesFile = "kanter.txt";

    unordered_map<int, Node> nodes = readNodes(nodesFile);
    if (nodes.empty()) {
        return 1;
    }
    const vector<Edge> edges = readEdges(edgesFile, nodes);
    if (edges.empty()) {
        return 1;
    }

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
