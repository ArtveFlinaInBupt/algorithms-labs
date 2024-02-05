#! /usr/bin/env python3
# -*- coding: utf-8 -*-


from typing import List

import dataclasses
import xlrd


@dataclasses.dataclass
class Entry:
    enodebid: int
    longitude: str
    latitude: str


def read_data(path: str) -> List[Entry]:
    data: List[Entry] = []
    sheet = xlrd.open_workbook(path).sheet_by_index(0)
    for i in range(1, sheet.nrows):
        row = sheet.row_values(i)
        data.append(Entry(int(row[0]), row[1], row[2]))
    return data


def write_data(path: str, data: List[Entry]) -> None:
    with open(path, 'w') as f:
        for entry in data:
            f.write(f'{entry.enodebid}\t{entry.longitude}\t{entry.latitude}\n')


def main() -> None:
    data = read_data('./data/02-1 1033个基站数据.xls')
    write_data('./data/stations.txt', data)


if __name__ == '__main__':
    main()