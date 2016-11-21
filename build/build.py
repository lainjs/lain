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

def run_cmd(cmd, args = []):
    cmd_line = cmd
    if len(args) > 0:
        cmd_line = cmd_line + " " + reduce(lambda x, y: x + " " + y, args)
    if config_echo:
        print cmd_line
    os.system(cmd_line)

def init_submodule():
    run_cmd('git', ['submodule', 'init'])
    run_cmd('git', ['submodule', 'update'])

def build_lain():
    run_cmd('perl ./build/js2c.pl');
    #make ninja files
    run_cmd("gyp all.gyp --depth=. -f ninja -Duv_library='static_library'");

    #build lain
    run_cmd("ninja -C ./out/Default lainjs");
    return True

if __name__ == '__main__':
    init_submodule();
    if not build_lain():
        sys.exit(1)

    sys.exit(0)
