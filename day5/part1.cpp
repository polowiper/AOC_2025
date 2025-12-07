#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

array<long long, 2> parse_range(const string &s) {
  array<long long, 2> range;
  stringstream ss(s);
  string token;
  getline(ss, token, '-');
  range[0] = stoll(token);
  getline(ss, token, '-');
  range[1] = stoll(token);
  return range;
}

int main(void) {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string line;
  int valid_items = 0;
  bool breakpoint = false;
  vector<array<long long, 2>> ranges;
  vector<long long> ids;

  while (getline(file, line)) {
    if (line.empty()) {
      breakpoint = true;
      continue;
    }
    if (!breakpoint) {
      ranges.push_back(parse_range(line));
    } else {
      ids.push_back(stoll(line));
    }
  }
  for (long long id : ids) {
    bool valid = false;
    for (auto &range : ranges) {
      if (id >= range[0] && id <= range[1]) {
        valid = true;
        valid_items++;
        break;
      }
    }
    if (valid) {
      cout << id << " is valid" << endl;
    } else {
      cout << id << " is invalid" << endl;
    }
  }

  cout << valid_items << endl;
  return 0;
}
