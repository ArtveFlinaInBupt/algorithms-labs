#! /usr/bin/env python3
# -*- coding: utf-8 -*-


from typing import List, Tuple
from cyaron import Graph

import argparse
import dataclasses
import sys
import time
import xlrd


@dataclasses.dataclass
class AdjMat:
    n: int
    src: int
    id: List[int]
    weight: List[List[float]]


def read_adj_mat(path: str) -> List[AdjMat]:
    book = xlrd.open_workbook(path)

    graphs = []
    for sheet_index in (0, 1):
        sheet = book.sheet_by_index(sheet_index)
        first_row = sheet.row_values(0)
        n = int(max(first_row[2:]))

        second_row = sheet.row_values(1)
        id = list(map(int, second_row[2:]))

        weight = []
        for i in range(2, n + 2):
            row = sheet.row_values(i)
            weight.append(list(row[2:]))

        # inject
        if n == 22:
            src = 567443
        elif n == 42:
            src = 565845
        else:
            src = id[0]

        graphs.append(AdjMat(n, src, id, weight))
    return graphs


def write_adj_mat(path: str, data: List[AdjMat]) -> None:
    buf = []
    buf.append(f'{len(data)}\n')
    for graph in data:
        buf.append(f'{graph.n} {graph.src}\n')
        buf.append(' '.join(map(str, graph.id)))
        buf.append('\n')
        for row in graph.weight:
            buf.append(' '.join(map(str, row)))
            buf.append('\n')

    with open(path, 'w') as f:
        f.writelines(buf)


def gen_cyaron_data(testcases: int) -> List[Tuple[int, int, Graph]]:
    N = 300000
    M = 1000000
    
    print(f'Generating {testcases} graphs with {N} nodes and {M} edges...')
    return [(N, M, Graph.graph(
        N, M, self_loop=False, repeated_edges=False, directed=True,
        weight_limit=(((1 << 30) - 1) // M)
    )) for _ in range(testcases)]


def write_cyaron_data(path: str, data: List[Tuple[int, int, Graph]]) -> None:
    buf = []
    buf.append(f'{len(data)}\n')
    for (n, m, graph) in data:
        buf.append(f'{n} {m} 1\n')
        # buf.append(' '.join(map(str, range(1, n + 1))))
        # buf.append('\n')
        for edge in graph.iterate_edges():
            buf.append(f'{edge.start} {edge.end} {edge.weight}\n')

    with open(path, 'w') as f:
        f.writelines(buf)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--testcases', dest='testcases', type=int)
    parser.add_argument('-i', '--input', dest='file_path', type=str)
    parser.add_argument('-o', '--output', dest='output_path', type=str)
    args = parser.parse_args()

    if args.testcases and args.file_path:
        sys.stderr.write('Cannot specify both testcases and file_path\n')
        parser.print_help()
    elif not args.testcases and not args.file_path:
        sys.stderr.write('Must specify either testcases or file_path\n')
        parser.print_help()
    elif args.testcases:
        print(
            '\033[33mGenerating data... '
            'Please wait, this could take a while.\033[0m'
        )
        start = time.time()

        write_cyaron_data(args.output_path, gen_cyaron_data(args.testcases))

        time_cost = time.time() - start
        if time_cost <= 5:
            print(
                f'\033[33mData generated in {time_cost:.2f} second(s).\033[0m'
            )
        else:
            print(
                f'\033[33mData generated in {time_cost:.2f} second(s). '
                'Sorry for the delay!\033[0m'
            )
    elif args.file_path:
        write_adj_mat(args.output_path, read_adj_mat(args.file_path))


if __name__ == '__main__':
    main()
