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

int main() {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  int sum = 0;
  string line;
  vector<string> text;
  while (getline(file, line)) {
    text.push_back(line);
  }
  vector<string> text_copy = text;
  for (int i = 0; i < text.size(); i++) {
    cout << text[i] << endl;
    for (int j = 0; j < text[i].size(); j++) {
      if (text[i][j] == '@') {
        if (check_around(text, i, j)) {
          text_copy[i][j] = 'x';
          sum++;
        }
      } else {
        continue;
      }
    }
  }

  cout << "Modified text:" << endl;
  for (int i = 0; i < text.size(); i++) {
    cout << text_copy[i] << endl;
  }
  cout << "Final result: " << sum << endl;
  return 0;
}
