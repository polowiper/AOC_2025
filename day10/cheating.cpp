#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <z3++.h> //I'm sorry father, forgive me
using namespace std;

int main(void) {
  ifstream file("data.txt");
  if (!file)
    return -1;

  string line;
  vector<vector<vector<int>>> buttons;
  vector<vector<int>> joltages;

  regex button_pattern(R"(\(([0-9,]+)\))");
  regex joltage_pattern(R"(\{([0-9,]+)\})");

  while (getline(file, line)) {
    vector<vector<int>> row_buttons;

    // Parse buttons
    auto btn_begin = sregex_iterator(line.begin(), line.end(), button_pattern);
    auto btn_end = sregex_iterator();

    for (auto it = btn_begin; it != btn_end; ++it) {
      smatch match = *it;
      string indices_str = match.str(1);
      stringstream ss(indices_str);
      string token;
      vector<int> indices;

      while (getline(ss, token, ',')) {
        indices.push_back(stoi(token));
      }
      row_buttons.push_back(indices);
    }

    smatch joltage_match;
    if (regex_search(line, joltage_match, joltage_pattern)) {
      string jolt_str = joltage_match.str(1);
      stringstream ss(jolt_str);
      string token;
      vector<int> jolt;

      while (getline(ss, token, ',')) {
        jolt.push_back(stoi(token));
      }
      joltages.push_back(jolt);
    }

    buttons.push_back(row_buttons);
  }

  int total = 0;

  for (int row = 0; row < joltages.size(); row++) {
    auto &joltage = joltages[row];
    auto &btns = buttons[row];

    z3::context ctx;
    z3::optimize opt(ctx);

    vector<z3::expr> press_vars;
    for (int b = 0; b < btns.size(); b++) {
      string var_name = "b" + to_string(b);
      press_vars.push_back(ctx.int_const(var_name.c_str()));
      opt.add(press_vars[b] >= 0);
    }

    for (int pos = 0; pos < joltage.size(); pos++) {
      z3::expr sum = ctx.int_val(0);

      for (int b = 0; b < btns.size(); b++) {
        bool affects = false;
        for (int idx : btns[b]) {
          if (idx == pos) {
            affects = true;
            break;
          }
        }
        if (affects) {
          sum = sum + press_vars[b];
        }
      }

      // Constraint: sum equals target joltage
      opt.add(sum == joltage[pos]);
    }

    z3::expr total_presses = ctx.int_val(0);
    for (auto &var : press_vars) {
      total_presses = total_presses + var;
    }
    opt.minimize(total_presses);

    if (opt.check() == z3::sat) {
      z3::model m = opt.get_model();
      int result = 0;

      for (auto &var : press_vars) {
        int val = m.eval(var).get_numeral_int();
        result += val;
      }

      cout << "[" << row + 1 << "/200]: ";
      for (int j : joltage)
        cout << j << " ";
      cout << "-> " << result << " presses" << endl;

      total += result;
    }
  }

  cout << "\nTotal: " << total << endl;
  return 0;
}
