#pragma once

#ifndef GRAPH_H
#  define GRAPH_H

#  include <iostream>
#  include <type_traits>
#  include <vector>

template<typename T>
  requires std::is_arithmetic_v<T>
struct Graph {
  int n, src;
  std::vector<int> id;
  std::vector<std::vector<std::pair<int, T>>> adj;

  void push_edge(int u, int v, T w) {
    adj[u].push_back({v, w});
  }

  Graph(int n_, int src_): n(n_), src(src_) {
    if (n <= 100)
      id.resize(n + 1);
    adj.resize(n + 1);
  }

  static std::vector<Graph<T>> read_from_adjacency_matrix(std::istream &is) {
    std::vector<Graph<T>> graphs;
    int t;
    for (is >> t; t; --t) {
      int n, src;
      is >> n >> src;
      Graph<T> g(n, src);
      for (int u = 1; u <= n; ++u)
        is >> g.id[u];
      for (int u = 1; u <= n; ++u)
        for (int v = 1; v <= n; ++v) {
          T w;
          is >> w;
          if (w != T(-1))
            g.push_edge(u, v, w);
        }
      graphs.push_back(g);
    }
    return graphs;
  }

  static std::vector<Graph<T>> read_from_edge_list(std::istream &is) {
    std::vector<Graph<T>> graphs;
    int t;
    for (is >> t; t; --t) {
      int n, m, src;
      is >> n >> m >> src;
      Graph<T> g(n, src);
      // for (int u = 1; u <= n; ++u)
      //   is >> g.id[u];
      for (; m; --m) {
        int u, v;
        T w;
        is >> u >> v >> w;
        g.push_edge(u, v, w);
      }
      graphs.push_back(g);
    }
    return graphs;
  }
};

#endif // !GRAPH_H
