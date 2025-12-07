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
  size_t cols = ops_tok.size();

  vector<vector<long long>> rows;
  for (auto &ln : lines) {
    vector<string> tok = process_line(ln);
    tok.resize(cols, "0");
    vector<long long> row;
    for (size_t k = 0; k < cols; ++k)
      row.push_back(stoll(tok[k]));
    rows.push_back(move(row));
  }

  uint64_t total_sum = 0;
  for (size_t c = 0; c < cols; ++c) {
    char op = ops_tok[c].empty()
                  ? '+'
                  : ops_tok[c][0]; // Yeah don't question that lol
    uint64_t col_res = (op == '*') ? 1 : 0;
    for (size_t r = 0; r < rows.size(); ++r) {
      cout << rows[r][c] << " ";
      if (op == '*')
        col_res *= (uint64_t)rows[r][c];
      else
        col_res += (uint64_t)rows[r][c];
    }
    cout << ":" << col_res << endl;
    total_sum += col_res;
  }

  cout << "Result: " << total_sum << endl;
  return 0;
}
