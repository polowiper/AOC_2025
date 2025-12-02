#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string> split(const string s, char delimiter) {
  vector<string> tokens;
  stringstream ss(s);
  string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

vector<string> separate(const string &s, int n) {
  vector<string> arr;

  if (s.length() % n != 0)
    return arr; // empty = invalid split

  int portion = s.length() / n;

  for (int i = 0; i < n; i++) {
    arr.push_back(s.substr(i * portion, portion));
  }

  return arr;
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

      long long start = stoll(RawStartEnd[0]);
      long long end = stoll(RawStartEnd[1]);
      cout << start << "-" << end << "\n";
      for (long long i = start; i <= end; i++) {
        bool invalid = false;
        string si = to_string(i);
        int max_portions = si.length();

        for (int j = 2; j <= max_portions; j++) {
          bool all_equal = true;
          vector<string> parts = separate(si, j);
          if (parts.empty())
            continue;
          for (int u = 1; u < parts.size(); u++) {
            if (parts[u - 1] != parts[u]) {
              all_equal = false;
              break;
            }
          }
          if (all_equal) {
            invalid = true;
            break;
          }
        }

        if (invalid) {
          cout << "Invalid : " << si << "\n";
          res += i;
        }
      }
    }
    cout << res;
  }

  return 0;
}
