#include <bits/stdc++.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// I'm not sure but I think this works for n digits
// Btw that took ages to do. Born to brute force, forced to find a somewhat
// decent approach
string best_substr(const string &s) {
  int n = (int)s.size();

  string st;
  st.reserve(12);

  for (int i = 0; i < n; ++i) {
    char c = s[i];
    // pop the smaller digits while the size is still <12
    while (!st.empty() && st.back() < c && (int)st.size() - 1 + (n - i) >= 12) {
      st.pop_back();
    }
    if ((int)st.size() < 12) {
      st.push_back(c);
    }
  }

  if ((int)st.size() > 12)
    st.resize(12);
  return st;
}

int main() {
  ifstream file("data.txt");
  if (!file) {
    return 1;
  }
  string bank;
  long long sum = 0;
  while (getline(file, bank)) {
    int first_index = 0;
    int second_index = 0;
    cout << bank;
    long long best = stoll(best_substr(bank));
    cout << " | " << best << "\n";
    sum += best;
  }

  cout << "\n" << sum << "\n";
  return 0;
}
