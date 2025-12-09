#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

typedef struct vec3_s {
  int x, y, z;
} vec3;

struct Edge {
  int aIndex;
  int bIndex;
  double distance;
};
double norm(vec3 v1, vec3 v2) {
  long long dx = (long long)v2.x - v1.x;
  long long dy = (long long)v2.y - v1.y;
  long long dz = (long long)v2.z - v1.z;
  return sqrt(dx * dx + dy * dy + dz * dz);
}
// Yes I've reuse my split from day2 5 GOGOL times now what ?
vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  stringstream ss(s);
  string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

int main(void) {
  // Oe Oe Oe grosse dédicasse à mon gars Kruskal et à Badin !!!
  ifstream data("data.txt");
  if (!data)
    return -1;
  string line;
  vector<string> input;
  vector<vec3> coords;
  while (getline(data, line)) {
    input.push_back(line);
  }
  for (auto l : input) {
    vec3 coo;
    vector<string> coo_s = split(l, ',');
    coo.x = stoi(coo_s[0]);
    coo.y = stoi(coo_s[1]);
    coo.z = stoi(coo_s[2]);
    coords.push_back(coo);
  }

  int numNodes = coords.size();

  vector<Edge> edgeStorage;
  for (int i = 0; i < numNodes - 1; i++) {
    for (int j = i + 1; j < numNodes; j++) {
      Edge e;
      e.aIndex = i;
      e.bIndex = j;
      e.distance = norm(coords[i], coords[j]);
      edgeStorage.push_back(e);
    }
  }

  sort(edgeStorage.begin(), edgeStorage.end(),
       [](const Edge &a, const Edge &b) { return a.distance < b.distance; });

  map<int, set<int>> circuitMap;
  map<int, int> nodeToCircuit;
  int circuitId = 0;

  for (int i = 0; i < numNodes; i++) {
    nodeToCircuit[i] = i;
    circuitMap[i] = set<int>{i};
  }

  int connectionsMade = 0;
  vec3 lastMergedA, lastMergedB;

  for (int i = 0; i < edgeStorage.size() && circuitMap.size() > 1; i++) {
    int aIndex = edgeStorage[i].aIndex;
    int bIndex = edgeStorage[i].bIndex;

    int circuitA = nodeToCircuit[aIndex];
    int circuitB = nodeToCircuit[bIndex];

    // Acyclique
    if (circuitA == circuitB) {
      continue;
    }

    // Merge
    auto &setA = circuitMap[circuitA];
    auto &setB = circuitMap[circuitB];

    int keepId = (setA.size() >= setB.size()) ? circuitA : circuitB;
    int mergeId = (setA.size() >= setB.size()) ? circuitB : circuitA;

    auto &keepSet = circuitMap[keepId];
    auto &mergeSet = circuitMap[mergeId];

    for (int node : mergeSet) {
      keepSet.insert(node);
      nodeToCircuit[node] = keepId;
    }

    circuitMap.erase(mergeId);
    connectionsMade++;

    lastMergedA = coords[aIndex];
    lastMergedB = coords[bIndex];
  }

  if (circuitMap.size() == 1) {
    cout << "Last merged nodes: (" << lastMergedA.x << "," << lastMergedA.y
         << "," << lastMergedA.z << ") and (" << lastMergedB.x << ","
         << lastMergedB.y << "," << lastMergedB.z << ")" << endl;
    long long result = (long long)lastMergedA.x * lastMergedB.x;
    cout << "Result: " << result << endl;

    return 0;
  }
}
