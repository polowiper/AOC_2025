#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
void dfs(const vector<string> &servers, const vector<vector<string>> &neighbors,
         int &count) {
  string start = "you";
  string end = "out";

  map<string, int> server_idx;
  for (int i = 0; i < servers.size(); i++) {
    server_idx[servers[i]] = i;
  }

  function<void(string, vector<bool> &)> dfs = [&](string curr,
                                                   vector<bool> &visited) {
    if (curr == end) {
      count++;
      return;
    }

    if (!server_idx.count(curr))
      return;

    int curr_idx = server_idx[curr];
    for (const string &neighbor : neighbors[curr_idx]) {
      if (neighbor == end) {
        count++;
      } else if (server_idx.count(neighbor)) {
        int nb_idx = server_idx[neighbor];
        if (!visited[nb_idx]) {
          visited[nb_idx] = true;
          dfs(neighbor, visited);
          visited[nb_idx] = false;
        }
      }
    }
  };

  vector<bool> visited(servers.size(), false);
  int start_idx = server_idx[start];
  visited[start_idx] = true;
  dfs(start, visited);
}
int main(void) {
  fstream file("data.txt");
  if (!file)
    return -1;
  string line;
  int count = 0;

  vector<string> servers;
  vector<vector<string>> neighbors;
  while (getline(file, line)) {
    int bp = line.find(":");
    servers.push_back(line.substr(0, bp));
    string neighbors_str = line.substr(bp + 1);
    stringstream ss(neighbors_str);
    string nb;
    vector<string> neighbor_list;
    while (ss >> nb) {
      neighbor_list.push_back(nb);
    }
    neighbors.push_back(neighbor_list);
  }
  dfs(servers, neighbors, count);
  cout << "Count: " << count << endl;
}
