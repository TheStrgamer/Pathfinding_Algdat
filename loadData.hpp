#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>
#include <sstream>

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
vector<Node> readNodes(const string& filename) {
    const auto start_time = chrono::high_resolution_clock::now();
    cout << "Reading nodes from file: " << filename << endl;

    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    int nodeCount;
    file >> nodeCount;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<Node> nodes(nodeCount);

    int num;
    float longitude, latitude;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        ss >> num >> latitude >> longitude;
        if (num >= nodes.size()) {
            nodes.resize(num + 1);
        }
        nodes[num] = Node(num);
    }

    const auto end_time = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Read " << nodes.size() << " nodes" << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    return nodes;
}

vector<Edge> readEdges(const string& filename, vector<Node>& nodes) {
    const auto start_time = chrono::high_resolution_clock::now();
    cout << "Reading edges from file: " << filename << endl;

    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    int edgeCount;
    file >> edgeCount;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<Edge> edges;
    edges.reserve(edgeCount);

    string line;
    int start, end, driveTime, dist, speed;
    while (getline(file, line)) {
        stringstream ss(line);
        ss >> start >> end >> driveTime >> dist >> speed;
        Edge edge(start, end, driveTime);
        nodes[start].addEdge(edge);
        edges.push_back(edge);
    }

    cout << edges.size() << " edges added" << endl;
    const auto end_time = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
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

vector<int> getTestCaseIsland(const int testNumber) {
    switch (testNumber) {
        case 1:  return {0, 112776};
        case 2:  return {10234, 54321};
        case 3:  return {43210, 8765};
        case 4:  return {12345, 67890};
        case 5:  return {6789, 9876};
        case 6:  return {13579, 24680};
        case 7:  return {11111, 22222};
        case 8:  return {33333, 44444};
        case 9:  return {55555, 66666};
        case 10: return {77777, 88888};
        case 11: return {99999, 112776};
        case 12: return {11000, 22000};
        case 13: return {30000, 45000};
        default:
            cerr << "Invalid test number. Returning default case." << endl;
        return {0, 0};
    }
}