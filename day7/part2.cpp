#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int find_start(vector<string> text) {
  int index = 0;
  while (text[0][index] != 'S') {
    index++;
  }
  return index;
}
long long explore(const vector<string> &text, int row, int col,
                  long long **Mem) {
  // Hahahaha I tried to do it without storing the results first hahahaha
  if (col < 0 || col >= text[0].size()) {
    return 0;
  }

  if (row >= text.size()) {
    return 1;
  }

  if (Mem[row][col] != -1) {
    return Mem[row][col];
  }

  if (text[row][col] == '^') {
    long long left = explore(text, row + 1, col - 1, Mem);
    long long right = explore(text, row + 1, col + 1, Mem);
    Mem[row][col] = left + right;
  } else {
    Mem[row][col] = explore(text, row + 1, col, Mem);
  }

  return Mem[row][col];
}
int main(void) {
  string line;
  ifstream file("data.txt");
  vector<string> input;
  if (!file)
    return -1;
  while (getline(file, line)) {
    input.push_back(line);
  }
  int start_index = find_start(input);
  long long **Mem = (long long **)malloc(sizeof(long long *) * input.size());
  for (int i = 0; i < input.size(); i++) {
    Mem[i] = (long long *)malloc(sizeof(long long) * input[0].size());
    for (int j = 0; j < input[0].size(); j++) {
      Mem[i][j] = -1;
    }
  }
  long long timelines = explore(input, 0, start_index, Mem);
  for (auto line : input)
    cout << line << endl;
  cout << timelines << endl;
  for (int i = 0; i < input.size(); i++) {
    free(Mem[i]);
  }
  free(Mem);
}
