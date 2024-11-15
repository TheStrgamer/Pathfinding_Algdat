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

void exportPath(const vector<int>& path, const vector<Node>& nodes, const string& filename = "path.txt") {
    ofstream file(filename, ios::out);
    if (!file.is_open()) {
        cerr << "Error opening file: path.txt" << endl;
        return;
    }
    for (int node : path) {
        file << node << " " << nodes[node].longitude << " " << nodes[node].latitude << endl;
    }
    file.close();
}

void exportPathVector(const vector<Result>& results, const vector<Node>& nodes, const string& filename = "path.txt") {
    ofstream file(filename, ios::out);
    if (!file.is_open()) {
        cerr << "Error opening file: path.txt" << endl;
        return;
    }
    for (const Result& result : results) {
        for (const int node : result.path) {
            file << node << " " << nodes[node].longitude << " " << nodes[node].latitude << endl;
        }
        file << endl;
    }
    file.close();
}

void printResults(const vector<Result>& results) {
    for (const Result& result : results) {
        const int node = result.path.back();
        cout << "Node " << node  << "\tDrive time: " << result.hours << ":" << result.minutes << ":" << result.seconds << endl;
    }
}
vector<int> findPointsOfInterestByType(const vector<PointOfInterest>& points, string type) {
    int code;
    if (type == "Stedsnavn") {
        code = 1;
    } else if (type == "Bensinstasjon") {
        code = 2;
    } else if (type == "Ladestasjon") {
        code = 4;
    } else if (type == "Spisested") {
        code = 8;
    } else if (type == "Drikkested") {
        code = 16;
    } else if (type == "Overnattingssted") {
        code = 32;
    } else {
        return {};
    }

    vector<int> filteredPoints;
    for (const PointOfInterest& point : points) {
        if (point.type & code) {
            filteredPoints.push_back(point.num);
        }
    }
    return filteredPoints;
}

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

    bool exportPaths = true; // Set to true to export paths to txt files to view with pathViewer.html

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
        if (exportPaths) {
            string filename = "Path_ALTPath" + to_string(i) + ".txt";
            exportPath(result.path, nodes, filename);
        }
        cout << "Dijkstra\t" << fromNode << "\t\t" << toNode << "\t\t";
        result = dijkstraShortestPath(nodes, fromNode, toNode);
        if (result.time == -1) {
            cout << "No path found" << endl;
        } else {
            cout << result.path.size() << "\t\t";
            cout  << result.hours << ":" << result.minutes << ":" << result.seconds << endl;
        }
        if (exportPaths) {
            string filename = "Path_DijkstraPath" + to_string(i) + ".txt";
            exportPath(result.path, nodes, filename);
        }

        cout << "---------------------------------------------------------------------------------------------------------" << endl;
    }
    const vector<PointOfInterest> points = readPointsOfInterest("interessepkt.txt");

    cout << endl << "Finding 4 charging stations near Levanger" << endl;
    if (points.empty()) {return 1;}
    vector<int> goalPoints = findPointsOfInterestByType(points, "Ladestasjon");
    int Levanger = 2106148;
    const vector<Result> nearestChargingStations = dijkstraFindNearestInterestpoints(nodes,Levanger, goalPoints, 4);
    printResults(nearestChargingStations);


    cout << endl << "Finding 4 drinking places near Gløshaugen" << endl;
    goalPoints = findPointsOfInterestByType(points, "Drikkested");
    int Gløshaugen = 2001238;
    const vector<Result> nearestDrinkingPlaces = dijkstraFindNearestInterestpoints(nodes, Gløshaugen, goalPoints, 4);
    printResults(nearestDrinkingPlaces);

    cout << endl << "Finding 4 eating places near Åre Björnen" << endl;
    int AreBjornen = 790843;
    goalPoints = findPointsOfInterestByType(points, "Spisested");
    const vector<Result> nearestEatingPlaces = dijkstraFindNearestInterestpoints(nodes, AreBjornen, goalPoints, 4);
    printResults(nearestEatingPlaces);

    if (exportPaths) {
        string filename = "Path_ChargingStationLevanger.txt";
        exportPathVector(nearestChargingStations, nodes, filename);

        filename = "Path_DrinkingPlaceGloshaugen.txt";
        exportPathVector(nearestDrinkingPlaces, nodes, filename);

        filename = "Path_EatingPlacePathsAreBjornen.txt";
        exportPathVector(nearestEatingPlaces, nodes, filename);
    }

    return 0;
}
