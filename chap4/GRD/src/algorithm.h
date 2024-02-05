#pragma once

#ifndef ALGORITHM_H
#  define ALGORITHM_H

#  include "graph.h"

#  include <queue>
#  include <type_traits>
#  include <vector>

// clang-format off
template<typename T>
  requires std::is_arithmetic_v<T>
inline std::pair<std::vector<T>, std::vector<int>>
dijkstra(const Graph<T> &g, int src) {
  // clang-format on
  std::vector<T> dis(g.n + 1, std::numeric_limits<T>::max() / 2);
  std::vector<int> from(g.n + 1, -1);
  std::vector<bool> vis(g.n + 1);

  std::priority_queue<std::pair<T, int>> q;
  q.push({dis[src] = 0, src});
  while (!q.empty()) {
    int u = q.top().second;
    q.pop();
    if (vis[u])
      continue;
    vis[u] = true;
    for (auto [v, w] : g.adj[u])
      if (dis[v] > dis[u] + w) {
        from[v] = u;
        q.push({-(dis[v] = dis[u] + w), v});
      }
  }
  return {dis, from};
}

#endif // !ALGORITHM_H
