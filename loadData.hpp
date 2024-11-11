#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>

using namespace std;


class Edge {
public:
    int startNode;
    int endNode;
    int driveTime;

    Edge() {}
    Edge(const int startNode, const int endNode, const int driveTime) {
        this->startNode = startNode;
        this->endNode = endNode;
        this->driveTime = driveTime;
    }
};

class Node {
public:
    int num;
    vector<Edge> edges;

    Node() {}
    Node(const int num) {
        this->num = num;
    }

    void addEdge(const Edge& edge) {
        edges.push_back(edge);
    }
};

unordered_map<int, Node> readNodes(const string& filename) {
    cout << "Reading nodes from file: " << filename << endl;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    int nodeCount;
    file >> nodeCount;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    unordered_map<int, Node> nodes;

    int num;
    float longitude, latitude;
    for (int i = 0; i < nodeCount; i++) {
        if (!(file >> num >> latitude >> longitude)) {
            cerr << "Error reading node data at line " << i + 2 << endl;
            break;
        }
        nodes[num] = Node(num);
    }
    cout << nodes.size() << " Nodes added" << endl;
    return nodes;
}

vector<Edge> readEdges(const string& filename, unordered_map<int, Node>& nodes) {
    cout << "Reading edges from file: " << filename << endl;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    int edgeCount;
    file >> edgeCount;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<Edge> edges;

    int start, end, driveTime;
    int dist, speed;
    for (int i = 0; i < edgeCount; i++) {
        if (!(file >> start >> end >> driveTime >> dist >> speed)) {
            cerr << "Error reading edge data at line " << i + 2 << endl;
            break;
        }
        if (nodes.find(start) != nodes.end() && nodes.find(end) != nodes.end()) {
            Edge edge(start, end, driveTime);
            nodes[start].addEdge(edge);
            edges.push_back(edge);
        }
    }
    cout << edges.size() << " Edges added" << endl;
    return edges;
}

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

vector<int> getTestCase(const int testNumber) {
  switch (testNumber) {
    case 1: return {2800567, 7705656};  // Kårvåg–Gjemnes
    case 2: return {7705656, 2800567};  // Gjemnes–Kårvåg
    case 3: return {647826, 136530};    // Malmö Brygghus–Nørrebro
    case 4: return {136530, 647826};    // Nørrebro–Malmö Brygghus
    case 5: return {7826348, 2948202};  // Trondheim–Oslo
    case 6: return {2948202, 7826348};  // Oslo–Trondheim
    case 7: return {339910, 1853145};   // Jyväskylä–Kárášjohka
    case 8: return {1853145, 339910};   // Kárášjohka–Jyväskylä
    case 9: return {2503331, 2866570};  // Gol–Lakselv Hotell
    case 10: return {2866570, 2503331}; // Lakselv Hotell–Gol
    case 11: return {6441311, 3168086}; // Esso Sauda–Tervo
    case 12: return {3168086, 6441311}; // Tervo–Esso Sauda
    case 13: return {2447358, 3516217}; // Askim-Bjørkelangen
    default:
      cerr << "Invalid test number. Returning default case." << endl;
    return {0, 0};
  }
}