#include "algorithm.h"

#include "data_entry.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <numeric>
#include <vector>

constexpr const double INF = 1e10;

void find_nearest_brute_force(const std::vector<Entry> &entries) {
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();

  double min_dist = INF, second_min_dist = INF;
  Entry nearest1{}, nearest2{}, second_nearest1{}, second_nearest2{};
  for (std::vector<Entry>::size_type i = 0; i < entries.size(); ++i)
    for (std::vector<Entry>::size_type j = i + 1; j < entries.size(); ++j) {
      double d = real_dist(entries[i], entries[j]);
      if (d < min_dist) {
        second_min_dist = min_dist;
        second_nearest1 = nearest1;
        second_nearest2 = nearest2;
        min_dist = d;
        nearest1 = entries[i];
        nearest2 = entries[j];
      } else if (d < second_min_dist) {
        second_min_dist = d;
        second_nearest1 = entries[i];
        second_nearest2 = entries[j];
      }
    }

  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  std::cout << std::format(
      "| Brute force:\n|          Nearest:  ({:.6f}, {:.6f}) [{}] <-> ({:.6f}, "
      "{:.6f}) [{}]  ({} m)\n|   Second Nearest:  ({:.6f}, {:.6f}) [{}] <-> "
      "({:.6f}, {:.6f}) [{}]  ({} m)\n| Time elapsed: {} μs\n",
      nearest1.longitude, nearest1.latitude, nearest1.enodebid,
      nearest2.longitude, nearest2.latitude, nearest2.enodebid,
      real_dist(nearest1, nearest2), second_nearest1.longitude,
      second_nearest1.latitude, second_nearest1.enodebid,
      second_nearest2.longitude, second_nearest2.latitude,
      second_nearest2.enodebid, real_dist(second_nearest1, second_nearest2),
      std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
  );
  std::cout << std::endl;
}

using Iter = std::vector<Point>::iterator;

InnerAlgoRes _find_nearest(Iter begin, Iter end) {
  Iter::difference_type dis = std::distance(begin, end);
  if (dis == 1)
    return {*begin, *begin, INF};
  else if (dis == 2)
    return {*begin, *std::next(begin), dist(*begin, *std::next(begin))};

  Iter mid = midpoint(begin, end);
  auto res1 = _find_nearest(begin, mid);
  auto res2 = _find_nearest(mid, end);
  if (res1.min_dist > res2.min_dist)
    std::swap(res1, res2);

  auto [p1, p2, min_dist] = res1;
  std::vector<Point> t;

  mid = std::prev(mid);
  for (Iter it = begin; it != end; ++it)
    if (std::abs(it->x - mid->x) <= min_dist)
      t.emplace_back(*it);
  std::sort(t.begin(), t.end(), [](const Point &lhs, const Point &rhs) {
    return lhs.y < rhs.y;
  });

  for (Iter it = t.begin(); it != t.end(); ++it)
    for (Iter jt = std::next(it); jt != t.end() && jt->y - it->y < min_dist;
         ++jt) {
      if (double d = dist(*it, *jt); d < min_dist) {
        min_dist = d;
        p1 = *it;
        p2 = *jt;
      }
    }

  return {p1, p2, min_dist};
}

AlgoRes find_nearest(std::vector<Point> points) {
  auto [nearest1, nearest2, min_dist] =
      _find_nearest(points.begin(), points.end());

  Point second_nearest1 = nearest1, second_nearest2 = nearest1;
  double second_min_dist = INF;

  std::erase(points, nearest1);
  std::erase(points, nearest2);

  for (Iter it = points.begin(); it != points.end(); ++it) {
    if (dist(*it, nearest1) < second_min_dist) {
      second_nearest1 = nearest1;
      second_nearest2 = *it;
      second_min_dist = dist(*it, nearest1);
    }
    if (dist(*it, nearest2) < second_min_dist) {
      second_nearest1 = nearest2;
      second_nearest2 = *it;
      second_min_dist = dist(*it, nearest2);
    }
  }

  auto [nearest3, nearest4, min_dist2] =
      _find_nearest(points.begin(), points.end());
  if (min_dist2 < second_min_dist) {
    second_nearest1 = nearest3;
    second_nearest2 = nearest4;
    second_min_dist = min_dist2;
  }

  return {nearest1, nearest2, second_nearest1, second_nearest2};
}
