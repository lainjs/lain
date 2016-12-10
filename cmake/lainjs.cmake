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

  target_include_directories(${targetName} PRIVATE ${LAINJS_INCDI})

  # FIXME
  find_library(M_LIB m)
  target_link_libraries(${targetName} ${LIBUV_LIB} ${LIBDUKTAPE_LIB} ${M_LIB})
endfunction()

BuildLainjs()
