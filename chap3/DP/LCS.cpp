// 注：这些头文件全部都用到了。
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace LCS {

enum class Direction : std::uint8_t { Left = 1, Up, LeftUp };

std::vector<std::vector<int>> f;
std::vector<std::vector<Direction>> dir;

template<typename T>
std::vector<T> _solve(const std::vector<T> &a, const std::vector<T> &b) {
  int n = a.size() - 1, m = b.size() - 1;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
      if (a[i] == b[j]) {
        f[i][j] = f[i - 1][j - 1] + 1;
        dir[i][j] = Direction::LeftUp;
      } else if (f[i - 1][j] > f[i][j - 1]) {
        f[i][j] = f[i - 1][j];
        dir[i][j] = Direction::Up;
      } else {
        f[i][j] = f[i][j - 1];
        dir[i][j] = Direction::Left;
      }
    }

  std::vector<T> ans;
  for (int i = n, j = m; i && j;) {
    switch (dir[i][j]) {
      case Direction::LeftUp:
        ans.emplace_back(a[i--]);
        /* fallthrough */
      case Direction::Left:
        --j;
        break;
      case Direction::Up:
        --i;
        break;
      default:
        throw std::runtime_error("Should not reach here!");
    }
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}

template<typename T>
std::vector<T> solve(std::vector<T> a, std::vector<T> b, bool time = true) {
  a.reserve(a.size() + 1);
  b.reserve(b.size() + 1);
  a.insert(a.begin(), T{});
  b.insert(b.begin(), T{});

  f.resize(a.size() + 1);
  dir.resize(a.size() + 1);
  for (size_t i = 0; i <= a.size(); ++i) {
    f[i].resize(b.size() + 1);
    dir[i].resize(b.size() + 1);
  }

  if (!time)
    return _solve(a, b);

  auto start = std::chrono::high_resolution_clock::now();
  auto ans = _solve(a, b);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::format(
      "Data size: {} * {}\nTime elapsed: {} μs\n", a.size() - 1, b.size() - 1,
      std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
  );
  return ans;
}

std::string
solve(const std::string &a_, const std::string &b_, bool time = true) {
  std::vector<char> a(a_.begin(), a_.end());
  std::vector<char> b(b_.begin(), b_.end());
  a.reserve(a.size() + 1);
  b.reserve(b.size() + 1);
  a.insert(a.begin(), '\0');
  b.insert(b.begin(), '\0');

  f.resize(a.size() + 1);
  dir.resize(a.size() + 1);
  for (size_t i = 0; i <= a.size(); ++i) {
    f[i].resize(b.size() + 1);
    dir[i].resize(b.size() + 1);
  }

  if (!time) {
    auto ans = _solve(a, b);
    return std::string(ans.begin(), ans.end());
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto ans = _solve(a, b);
  auto anss = std::string(ans.begin(), ans.end());
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::format(
      "Data size: {} * {}\nTime elapsed: {} μs\n", a.size() - 1, b.size() - 1,
      std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
  );
  return anss;
}

} // namespace LCS

namespace LDS {

template<typename T>
std::vector<T> solve(const std::vector<T> &seq, bool time = true) {
  auto start = std::chrono::high_resolution_clock::now();
  auto sorted = seq;
  std::sort(sorted.rbegin(), sorted.rend());

  // 取消注释以下两行以求得最长严格下降子序列
  // auto it = std::unique(sorted.rbegin(), sorted.rend());
  // sorted.erase(sorted.begin(), it.base());

  auto ans = LCS::solve(seq, sorted, false);

  if (time) {
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::format(
        "Data size: {}\nTime elapsed: {} μs\n", seq.size(),
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
    );
  }

  return ans;
}

} // namespace LDS

void output(const std::string_view &prompt, const std::string &s) {
  std::cout << std::format("{} [长度 {}]: {}\n", prompt, s.size(), s)
            << std::endl;
}

void problem1_1() {
  constexpr const static std::string_view FILE_PATH =
      "附件1.最长公共子序列输入文件-2023.txt";

  std::cout << "\033[32mProblem 1.1\n\033[0m";
  std::ifstream fin(FILE_PATH.data());
  std::string a, b, c, d;
  fin >> a >> a;
  fin >> b >> b;
  fin >> c >> c;
  fin >> d >> d;

  output("A - B 的 LCS", LCS::solve(a, b));
  output("C - D 的 LCS", LCS::solve(c, d));
  output("A - D 的 LCS", LCS::solve(a, d));
  output("C - B 的 LCS", LCS::solve(c, b));
}

void problem1_2() {
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

  auto to_string = [](const std::vector<int> &seq) -> std::string {
    std::string s = std::format("[长度 {}] {{", seq.size());
    for (int n : seq)
      s += std::format("{}, ", n);
    s.pop_back();
    s.pop_back();
    s.append("}");
    return s;
  };

  std::cout << "\033[32mProblem 1.2: LDS\033[0m\n";

  auto seq1 = read_from_file(SEQ1_FILE_PATH);
  auto seq2 = read_from_file(SEQ2_FILE_PATH);

  std::cout << std::format("序列 1 的 LDS: {}\n", to_string(LDS::solve(seq1)))
            << std::endl;
  std::cout << std::format("序列 2 的 LDS: {}\n", to_string(LDS::solve(seq2)))
            << std::endl;
}

void extra() {
  auto rand_string = [](int len) -> std::string {
    std::string s;
    s.reserve(len);
    std::generate_n(std::back_inserter(s), len, [&]() -> char {
      static std::mt19937 gen(std::random_device{}());
      static std::uniform_int_distribution<> dis(33, 126);
      return static_cast<char>(dis(gen));
    });
    return s;
  };

  std::cout << "\033[32mProblem Extra: Random Data\033[0m\n";
  std::string a = rand_string(10000);
  std::string b = rand_string(10000);
  output("Extra Test (LCS)", LCS::solve(a, b));
}

int main() {
  problem1_1();
  problem1_2();
  extra();
}
