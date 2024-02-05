#! /usr/bin/env python3
# -*- coding: utf-8 -*-

from dataclasses import dataclass
from typing import List

import argparse
import os
import subprocess
import sys


SRC_DIR = './src'
TARGET_DIR = './target'
DATA_DIR = './data'

SRC_FILES = [
    'main.cpp',
    'reader.cpp',
    'algorithm.cpp',
]


class Argument:
    @dataclass
    class Default:
        CXX: str = 'g++'
        CXXFLAGS: str = '-W -Wall -Wextra -Wpedantic -O3 -std=c++20 -Isrc'
        LDFLAGS: str = ''

    def __init__(self):
        default = Argument.Default()
        for key in ('CXX', 'CXXFLAGS', 'LDFLAGS'):
            self.__setattr__(key, os.getenv(key))
            if self.__getattribute__(key) is None:
                print(
                    f'Env var {key} not set, default to `{default.__getattribute__(key)}`'
                )
                self.__setattr__(key, default.__getattribute__(key))


def run_cmd(*kargs) -> int:
    cmd = ' '.join(kargs)
    print(f'\033[34mExecuting:\033[0m `{cmd}`', end=' ')
    sys.stdout.flush()
    ret = subprocess.run(cmd.split()).returncode
    if ret != 0:
        print(f'\033[31mFailed.\033[0m')
    else:
        print(f'\033[34mDone.\033[0m')
    return ret


def install_deps() -> None:
    try:
        import xlrd as _
    except ImportError:
        print('xlrd not installed, installing...')
        run_cmd(f'{sys.executable} -m pip install xlrd~=2.0.1')


def clean() -> None:
    run_cmd(f'rm -rf {TARGET_DIR}')


def preprocess() -> None:
    install_deps()
    run_cmd(f'python3 {SRC_DIR}/preprocess.py')


def compile() -> None:
    clean()

    if not os.path.exists(TARGET_DIR):
        os.mkdir(TARGET_DIR)

    arg = Argument()
    CXX: str = arg.CXX  # type: ignore
    CXXFLAGS: List[str] = arg.CXXFLAGS  # type: ignore
    LDFLAGS: List[str] = arg.LDFLAGS  # type: ignore

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


def run(compiled: bool) -> None:
    if not compiled or not os.path.exists(TARGET_DIR) \
            or not os.path.exists(f'{TARGET_DIR}/main'):
        print('Target dir not found, compiling...')
        compile()

    run_cmd(f'{TARGET_DIR}/main {DATA_DIR}/stations.txt')


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-c', '--compile', action='store_true', help='compile the C++ code'
    )
    parser.add_argument(
        '-p', '--preprocess', action='store_true', help='preprocess the data'
    )
    parser.add_argument(
        '-r', '--run', action='store_true', help='run the program'
    )
    parser.add_argument(
        '--clean', action='store_true', help='clean the target dir'
    )
    args = parser.parse_args()

    if args.clean:
        clean()

    if args.preprocess:
        preprocess()
    if args.compile:
        compile()
    if args.run:
        run(args.compile)

    if not any([args.preprocess, args.compile, args.run, args.clean]):
        parser.print_help()


if __name__ == '__main__':
    main()
