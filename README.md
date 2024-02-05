# Algorithm-labs

这是北京邮电大学计算机科学与技术专业 2021 级大三上（2023–2024 学年度秋季学期）《算法设计与分析》课程小作业（上机实验）。注意：这门课每个老师布置的小作业不同。

文件结构和原始提交的毫无关系，所以和报告对不上。其中包含了作业提供的数据，但没有包含作业题目。

实验数据并非本人生产，若有版权问题请联系我从仓库中删除。If there is any copyright issue concerning the data, please contact me to get them removed from the repository.

| 章节                           | 项目名 | 选择的题目                     | 其他备选的题目<sup>（与本仓库无关）</sup> |
| ------------------------------ | ------ | ------------------------------ | ----------------------------------------- |
| 第二章 分治                    | PNP    | 平面最近点对[^1]               | 快速选择                                  |
| 第三章 动态规划                | DP     | 最长公共子序列、最大子段和[^2] | 凸多边形最优三角剖分[^3]                  |
| 第四章 贪心                    | GRD    | 单源最短路径                   | 最小生成树、哈夫曼编码                    |
| 第五章 回溯<br>第六章 分支限界 | SRH    | “TSP 问题”[^4]                 |                                           |

[^1]: 叶文一拍大腿搞了个球面最近点对的题目背景，导致这题压根就是错题。平面最近点对算法实际上不能正确求解这个问题，需要上空间最近点对算法。不过他给的数据非常水，所以随便咋写都能跑出一样的答案来。
[^2]: 老实说，把最大子段和的一眼做法说成 dp 简直是纯扯淡。
[^3]: 这啥啊？
[^4]: 叶文压根不知道 TSP 是什么，他出的这个作业题实际上是求一般简单无向图上的 Hamilton 回路。[^5][^6]
[^5]: 其他班的同学在课程群指出他的错误被他扣了一通问号，他好像到最后也没相信自己是错的。
[^6]: 虽然他对 TSP 原问题的认识是错误的，但是他的课件里抄来的内容是解决正儿八经 TSP 的问题的算法。这些算法中有的不能应用在求一般图的 Hamilton 回路上（或者说，可以套上去应用但没有实用意义），但他对此浑然不知。
