#pragma once

#ifndef DATA_H
#  define DATA_H

#  include <cmath>
#  include <numbers>
#  include <numeric>
#  include <tuple>

constexpr const static double PI = std::numbers::pi;
constexpr const static double EARTH_RADIUS = 6378137.; // m

struct Point {
  double x, y;

  bool operator<(const Point &rhs) const {
    return std::tie(x, y) < std::tie(rhs.x, rhs.y);
  }

  bool operator==(const Point &rhs) const {
    return std::tie(x, y) == std::tie(rhs.x, rhs.y);
  }
};

inline double dist(const Point &p1, const Point &p2) {
  return std::hypot(p1.x - p2.x, p1.y - p2.y);
}

inline double deg2rad(double deg) {
  return deg * PI / 180;
}

struct Entry {
  int enodebid;
  double longitude, latitude;

  bool operator<(const Entry &rhs) const {
    return std::tie(latitude, longitude) <
           std::tie(rhs.latitude, rhs.longitude);
  }

  bool operator==(const Entry &rhs) const {
    return std::tie(latitude, longitude) ==
           std::tie(rhs.latitude, rhs.longitude);
  }
};

inline double real_dist(const Entry &e1, const Entry &e2) {
  double rad_lon1 = deg2rad(e1.longitude);
  double rad_lon2 = deg2rad(e2.longitude);
  double rad_lat1 = deg2rad(e1.latitude);
  double rad_lat2 = deg2rad(e2.latitude);
  double s = acos(
      cos(rad_lat1) * cos(rad_lat2) * cos(rad_lon1 - rad_lon2) +
      sin(rad_lat1) * sin(rad_lat2)
  );
  s *= EARTH_RADIUS;
  return s;
}

inline Point sinusoidal(const Entry &e) {
  constexpr const static double CENTRAL_MERIDIAN =
      std::midpoint(102.673290, 102.895277); // 在实验要求 ppt 中给出

  double rad_lat = deg2rad(e.latitude);
  return {deg2rad(e.longitude - CENTRAL_MERIDIAN) * cos(rad_lat), rad_lat};
}

inline Point mercator(const Entry &e) {
  return {
      deg2rad(e.longitude),
      log(tan(deg2rad(e.latitude) / 2 + PI / 4)),
  };
}

inline Point miller(const Entry &e) {
  return {
      deg2rad(e.longitude),
      1.25 * log(tan(PI / 4 + 0.4 * deg2rad(e.latitude))),
  };
}

#endif // !DATA_H
