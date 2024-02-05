#include "algorithm.h"
#include "graph.h"

// 这些头文件全部都用到了
// 让我们感谢 C++ STL 的基础设施
// 我给 STL 恭恭敬敬磕大头
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

template<typename T>
void algo_main(const Graph<T> &g) {
  auto start = std::chrono::high_resolution_clock::now();

  auto get_index = [&](int id) -> int {
    if (g.n > 100)
      return id;

    if (auto it = std::find(g.id.begin(), g.id.end(), id);
        it != g.id.end() && *it == id)
      return it - g.id.begin();
    throw std::invalid_argument(std::format("Invalid vertex id {}", id));
  };

  auto [dis, from] = dijkstra(g, get_index(g.src));
  auto end = std::chrono::high_resolution_clock::now();

  std::string buf = std::format(
      "Time Elapsed: {:.3f} ms\n",
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count()
      ) / 1000
  );

  if (g.n > 100) {
    int max_index = 1;
    for (int i = 1; i <= g.n; ++i)
      if (dis[i] != std::numeric_limits<T>::max() / 2 &&
          dis[i] > dis[max_index])
        max_index = i;

    buf += std::format(
        "The vertex can be reached from {} with longest shortest path is {} "
        "with length {}:\n",
        g.src, max_index, dis[max_index]
    );
    std::vector<int> path;
    for (int v = max_index; v != -1; v = from[v])
      path.push_back(v);
    for (auto it = path.rbegin(); it != path.rend(); ++it)
      buf += std::format(" {} ->", *it);
    buf.pop_back(), buf.pop_back(), buf.pop_back();
    buf += "\n";
    std::cout << buf << std::endl;
    return;
  }

  buf += std::format("Shortest path from {}: {{\n", g.src);
  for (int u = 1; u <= g.n; ++u) {
    if (dis[u] != std::numeric_limits<T>::max() / 2) {
      if constexpr (std::is_integral_v<T>)
        buf += std::format("  {:6}: [length {}]", g.id[u], dis[u]);
      else if (std::is_floating_point_v<T>)
        buf += std::format("  {:6}: [length {:8.3f}]", g.id[u], dis[u]);

      std::vector<int> path;
      for (int v = u; v != -1; v = from[v])
        path.push_back(g.id[v]);
      for (auto it = path.rbegin(); it != path.rend(); ++it)
        buf += std::format(" {} ->", *it);
      buf.pop_back(), buf.pop_back(), buf.pop_back();
    } else {
      buf += std::format("  {}: [unreachable] ", g.id[u]);
    }
    buf += "\n";
  }
  buf += "}\n";
  std::cout << buf << std::endl;
}

template<typename T>
void algo_main(const std::vector<Graph<T>> &graphs) {
  std::cout << std::format("Found {} graph(s)\n", graphs.size());
  for (size_t i = 0; i < graphs.size(); ++i) {
    std::cout << std::format("\033[32mGraph #{}\033[0m\n", i + 1);
    algo_main(graphs[i]);
  }
}

enum class InputType : std::uint8_t {
  AdjacencyMatrix = 1,
  EdgeList = 2,
};

/// @param argc
/// @param argv {input file, input type}
int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input file> <input type>\n";
    return EXIT_FAILURE;
  }
  char *input_file = argv[1];
  InputType input_type = static_cast<InputType>(std::stoi(argv[2]));

  std::ifstream ifs(input_file);
  if (!ifs) {
    throw std::runtime_error("Failed to open input file");
  }

  switch (input_type) {
    case InputType::AdjacencyMatrix:
      algo_main(Graph<double>::read_from_adjacency_matrix(ifs));
      break;
    case InputType::EdgeList:
      algo_main(Graph<int>::read_from_edge_list(ifs));
      break;
    default:
      throw std::invalid_argument("Invalid input type");
  }
}
