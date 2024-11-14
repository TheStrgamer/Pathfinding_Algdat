#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "loadData.hpp"
#include "dijkstra.hpp"
#include "precompute.hpp"
#include "ALT.hpp"

using namespace std;

bool verifyPrecomputedDataExists(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file.close();
    return true;
}

int main() {
    const string nodesFile = "noder.txt";
    const string edgesFile = "kanter.txt";

    vector<Node> nodes = readNodes(nodesFile);
    if (nodes.empty()) {return 1;}
    cout << endl;
    const vector<int> landmarks = { 1682,7900879,12356,880090};

    if (!verifyPrecomputedDataExists("driveTimesTo.dat") || !verifyPrecomputedDataExists("driveTimesFrom.dat") || false) {
        cout << "Precomputing drive times to landmarks" << endl;
        vector<Edge> edges = readEdges(edgesFile, nodes, true);
        if (edges.empty()) {return 1;}
        precomputeDriveTimesTo(nodes, landmarks, "driveTimesTo.dat");
        clearNodeEdges(nodes);

        cout << "Precomputing drive times from landmarks" << endl;
        edges = readEdges(edgesFile, nodes);
        if (edges.empty()) {return 1;}
        precomputeDriveTimesFrom(nodes, landmarks, "driveTimesFrom.dat");
    }
    else {
        const vector<Edge> edges = readEdges(edgesFile, nodes);
        if (edges.empty()) {
            return 1;
        }
    }
    cout << endl;

    vector<vector<int>> fromLandmarks;
    vector<vector<int>> toLandmarks;
    loadPrecomputedData("driveTimesFrom.dat", "driveTimesTo.dat", landmarks.size(), nodes.size(), fromLandmarks, toLandmarks);

    cout << endl << "Algorithm\tFrom node:\tTo node:\tCalculate time\tNodes visited\tNodes in path:\tDrive time:" << endl;
    for (int i = 1; i <= 13; i++) {
        const vector<int> toAndFrom = getTestCase(i);
        const int fromNode = toAndFrom[0];
        const int toNode = toAndFrom[1];

        cout << "ALT\t\t" << fromNode << "\t\t" << toNode << "\t\t";
        Result result = altShortestPath(nodes, fromNode, toNode, landmarks, fromLandmarks, toLandmarks);
        if (result.time == -1) {
            cout << "No path found" << endl;
        } else {
            cout << result.path.size() << "\t\t";
            cout  << result.hours << ":" << result.minutes << ":" << result.seconds << endl;
        }

        cout << "Dijkstra\t" << fromNode << "\t\t" << toNode << "\t\t";
        result = dijkstraShortestPath(nodes, fromNode, toNode);
        if (result.time == -1) {
            cout << "No path found" << endl;
        } else {
            cout << result.path.size() << "\t\t";
            cout  << result.hours << ":" << result.minutes << ":" << result.seconds << endl;
        }
        cout << "---------------------------------------------------------------------------------------------------------" << endl;
    }
    return 0;
}
