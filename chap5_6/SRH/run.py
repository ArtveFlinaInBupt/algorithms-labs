#! /usr/bin/env python3
# -*- coding: utf-8 -*-

from dataclasses import dataclass
from typing import List, Literal

import argparse
import os
import subprocess
import sys


SRC_DIR: str = './src'
TARGET_DIR: str = './target'
DATA_DIR: str = './data'

SRC_FILES: List[str] = [
    'main.cpp',
]

silent: bool = False


def print_info(s: str) -> None:
    global silent
    if not silent:
        print(s)


class Argument:
    @dataclass
    class Default:
        CXX: str = 'g++'
        CXXFLAGS: str = '-W -Wall -Wextra -Wpedantic -Wno-psabi -O3 -std=c++20 -Isrc'
        # CXXFLAGS: str = '-W -Wall -Wextra -Wpedantic -Wno-psabi -Og -g -std=c++20 -Isrc'
        LDFLAGS: str = ''

    def __init__(self):
        default = Argument.Default()
        for key in ('CXX', 'CXXFLAGS', 'LDFLAGS'):
            self.__setattr__(key, os.getenv(key))
            if self.__getattribute__(key) is None:
                print_info(
                    f'Env var {key} not set, '
                    f'default to `{default.__getattribute__(key)}`'
                )
                self.__setattr__(key, default.__getattribute__(key))


def run_cmd(*kargs) -> int:
    cmd = ' '.join(kargs)
    print_info(f'\033[1;34mExecuting:\033[0m `{cmd}`')
    sys.stdout.flush()
    ret = subprocess.run(cmd.split()).returncode
    if ret != 0:
        print_info(f'\033[1;31mFailed.\033[0m')
    else:
        print_info(f'\033[1;34mDone.\033[0m')
    return ret


def install_deps() -> None:
    try:
        import xlrd as _
    except ImportError:
        print_info('xlrd not installed, installing...')
        run_cmd(f'{sys.executable} -m pip install xlrd~=2.0.1')


def clean() -> None:
    run_cmd(f'rm -rf {TARGET_DIR}')


def preprocess(amount: int) -> None:
    install_deps()
    run_cmd(
        f'python3 {SRC_DIR}/preprocess.py '
        f'-n {amount} '
        f'-i {DATA_DIR}/附件1-2.基站图的邻接矩阵-v1-2023.xls '
        f'-o {DATA_DIR}/graph.txt '
    )


def compile(type: Literal['dfs', 'bfs']) -> None:
    clean()

    if not os.path.exists(TARGET_DIR):
        os.mkdir(TARGET_DIR)

    arg = Argument()
    CXX: str = arg.CXX  # type: ignore
    CXXFLAGS: str = arg.CXXFLAGS  # type: ignore
    LDFLAGS: str = arg.LDFLAGS  # type: ignore

    if type == 'dfs':
        CXXFLAGS += ' -DDFS'
    else:
        CXXFLAGS += ' -DBFS'

    for src_file in SRC_FILES:
        if run_cmd(
            f'{CXX} {CXXFLAGS} -c {SRC_DIR}/{src_file} '
            f'-o {TARGET_DIR}/{src_file}.o {LDFLAGS}'
        ) != 0:
            exit(1)

    if run_cmd(
        f'{CXX} {CXXFLAGS} '
        f'{" ".join([f"{TARGET_DIR}/{src_file}.o" for src_file in SRC_FILES])}'
        f' -o {TARGET_DIR}/main {LDFLAGS}'
    ) != 0:
        exit(1)


def run() -> None:
    run_cmd(f'{TARGET_DIR}/main {DATA_DIR}/graph.txt')


def main() -> None:
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '-d', '--dfs', type=int, dest='dfs',
        help='run dfs with the given node amount'
    )
    parser.add_argument(
        '-b', '--bfs', type=int, dest='bfs',
        help='run bfs with the given node amount'
    )
    parser.add_argument(
        '-c', '--clean', action='store_true', help='clean the target dir'
    )
    parser.add_argument(
        '-s', '--silent', action='store_true', help='do not print execution info', default=False
    )
    args = parser.parse_args()

    global silent
    silent = args.silent

    if args.dfs and args.bfs:
        print_info('Please specify only one of dfs and bfs')
        exit(1)
    if not any((args.dfs, args.bfs, args.clean)):
        parser.print_help()
        exit(1)

    if args.clean:
        clean()
    elif args.dfs:
        compile('dfs')
        preprocess(args.dfs)
        run()
    elif args.bfs:
        compile('bfs')
        preprocess(args.bfs)
        run()


if __name__ == '__main__':
    main()
