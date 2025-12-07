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
void fix(vector<array<long long, 2>> &ranges, array<long long, 2> &new_range) {
  if (ranges.empty()) {
    ranges.push_back(new_range);
    return;
  }
  size_t i = 0;
  while (i < ranges.size()) {
    auto &ex = ranges[i];
    if (!(new_range[1] < ex[0] || new_range[0] > ex[1])) {
      new_range[0] = min(new_range[0], ex[0]);
      new_range[1] = max(new_range[1], ex[1]);
      ranges.erase(ranges.begin() + i);
    } else {
      ++i;
    }
  }
  ranges.push_back(new_range);
}
int main(void) {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string line;
  // What the actual fuck a long long wasn't enough hahahahahaha
  // Fucking 352716206375547 what a joke
  unsigned long long valid_items = 0ULL;
  vector<array<long long, 2>> ranges;

  while (getline(file, line)) {
    if (line.empty()) {
      break;
    }
    array<long long, 2> range = parse_range(line);
    fix(ranges, range);
  }
  for (auto &m : ranges) {
    cout << m[0] << "-" << m[1] << endl;
    unsigned long long len =
        (unsigned long long)(m[1]) - (unsigned long long)(m[0]) + 1ULL;
    valid_items += len;
  }

  cout << "Valid items: " << valid_items << endl;
  return 0;
}
