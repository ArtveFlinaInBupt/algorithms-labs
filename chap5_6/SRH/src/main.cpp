#if __cplusplus < 202002L
#  error "请使用 C++20 或更新的语言标准编译此程序"
#endif

#include "algo_base.h"
#include "bfs.h"
#include "dfs.h"
#include "graph.h"
#include "mem.h"

// 所有包含的头文件都用到了，
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <format>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

using namespace std::string_view_literals;

constexpr const std::string_view PATH = "data/graph.txt"sv;

template<typename Weight>
void algo_main(const Graph<Weight> &g) {
  auto start = std::chrono::high_resolution_clock::now();
  std::atomic<bool> recording_mem(true);
  std::future<unsigned long> mem_usage_monitor = std::async(
      std::launch::async, monitor_mem_usage, std::ref(recording_mem)
  );

  auto [path, cost, node_count] =
#ifdef DFS
      dfs
#else // including #if defined(BFS)
      bfs
#endif
      (g);

  auto path_to_string = [&](const std::string &path) {
    std::string buf;
    for (int u : path)
      buf += std::format("{} -> ", g.id[u]);
    if (!buf.empty())
      buf.pop_back(), buf.pop_back(), buf.pop_back();
    return buf;
  };

  auto end = std::chrono::high_resolution_clock::now();
  recording_mem = false;
  auto mem_usage = mem_usage_monitor.get();

  std::cout << std::format(
      "Time Elapsed: {:.3f} ms\nMemory Usage: {:.3f} MB\nNode Traverse Count: "
      "{}\nTSP Solution:\n  Path with cost {:.3f}: {}\n",
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count()
      ) / 1000,
      static_cast<double>(mem_usage) / 1024 / 1024, node_count, cost,
      path_to_string(path)
  );
  std::cout << std::endl;
}

int main() {
  auto fin = std::ifstream(PATH.data());
  if (!fin.is_open()) {
    std::cerr << "Failed to open file: " << PATH << std::endl;
    return 1;
  }

  algo_main(Graph<double>::read_from(fin));
}
