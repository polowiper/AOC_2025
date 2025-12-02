#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  stringstream ss(s);
  string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

int main() {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string line;
  long long res;
  while (getline(file, line)) {
    vector<string> ranges = split(line, ',');
    for (string range : ranges) {
      vector<string> RawStartEnd = split(range, '-');
      if (RawStartEnd[0].length() % 2 == 1 &&
          RawStartEnd[0].length() == RawStartEnd[1].length()) {
        continue;
      }

      long long start = stoll(RawStartEnd[0]);
      long long end = stoll(RawStartEnd[1]);
      cout << start << "-" << end << "\n";

      for (long long i = start; i <= end; i++) {
        string si = to_string(i);
        if (si.length() % 2 == 1) {
          continue;
        }
        int half = si.length() / 2;
        string left = si.substr(0, half);
        string right = si.substr(half);
        if (right == left) {
          cout << "Invalid : " << i << "\n";
          res += i;
        }
      }
    }
    cout << res;
  }

  return 0;
}
