#! /usr/bin/env python3
# -*- coding: utf-8 -*-


from typing import Dict, List

import argparse
import dataclasses
import sys
import xlrd


@dataclasses.dataclass
class AdjMat:
    n: int
    src: int
    id: List[int]
    weight: List[List[float]]


def read_adj_mat(path: str, n: int, sheet_index: int) -> AdjMat:
    book = xlrd.open_workbook(path)
    sheet = book.sheet_by_index(sheet_index)

    first_row = sheet.row_values(0)
    # n = int(max(first_row[2:]))

    second_row = sheet.row_values(1)
    id = list(map(int, second_row[2:]))

    weight = []
    for i in range(2, n + 2):
        row = sheet.row_values(i)
        weight.append(list(row[2:]))

    # inject
    src = 567443
    if src not in id:
        src = id[0]

    return AdjMat(n, src, id, weight)


def write_adj_mat(path: str, graph: AdjMat) -> None:
    buf = []

    buf.append(f'{graph.n} {graph.src}\n')
    buf.append(' '.join(map(str, graph.id)))
    buf.append('\n')
    for row in graph.weight:
        buf.append(' '.join(map(str, row)))
        buf.append('\n')

    with open(path, 'w') as f:
        f.writelines(buf)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--amount', dest='amount', type=int)
    parser.add_argument('-i', '--input', dest='file_path', type=str)
    parser.add_argument('-o', '--output', dest='output_path', type=str)
    args = parser.parse_args()

    VALID_AMOUNT: Dict[int, int] = {
        15: 0,
        20: 1,
        22: 2,
        30: 3,
        5: 4,
    }

    if not args.amount or not args.file_path or not args.output_path:
        parser.print_help()
        exit(1)

    if not args.amount or args.amount not in VALID_AMOUNT:
        sys.stderr.write(
            'Must specify amount with one of the following values: '
            f'{", ".join(map(str, VALID_AMOUNT.keys()))}\n'
        )
        exit(1)

    write_adj_mat(
        args.output_path,
        read_adj_mat(args.file_path, args.amount, VALID_AMOUNT[args.amount])
    )


if __name__ == '__main__':
    main()
