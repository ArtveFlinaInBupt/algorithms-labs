// 注：这些头文件全部都用到了。
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <string_view>
#include <tuple>
#include <vector>

namespace MSS {

std::tuple<int, int, int> solve_(const std::vector<int> &seq) {
  int max_sum = 0, max_left = 0, max_right = 0;
  int cur_left = 0, sum = 0;
  for (size_t i = 0; i < seq.size(); ++i) {
    sum += seq[i];
    if (sum > max_sum)
      std::tie(max_sum, max_left, max_right) =
          std::make_tuple(sum, cur_left, i);
    if (sum < 0) {
      sum = 0;
      cur_left = i + 1;
    }
  }
  return {max_sum, max_left, max_right};
}

std::tuple<int, int, int> solve(const std::vector<int> &seq, bool time = true) {
  if (!time)
    return solve(seq);
  
  auto start = std::chrono::high_resolution_clock::now();
  auto ret = solve_(seq);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::format(
      "Data size: {}\nTime elapsed: {} μs\n", seq.size(),
      std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
  );
  return ret;
}

} // namespace MSS

void output(
    const std::string_view &prompt, const std::tuple<int, int, int> &ret
) {
  std::cout << std::format(
      "{} 的最大子段和 ([{}, {}]): {}\n", prompt, std::get<1>(ret),
      std::get<2>(ret), std::get<0>(ret)
  );
  std::cout << std::endl;
};

void problem2() {
  constexpr const static std::string_view SEQ1_FILE_PATH =
      "附件2.最大子段和输入数据-序列1-2023.txt";
  constexpr const static std::string_view SEQ2_FILE_PATH =
      "附件2.最大子段和输入数据-序列2-2023.txt";

  auto read_from_file = [&](const std::string_view &path) -> std::vector<int> {
    std::vector<int> seq;
    std::ifstream fin(path.data());
    for (int n; fin >> n;)
      seq.emplace_back(n);
    return seq;
  };

  std::cout << "\033[32mAssignment 3: Max Subsegment Sum\033[0m\n";
  auto seq1 = read_from_file(SEQ1_FILE_PATH);
  auto seq2 = read_from_file(SEQ2_FILE_PATH);

  output("序列 1", MSS::solve(seq1));
  output("序列 2", MSS::solve(seq2));
}

void extra() {
  auto rand_seq = [](int len) -> std::vector<int> {
    std::vector<int> seq;
    std::generate_n(std::back_inserter(seq), len, [&]() {
      static std::mt19937 gen(std::random_device{}());
      static std::uniform_int_distribution<> dis(-100, 100);
      return dis(gen);
    });
    return seq;
  };

  std::cout << "\033[32mExtra: Random Data\033[0m\n";
  output("随机序列 1", MSS::solve(rand_seq(10000000)));
}

int main() {
  problem2();
  extra();
}
