#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef struct point {
  int x, y;
} point;

uint64_t distance(uint64_t a, uint64_t b) { return a > b ? a - b : b - a; }

int main(void) {
  ifstream file("data.txt");
  if (!file)
    return -1;

  string line;
  vector<point> points;
  while (getline(file, line)) {
    size_t comma = line.find(',');
    points.push_back(
        {stoi(line.substr(0, comma)), stoi(line.substr(comma + 1))});
  }

  uint64_t **areas = (uint64_t **)malloc(sizeof(uint64_t *) * points.size());
  uint64_t max = 0;
  int p1, p2;
  for (int i = 0; i < points.size(); i++) {
    areas[i] = (uint64_t *)malloc(sizeof(uint64_t) * points.size());
    for (int j = 0; j < points.size(); j++) {
      areas[i][j] = (distance(points[i].x, points[j].x) + 1) *
                    (distance(points[i].y, points[j].y) + 1);
      if (areas[i][j] > max) {
        max = areas[i][j];
        p1 = i;
        p2 = j;
      }
    }
  }
  cout << "The max area is " << max << " between points " << p1 << " and " << p2
       << endl;
  cout << "Point 1: (" << points[p1].x << ", " << points[p1].y << ")" << endl;
  cout << "Point 2: (" << points[p2].x << ", " << points[p2].y << ")" << endl;
  return 0;
}
