#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void dfs(const vector<string> &servers, const vector<vector<string>> &neighbors,
         long long &count) {
  string start = "svr";
  string end = "out";
  vector<string> required = {"dac", "fft"};

  map<string, int> server_idx;
  for (int i = 0; i < servers.size(); i++) {
    server_idx[servers[i]] = i;
  }

  map<string, int> req_bit;
  for (int i = 0; i < required.size(); i++) {
    req_bit[required[i]] = i;
  }
  int all_visited = (1 << required.size()) - 1;

  vector<map<int, long long>> memo(servers.size());

  function<long long(int, int)> search = [&](int curr_idx,
                                             int visited_mask) -> long long {
    if (memo[curr_idx].count(visited_mask)) {
      return memo[curr_idx][visited_mask];
    }

    string curr = servers[curr_idx];

    if (req_bit.count(curr)) {
      visited_mask |= (1 << req_bit[curr]);
    }

    if (curr == end) {
      return (visited_mask == all_visited) ? 1 : 0;
    }

    long long path_count = 0;

    for (const string &neighbor : neighbors[curr_idx]) {
      if (neighbor == end) {
        if (visited_mask == all_visited) {
          path_count++;
        }
      } else if (server_idx.count(neighbor)) {
        int nb_idx = server_idx[neighbor];
        path_count += search(nb_idx, visited_mask);
      }
    }

    memo[curr_idx][visited_mask] = path_count;
    return path_count;
  };

  if (server_idx.count(start)) {
    int start_idx = server_idx[start];
    count = search(start_idx, 0);
  }
}

int main(void) {
  fstream file("data.txt");
  if (!file)
    return -1;
  string line;
  long long count = 0;

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
