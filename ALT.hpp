#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <queue>

#include "loadData.hpp"
#include "precompute.hpp"



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

Result ALT_shortest_path(const vector<Node>& nodes, const int fromNode, const int toNode) {
  const auto setup_start = chrono::high_resolution_clock::now();


  vector<long long> driveTimes;
  vector<int> prev;
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, decltype(compare)> queue(compare);

  for (int i = 0; i < nodes.size(); i++) {
    driveTimes.push_back(numeric_limits<long long>::max());
    prev.push_back(-1);
  }
  const auto pathfinding_start = chrono::high_resolution_clock::now();


}