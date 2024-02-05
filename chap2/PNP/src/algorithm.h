#pragma once

#ifndef ALGORITHM_H
#  define ALGORITHM_H

#  include "data_entry.h"

#  include <vector>

template<typename I>
  requires std::random_access_iterator<I>
inline I midpoint(I l, I r) {
  return l + std::distance(l, r) / 2;
}

struct AlgoRes {
  Point nearest1, nearest2;
  Point second_nearest1, second_nearest2;
};

struct InnerAlgoRes {
  Point nearest1, nearest2;
  double min_dist;
};

void find_nearest_brute_force(const std::vector<Entry> &entries);

AlgoRes find_nearest(std::vector<Point> points);

#endif // !ALGORITHM_H
