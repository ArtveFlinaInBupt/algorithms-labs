#pragma once

#ifndef GRAPH_H
#  define GRAPH_H

#  include <algorithm>
#  include <exception>
#  include <format>
#  include <iostream>
#  include <type_traits>
#  include <vector>

// clang-format off
template<typename T>
requires std::is_arithmetic_v<T>
struct Graph {
  // clang-format on

  constexpr const static inline T INF = std::numeric_limits<T>::max() / 2;

  int n;
  int src;
  std::vector<int> id;
  std::vector<std::vector<T>> adj;

  Graph(int n_, int src_): n(n_), src(src_) {
    id.resize(n + 1);
    adj.resize(n + 1);
    for (int u = 1; u <= n; ++u)
      adj[u].resize(n + 1);
  }

  static Graph<T> read_from(std::istream &is) {
    int n, src;
    is >> n >> src;
    Graph<T> g(n, src);
    for (int u = 1; u <= n; ++u)
      is >> g.id[u];

    auto get_index = [&](int id) -> int {
      if (auto it = std::find(g.id.begin(), g.id.end(), id);
          it != g.id.end() && *it == id)
        return std::distance(g.id.begin(), it);
      throw std::invalid_argument(std::format("Invalid vertex id {}", id));
    };
    g.src = get_index(g.src);

    for (int u = 1; u <= n; ++u)
      for (int v = 1; v <= n; ++v) {
        is >> g.adj[u][v];

        if (std::abs(g.adj[u][v] - T(99999)) <
            std::numeric_limits<T>::epsilon())
          g.adj[u][v] = g.INF;
      }

    for (int u = 1; u <= n; ++u)
      g.adj[u][u] = 0;
    return g;
  }
};

#endif // !GRAPH_H
