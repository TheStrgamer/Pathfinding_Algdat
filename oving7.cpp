#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "loadData.hpp"
#include "dijkstra.hpp"
#include "precompute.hpp"

using namespace std;

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
