#include "algorithm.h"
#include "data_entry.h"
#include "reader.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

std::string to_string(
    const Entry &e,
    const std::map<std::pair<double, double>, std::vector<int>> &coord2id
) {
  // assert e{lat, lon} to be exist in coord2id, or let it crash
  std::string ret;
  ret.reserve(32);
  ret += std::format("({:.6f}, {:.6f}) [", e.longitude, e.latitude);
  for (int id : coord2id.find({e.longitude, e.latitude})->second)
    ret += std::format("{} ", id);
  ret.pop_back();
  ret += ']';
  return ret;
}

int main(int argc, char **argv) {
  std::vector<Entry> entries = read_data(argc, argv);

  std::map<std::pair<double, double>, std::vector<int>> coord2id;
  for (const Entry &e : entries)
    coord2id[{e.longitude, e.latitude}].emplace_back(e.enodebid);
  for (const auto &[p, v] : coord2id) {
    if (v.size() > 1) {
      std::cout << "Duplicate entries detected: ";
      for (int e : v)
        std::cout << e << ' ';
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;

  std::sort(entries.begin(), entries.end());
  entries.erase(std::unique(entries.begin(), entries.end()), entries.end());

  find_nearest_brute_force(entries);

  for (const auto &projection : {sinusoidal, mercator, miller}) {
    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();

    std::vector<Point> points;
    auto point_to_index = [&points](const Point &p) {
      return std::distance(
          points.begin(), std::find(points.begin(), points.end(), p)
      );
    };

    for (const Entry &e : entries)
      points.emplace_back(projection(e));

    AlgoRes ret = find_nearest(points);
    Entry e1 = entries[point_to_index(ret.nearest1)];
    Entry e2 = entries[point_to_index(ret.nearest2)];
    Entry e3 = entries[point_to_index(ret.second_nearest1)];
    Entry e4 = entries[point_to_index(ret.second_nearest2)];

    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    if (projection == sinusoidal)
      std::cout << "| Sinusoidal projection:\n";
    else if (projection == mercator)
      std::cout << "| Mercator projection:\n";
    else if (projection == miller)
      std::cout << "| Miller projection:\n";

    std::cout << std::format(
        "|          Nearest:  {} <-> {}  ({} m)\n|   Second Nearest:  {} <-> "
        "{}  ({} m)\n| Time elapsed: {} μs\n",
        to_string(e1, coord2id), to_string(e2, coord2id), real_dist(e1, e2),
        to_string(e3, coord2id), to_string(e4, coord2id), real_dist(e3, e4),
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count()
    );
    std::cout << std::endl;
  }
}
