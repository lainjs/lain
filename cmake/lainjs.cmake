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

file(GLOB LAINJS_SRC ${SRC_ROOT}/*.c)

set(LAINJS_CFLAGS ${CFLAGS})
set(LAINJS_INCDI  ${SRC_ROOT}
                  ${LIBDUKTAPE_INCDIR}
                  ${LIBUV_INCDIR})

function(BuildLainjs)
  set(targetName lainjs)
  add_executable(${targetName} ${LAINJS_SRC})

  set_property(TARGET ${targetName}
               PROPERTY COMPILE_FLAGS "${LAINJS_CFLAGS}")
  set_property(TARGET ${targetName}
               PROPERTY LINK_FLAGS "${LAINJS_CFLAGS}")

if((${CMAKE_VERSION} VERSION_GREATER "2.8.9"))
  target_include_directories(${targetName} PRIVATE ${LAINJS_INCDI})
else()
  include_directories(${LAINJS_INCDI})
endif()

  # FIXME
  find_library(M_LIB m)
  target_link_libraries(${targetName} ${LIBUV_LIB} ${LIBDUKTAPE_LIB} ${M_LIB})
endfunction()

BuildLainjs()
