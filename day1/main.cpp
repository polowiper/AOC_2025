
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
  int dial = 50;
  int zeroes = 0;

  ifstream file("data.txt");
  if (!file)
    return 1;

  string line;
  while (getline(file, line)) {
    char way = line[0];
    int steps = stoi(line.substr(1));
    // After around an hour of trying to find a smart way to do it I realized
    // that I'm not that smart so I did it the dumb way
    for (int i = 0; i < steps; ++i) {
      if (way == 'R') {
        dial = (dial + 1) % 100;
      } else if (way == 'L') {
        dial = (dial - 1 + 100) % 100;
      }

      if (dial == 0)
        zeroes++;
    }
  }

  cout << zeroes << "\n";
}
