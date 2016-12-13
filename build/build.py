#!/usr/bin/env python

#  Copyright (c) 2016 by Lain.js authors
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

import os
import shutil
import subprocess
import sys
from os import path

config_echo = True

def join_path(pathes):
    return path.abspath(reduce(lambda x, y: path.join(x, y), pathes))


def check_path(path):
    if not os.path.exists(path):
        return False
    return True

#<project root>/build
SCRIPT_PATH = path.dirname(path.abspath(__file__))

#<project root>
ROOT = join_path([SCRIPT_PATH, '../'])

#<project root>/third_party
THIRD_PARTY_ROOT = join_path([ROOT, 'third_party'])

#<project root>/third_party/livu
LIBUV_ROOT = join_path([THIRD_PARTY_ROOT, 'libuv'])

#<project root>/third_party/duktape
DUKTAPE_ROOT = join_path([THIRD_PARTY_ROOT, 'duktape'])

LIBUV_BUILD_SUFFIX = 'third_party/libuv'

DUKTAPE_BUILD_SUFFIX = 'third_party/duktape'

def mkdir(path):
    run_cmd('mkdir', ['-p', path])

def copy(src, dst):
    run_cmd('cp', [src, dst])

def run_cmd(cmd, args = []):
    cmd_line = cmd
    if len(args) > 0:
        cmd_line = cmd_line + " " + reduce(lambda x, y: x + " " + y, args)
    if config_echo:
        print cmd_line
    os.system(cmd_line)

def get_git_hash(path):
    os.chdir(path)
    return subprocess.check_output(['git', 'rev-parse', 'HEAD']).strip()

def sys_name():
    name, _, _, _, _ = os.uname()
    return name.lower()

def sys_machine():
    _, _, _, _, machine = os.uname()
    return machine.lower()

options = {
    'buildtype': 'release',
    'builddir': 'out',
    'buildlib': False,
    'target-arch': sys_machine(),
    'target-os': sys_name(),
    'make-flags': '-j9',
    'nuttx-home': '',
    'init-submodule': True,
    'tidy': True,
}

def opt_build_type():
    return options['buildtype']

def opt_build_root():
    return join_path([ROOT,
                      options['builddir'],
                      opt_target_tuple(),
                      opt_build_type()])

def opt_build_libs():
    return join_path([opt_build_root(), 'libs'])

def opt_target_arch():
    return options['target-arch']

def opt_target_os():
    return options['target-os']

def opt_target_tuple():
    return opt_target_arch() + '-' + opt_target_os()

def opt_make_flags():
    return options['make-flags']

def init_submodule():
    run_cmd('git', ['submodule', 'init'])
    run_cmd('git', ['submodule', 'update'])

def libuv_output_path():
    return join_path([opt_build_libs(), "libuv.a"])

def libduktape_output_path():
    return join_path([opt_build_libs(), "libduktape.a"])

def build_libuv():
    if not check_path(LIBUV_ROOT):
        print 'Error> libuv build failed - submodule not exists.'
        return False
    build_home = join_path([opt_build_root(), LIBUV_BUILD_SUFFIX])

    mkdir(build_home)
    mkdir(join_path([LIBUV_ROOT, 'out']))

    os.chdir(LIBUV_ROOT)

    run_cmd('./gyp_uv.py', ['-f', 'make'])

    build_type = 'Release' if opt_build_type() == 'release' else 'Debug'
    run_cmd('make', ['-C',
                    'out',
                    'BUILDTYPE=' + build_type,
                    opt_make_flags()])

    output = join_path([LIBUV_ROOT, 'out', build_type, 'obj.target', 'libuv.a'])

    mkdir(opt_build_libs())
    copy(output, libuv_output_path())

    return True

def build_duktape():
    if not check_path(DUKTAPE_ROOT):
        print 'Error> duktape build failed - submodule not exists.'
        return False

    build_home = join_path([opt_build_root(), DUKTAPE_BUILD_SUFFIX])

    # TODO : Clean up build for duktape
    mkdir(build_home)
    duktape_build_path = join_path([DUKTAPE_ROOT, 'out'])
    mkdir(duktape_build_path)
    os.chdir(duktape_build_path)
    run_cmd('cmake', [DUKTAPE_ROOT])
    run_cmd('make');

    mkdir(opt_build_libs())
    copy(join_path([duktape_build_path, 'libduktape.a']), libduktape_output_path())

    return True

def build_lain():
    os.chdir(ROOT)
    run_cmd('perl ./build/js2c.pl')

    build_home = join_path([opt_build_root(), 'lainjs'])

    mkdir(build_home)

    build_type = 'Release' if opt_build_type() == 'release' else 'Debug'

    os.chdir(build_home)

    lainjs_cmake_opt = [ROOT, "-DCMAKE_BUILD_TYPE=" + build_type]
    run_cmd('cmake', lainjs_cmake_opt)

    run_cmd('make', [opt_make_flags()])

    return True

if __name__ == '__main__':
    init_submodule()
    mkdir(opt_build_root())
    build_libuv()
    build_duktape()
    if not build_lain():
        sys.exit(1)

    sys.exit(0)
