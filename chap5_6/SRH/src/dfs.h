#pragma once

#ifndef DFS_H
#  define DFS_H

#  include "algo_base.h"
#  include "graph.h"

#  include <cstdint>
#  include <numeric>
#include <bitset>

template<typename Weight>
class Dfs {
  const Graph<Weight> &g;
  std::string path{};
  std::bitset<31> visited{};
  Weight cost;

public:
  std::string min_path{};
  Weight min_cost{Graph<Weight>::INF};
  std::int64_t node_count{};

  Dfs(const Graph<Weight> &g_): g(g_) {
    path.append(1, g.src);
    visited.set(g.src);
  }

  void run() {
    ++node_count;
    int u = path.back();

    if (static_cast<int>(path.size()) == g.n) {
      if (g.adj[u][g.src] == g.INF)
        return;

      path.append(1, g.src);
      cost += g.adj[u][g.src];

      if (cost < min_cost) {
        min_cost = cost;
        min_path = path;
      }

      cost -= g.adj[u][g.src];
      path.pop_back();
      return;
    }

    for (int v = 1; v <= g.n; ++v) {
      if (u == v || g.adj[u][v] == g.INF || visited[v] ||
          cost + g.adj[u][v] >= min_cost)
        continue;

      path.append(1, v);
      cost += g.adj[u][v];
      visited.set(v);
      run();
      visited.reset(v);
      cost -= g.adj[u][v];
      path.pop_back();
    }
  }
};

template<typename Weight>
Result<Weight> dfs(const Graph<Weight> &g) {
  Dfs<Weight> dfs(g);
  dfs.run();
  return {dfs.min_path, dfs.min_cost, dfs.node_count};
}

#endif // !DFS_H
