#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

// While this works, this is painfully slow and I hate it
// It took ~45mins to complete the full input on a 16-core ryzen 7 7840u so.....
// I couldn't think of a smarter approach even after a few hours so, fuck this
// problem

mutex mtx;
map<vector<int>, int> memo;
int total_count = 0;
void process_row(int row, const vector<int> &joltage,
                 const vector<vector<int>> &btns, int &total) {
  map<vector<int>, int> local_memo;

  function<int(vector<int>)> local_dfs = [&](vector<int> vals) -> int {
    bool all_zero = true;
    for (int v : vals) {
      if (v != 0) {
        all_zero = false;
        break;
      }
    }
    if (all_zero)
      return 0;

    if (local_memo.count(vals))
      return local_memo[vals];

    int best_pos = -1;
    int min_buttons = INT_MAX;

    for (int i = 0; i < vals.size(); i++) {
      if (vals[i] == 0)
        continue;

      int count = 0;
      for (auto &btn : btns) {
        for (int idx : btn) {
          if (idx == i) {
            count++;
            break;
          }
        }
      }

      if (count > 0 && count < min_buttons) {
        min_buttons = count;
        best_pos = i;
      }
    }

    if (best_pos == -1)
      return INT_MAX;

    vector<int> relevant_btns;
    for (int b = 0; b < btns.size(); b++) {
      for (int idx : btns[b]) {
        if (idx == best_pos) {
          relevant_btns.push_back(b);
          break;
        }
      }
    }

    int target = vals[best_pos];
    int best = INT_MAX;

    // Try all integer partitions of target using relevant buttons
    function<void(int, vector<int> &, int)> partition =
        [&](int btn_idx, vector<int> &presses, int sum) {
          if (btn_idx == relevant_btns.size()) {
            if (sum == target) {
              vector<int> new_vals = vals;
              int total_presses = 0;

              for (int i = 0; i < relevant_btns.size(); i++) {
                int btn = relevant_btns[i];
                for (int p = 0; p < presses[i]; p++) {
                  for (int idx : btns[btn]) {
                    new_vals[idx]--;
                  }
                  total_presses++;
                }
              }

              bool valid = true;
              for (int v : new_vals) {
                if (v < 0) {
                  valid = false;
                  break;
                }
              }

              if (valid) {
                int res = local_dfs(new_vals);
                if (res != INT_MAX) {
                  best = min(best, total_presses + res);
                }
              }
            }
            return;
          }

          for (int cnt = 0; cnt <= target; cnt++) {
            presses[btn_idx] = cnt;
            partition(btn_idx + 1, presses, sum + cnt);
            if (sum + cnt > target)
              break; // We can get rid of unnecessary calls
          }
        };

    vector<int> presses(relevant_btns.size(), 0);
    partition(0, presses, 0);

    local_memo[vals] = best;
    return best;
  };

  int result = local_dfs(joltage);

  lock_guard<mutex> lock(mtx);
  total_count++;
  cout << "[" << total_count << "/200]: ";
  for (int j : joltage)
    cout << j << " ";
  cout << "-> " << result << " presses" << endl;

  if (result != INT_MAX)
    total += result;
}

int main(void) {
  ifstream file("data.txt");
  if (!file)
    return -1;

  string line;
  vector<vector<vector<int>>> buttons; // Holy fucking shit
  vector<string> lights;
  vector<vector<int>> joltages;

  regex light_pattern(R"(\[([#.]+)\])");
  regex button_pattern(R"(\(([0-9,]+)\))");
  regex joltage_pattern(R"(\{([0-9,]+)\})");

  while (getline(file, line)) {
    smatch match;
    if (regex_search(line, match, light_pattern)) {
      lights.push_back(match[1].str());
    }

    smatch joltage_match;
    if (regex_search(line, joltage_match, joltage_pattern)) {
      string joltage_str = joltage_match[1].str();

      vector<int> joltages_row;
      stringstream ss(joltage_str);
      string num;
      while (getline(ss, num, ',')) {
        joltages_row.push_back(stoi(num));
      }
      joltages.push_back(joltages_row);
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
  vector<thread> threads;

  for (int row = 0; row < buttons.size(); row++) {
    threads.emplace_back(process_row, row, ref(joltages[row]),
                         ref(buttons[row]), ref(total));
  }

  for (auto &t : threads) {
    t.join();
  }

  cout << "Total: " << total << endl;

  return 0;
}
