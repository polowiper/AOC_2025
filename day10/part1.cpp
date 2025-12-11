#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int bfs(int target, const vector<vector<int>> &btns) {
  if (target == 0)
    return 0;

  queue<pair<int, int>> q; // (state, steps)
  set<int> visited;

  q.push({0, 0});
  visited.insert(0);

  while (!q.empty()) {
    auto [state, steps] = q.front();
    q.pop();

    for (auto &btn : btns) {
      int new_state = state;
      for (int idx : btn) {
        new_state ^= (1 << idx);
      }

      if (new_state == target)
        return steps + 1;

      if (!visited.count(new_state)) {
        visited.insert(new_state);
        q.push({new_state, steps + 1});
      }
    }
  }

  return -1;
}

int main(void) {
  ifstream file("data.txt");
  if (!file)
    return -1;

  string line;
  vector<vector<vector<int>>> buttons; // Holy fucking shit
  vector<string> lights;

  regex light_pattern(R"(\[([#.]+)\])");
  regex button_pattern(R"(\(([0-9,]+)\))");

  while (getline(file, line)) {
    smatch match;
    if (regex_search(line, match, light_pattern)) {
      lights.push_back(match[1].str());
    }

    vector<vector<int>> row_buttons;
    auto buttons_begin =
        sregex_iterator(line.begin(), line.end(), button_pattern);
    auto buttons_end = sregex_iterator();

    for (auto it = buttons_begin; it != buttons_end; ++it) {
      smatch btn_match = *it;
      string btn_str = btn_match[1].str();

      vector<int> indices;
      stringstream ss(btn_str);
      string num;
      while (getline(ss, num, ',')) {
        indices.push_back(stoi(num));
      }
      row_buttons.push_back(indices);
    }
    buttons.push_back(row_buttons);
  }

  int total = 0;

  for (int row = 0; row < buttons.size(); row++) {
    int target = 0;
    for (int i = 0; i < lights[row].size(); i++) {
      if (lights[row][i] == '#') {
        target |= (1 << i);
      }
    }

    int result = bfs(target, buttons[row]);

    cout << row << ": " << lights[row] << " " << result << endl;
    total += result;
  }

  cout << "Total: " << total << endl;

  return 0;
}
