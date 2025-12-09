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
void explore(vector<string> &text, int i, int j, int *counter) {

  // We start from text[i][j] the number j char in the line i in the text
  for (int u = i; u < text.size(); u++) {
    if (text[u][j] == '|') {
      // Already explored
      break;
    }
    if (text[u][j] == '^') {
      *counter += 1;
      if (j > 0)
        text[u][j - 1] = '|';
      if (u + 1 < text.size())
        explore(text, u + 1, j - 1, counter);
      if (j + 1 < text[u].size())
        text[u][j + 1] = '|';
      if (u + 1 < text.size())
        explore(text, u + 1, j + 1, counter);
      break; // We can't go lower because we're stuck by the ^ we need to go on
             // the sides
    }
    text[u][j] = '|';
  }
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
  int counter = 0;
  explore(input, 0, start_index, &counter);
  for (auto line : input)
    cout << line << endl;
  cout << counter << endl;
}
