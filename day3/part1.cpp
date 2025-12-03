#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int best_num(const string &s) {

  int best = -1;
  int best_tens = -1;

  for (char c : s) {
    int d = c - '0';
    if (best_tens != -1) {
      int val = best_tens * 10 + d;
      if (val > best)
        best = val;
    }

    if (d > best_tens)
      best_tens = d;
  }

  return best;
}

int main() {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string bank;
  int sum = 0;
  while (getline(file, bank)) {
    int first_index = 0;
    int second_index = 0;
    cout << bank;
    int best = best_num(bank);
    cout << " | " << best << "\n";
    sum += best;
  }

  cout << "\n" << sum << "\n";
  return 0;
}
