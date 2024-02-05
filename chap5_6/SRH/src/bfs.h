#pragma once

#include <cstddef>
#ifndef BFS_H
#  define BFS_H

#  include "algo_base.h"
#  include "graph.h"

#  include <bitset>
#  include <cstdint>
#  include <queue>
#  include <set>
#  include <vector>

template<typename Weight>
class Bfs {
  const Graph<Weight> &g;

  struct Node {
    std::string path{};
    Weight cost{}, lower_bound{};

    Node(int src_): path(1, src_) {}

    bool operator<(const Node &rhs) const {
      return lower_bound > rhs.lower_bound;
    }
  };

  void update_lower_bound(Node &node) {
    if (static_cast<int>(node.path.size()) == g.n) {
      node.lower_bound = node.cost + g.adj[node.path.back()][g.src];
      return;
    }

    node.lower_bound = 2 * node.cost;

    std::bitset<31> visited;
    for (int u : node.path)
      visited[u] = true;

    auto get_min_elem = [&](int u) -> Weight {
      Weight min_elem = Graph<Weight>::INF;
      for (int v = 1; v <= g.n; ++v)
        if (!visited[v] && g.adj[u][v] < min_elem)
          min_elem = g.adj[u][v];
      return min_elem;
    };
    auto get_sum_two_min_elem = [&](int u) -> Weight {
      Weight min_elem = Graph<Weight>::INF,
             second_min_elem = Graph<Weight>::INF;
      for (int v = 1; v <= g.n; ++v)
        if (v != u) {
          if (g.adj[u][v] < min_elem) {
            second_min_elem = min_elem;
            min_elem = g.adj[u][v];
          } else if (g.adj[u][v] < second_min_elem)
            second_min_elem = g.adj[u][v];
        }
      return min_elem + second_min_elem;
    };

    if (node.path.size() != 1) {
      node.lower_bound += get_min_elem(node.path.front());
      node.lower_bound += get_min_elem(node.path.back());
    } else
      node.lower_bound += get_sum_two_min_elem(node.path.front());

    for (int u = 1; u <= g.n; ++u)
      if (!visited[u])
        node.lower_bound += get_sum_two_min_elem(u);

    node.lower_bound /= 2;
  }

public:
  std::string min_path;
  Weight min_cost{Graph<Weight>::INF}; // global upper bound
  std::int64_t node_count{};

  Bfs(const Graph<Weight> &g_): g(g_) {}

  void run() {
    std::priority_queue<Node> q;
    std::multiset<Weight> lower_bounds;

    Node node(g.src);
    update_lower_bound(node);
    lower_bounds.emplace(node.lower_bound);
    q.push(node);

    while (!q.empty()) {
      Node node = q.top();
      q.pop();

      lower_bounds.erase(lower_bounds.find(node.lower_bound));
      if (node.lower_bound > min_cost)
        continue;
      ++node_count;

      if (static_cast<int>(node.path.size()) == g.n) {
        if (g.adj[node.path.back()][g.src] == g.INF)
          continue;

        node.cost += g.adj[node.path.back()][g.src];
        node.path.reserve(node.path.size() + 1);
        node.path.append(1, g.src);

        if (node.cost < min_cost) {
          min_cost = node.cost;
          min_path = node.path;
          if (node.cost == *lower_bounds.begin())
            break;
        }
        continue;
      }

      int u = node.path.back();
      std::vector<bool> visited(g.n + 1);
      for (int v : node.path)
        visited[v] = true;

      for (int v = 1; v <= g.n; ++v) {
        if (g.adj[u][v] == g.INF || visited[v])
          continue;

        Node new_node = node;
        new_node.path.reserve(new_node.path.size() + 1);
        new_node.path.append(1, v);
        new_node.cost += g.adj[u][v];
        update_lower_bound(new_node);

        if (new_node.lower_bound < min_cost) {
          lower_bounds.emplace(new_node.lower_bound);
          q.push(new_node);
        }
      }
    }
  }
};

template<typename Weight>
Result<Weight> bfs(const Graph<Weight> &g) {
  Bfs<Weight> bfs(g);
  bfs.run();
  return {bfs.min_path, bfs.min_cost, bfs.node_count};
}

#endif // !BFS_H
