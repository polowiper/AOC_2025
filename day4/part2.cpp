#include <bits/stdc++.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool check_around(vector<string> &text, int i, int j) {
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  int invalid = 0;
  for (auto &dir : directions) {
    int x = i + dir[0];
    int y = j + dir[1];
    if (x >= 0 && x < text.size() && y >= 0 && y < text[0].size()) {
      if (text[x][y] == '@') {
        invalid++;
      }
    }
  }
  return invalid < 4;
}
int solution(vector<string> &initial_state, vector<string> &modified_state) {
  int sum = 0;
  vector<string> text_copy = initial_state;
  for (int i = 0; i < initial_state.size(); i++) {
    for (int j = 0; j < initial_state[i].size(); j++) {
      if (initial_state[i][j] == 'x') {
        modified_state[i][j] = 'x';
        continue;
      }
      if (initial_state[i][j] == '@') {
        if (check_around(initial_state, i, j)) {
          modified_state[i][j] = 'x';
          sum++;
        }
      } else {
        continue;
      }
    }
  }
  return sum;
}
int main() {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string line;
  vector<string> initial_state;
  while (getline(file, line)) {
    initial_state.push_back(line);
  }
  vector<string> modified_state = initial_state;
  int res = 0;
  for (int i = 0; i < initial_state.size(); i++) {
    cout << initial_state[i] << endl;
  }
  int sol = 0;
  while ((sol = solution(initial_state, modified_state)) > 0) {
    cout << "-------------------" << endl
         << "Solution this round: " << sol << endl;
    for (int i = 0; i < modified_state.size(); i++) {
      cout << modified_state[i] << endl;
    }
    res += sol;
    initial_state = modified_state;
  }
  cout << "Final result: " << res << endl;
  return 0;
}
