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

cmake_minimum_required(VERSION 2.8)

set(CC ${CMAKE_C_COMPILER})

set(CFLAGS ${CMAKE_CXX_FLAGS})
set(CFLAGS "${CFLAGS} -pthread")
set(CFLAGS "${CFLAGS} -lm")

set(ROOT ${CMAKE_SOURCE_DIR})
set(SRC_ROOT ${ROOT}/src)
set(THIRD_PARTY_ROOT ${ROOT}/third_party)
set(BIN_ROOT ${CMAKE_BINARY_DIR})
set(LIB_ROOT ${BIN_ROOT}/../libs)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CFLAGS "${CFLAGS} -O2")
else ()
  set(CFLAGS "${CFLAGS} -g")
endif()
