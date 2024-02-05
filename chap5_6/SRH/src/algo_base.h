#pragma once

#ifndef ALGO_BASE
#  define ALGO_BASE

#  include <cstdint>
#  include <string>
#  include <tuple>

template<typename Weight>
using Result = std::tuple<std::string, Weight, std::int64_t>;

// 等价于如下的聚合类型定义。将上面的定义修改成下面的，不改变任何其他代码，代码仍然能通过编译并正确运行。

// template<typename Weight>
// struct Result {
//   std::string path{};
//   Weight cost{};
//   std::int64_t node_count{};
// };

#endif // !ALGO_BASE
