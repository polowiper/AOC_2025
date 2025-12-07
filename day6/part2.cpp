#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string> process_line(const string &line) {
  vector<string> result;
  stringstream ss(line);
  string token;
  while (ss >> token) {
    result.push_back(token);
  }
  return result;
}

int main(void) {
  string line;
  ifstream file("data.txt");
  if (!file)
    return 1;

  vector<string> lines;
  while (getline(file, line)) {
    if (line.find_first_not_of(" \t\r\n") == string::npos)
      continue;
    lines.push_back(line);
  }
  if (lines.empty())
    return 0;

  string ops_line = lines.back();
  lines.pop_back();
  vector<string> ops_tok = process_line(ops_line);
  reverse(ops_tok.begin(), ops_tok.end());
  size_t cols = ops_tok.size();

  size_t rows_count = lines.size();
  size_t maxw = 0;
  for (auto &ln : lines)
    if (ln.size() > maxw)
      maxw = ln.size();
  for (auto &ln : lines)
    if (ln.size() < maxw)
      ln.append(maxw - ln.size(), ' ');

  vector<string> columns(maxw);
  for (size_t c = 0; c < maxw; ++c) {
    string col;
    col.reserve(rows_count);
    for (size_t r = 0; r < rows_count; ++r)
      col.push_back(lines[r][c]);
    columns[c] = move(col);
  }
  reverse(columns.begin(), columns.end());

  auto is_all_spaces = [](const string &col) {
    for (char ch : col)
      if (ch != ' ')
        return false;
    return true;
  };

  vector<vector<string>> groups;
  for (size_t i = 0; i < columns.size();) {
    if (is_all_spaces(columns[i])) {
      ++i;
      continue;
    }
    vector<string> g;
    while (i < columns.size() && !is_all_spaces(columns[i]))
      g.push_back(columns[i++]);
    groups.push_back(move(g));
  }

  vector<vector<long long>> number_groups;
  number_groups.reserve(groups.size());
  for (auto &grp : groups) {
    vector<long long> nums;
    nums.reserve(grp.size());
    for (auto &col : grp) {
      string s;
      s.reserve(col.size());
      for (char ch : col)
        if (ch != ' ')
          s.push_back(ch);
      nums.push_back(s.empty() ? 0LL : stoll(s));
    }
    number_groups.push_back(move(nums));
  }

  size_t pairs = min(ops_tok.size(), number_groups.size());
  uint64_t total = 0;

  for (size_t i = 0; i < pairs; ++i) {
    string sym = ops_tok[i];
    char op = sym.empty() ? '+' : sym[0];
    auto &nums = number_groups[i];
    if (nums.empty())
      continue;

    uint64_t acc;
    if (op == '*') {
      acc = (uint64_t)nums[0];
      for (size_t k = 1; k < nums.size(); ++k)
        acc *= (uint64_t)nums[k];
    } else {
      acc = 0;
      for (auto v : nums)
        acc += (uint64_t)v;
    }

    string nums_s;
    for (size_t k = 0; k < nums.size(); ++k) {
      if (k)
        nums_s += string(" ") + op + string(" ");
      nums_s += to_string(nums[k]);
    }

    cout << nums_s << " = " << acc << "\n";
    total += acc;
  }

  cout << "Result: " << total << endl;
  return 0;
}
