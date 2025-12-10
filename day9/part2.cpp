#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

struct point {
  int x, y;
};
struct vseg {
  int x, y1, y2;
};
struct hseg {
  int y, x1, x2;
};

uint64_t dist(uint64_t a, uint64_t b) { return a > b ? a - b : b - a; }

bool ray_hit(int px, int py, const vseg &s) {
  return py >= s.y1 && py < s.y2 && px <= s.x;
}

bool intersect(int y, int x1, int x2, int x, int y1, int y2) {
  return x1 < x && x < x2 && y1 < y && y < y2;
}

bool inside(point p, const vector<vseg> &vsegs) {
  int cnt = 0;
  for (auto &s : vsegs)
    if (ray_hit(p.x, p.y, s))
      cnt++;
  return cnt & 1;
}

bool valid(point a, point b, const set<pair<int, int>> &boundary,
           const vector<vseg> &vsegs, const vector<hseg> &hsegs) {
  point corners[] = {a, {a.x, b.y}, b, {b.x, a.y}};
  for (auto &c : corners) {
    if (!boundary.count({c.x, c.y}) && !inside(c, vsegs))
      return false;
  }

  int x1 = min(a.x, b.x), x2 = max(a.x, b.x);
  int y1 = min(a.y, b.y), y2 = max(a.y, b.y);

  for (int y : {y1, y2})
    for (auto &v : vsegs)
      if (intersect(y, x1, x2, v.x, v.y1, v.y2))
        return false;

  for (int x : {x1, x2})
    for (auto &h : hsegs)
      if (intersect(h.y, h.x1, h.x2, x, y1, y2))
        return false;

  return true;
}

// I refuse to believe that a random video about doom rendering engine I watched
// months ago was useful
int main() {
  ifstream f("data.txt");
  if (!f)
    return -1;

  string line;
  vector<point> pts;
  while (getline(f, line)) {
    size_t comma = line.find(',');
    pts.push_back({stoi(line.substr(0, comma)), stoi(line.substr(comma + 1))});
  }

  vector<vseg> vsegs;
  vector<hseg> hsegs;
  set<pair<int, int>> boundary;

  for (int i = 0; i < pts.size(); i++) {
    point p1 = pts[i], p2 = pts[(i + 1) % pts.size()];
    int dx = p1.x == p2.x ? 0 : (p2.x > p1.x ? 1 : -1);
    int dy = p1.y == p2.y ? 0 : (p2.y > p1.y ? 1 : -1);

    for (int x = p1.x, y = p1.y; x != p2.x || y != p2.y; x += dx, y += dy)
      boundary.insert({x, y});
    boundary.insert({p2.x, p2.y});

    if (p1.x == p2.x)
      vsegs.push_back({p1.x, min(p1.y, p2.y), max(p1.y, p2.y)});
    else
      hsegs.push_back({p1.y, min(p1.x, p2.x), max(p1.x, p2.x)});
  }

  uint64_t mx = 0;
  int i1 = -1, i2 = -1;

  for (int i = 0; i < pts.size(); i++) {
    for (int j = i + 1; j < pts.size(); j++) {
      if (valid(pts[i], pts[j], boundary, vsegs, hsegs)) {
        uint64_t area =
            (dist(pts[i].x, pts[j].x) + 1) * (dist(pts[i].y, pts[j].y) + 1);
        if (area > mx) {
          mx = area;
          i1 = i;
          i2 = j;
        }
      }
    }
  }

  cout << mx << endl;
  return 0;
}
