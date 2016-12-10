cmake_minimum_required(VERSION 2.8)

set(LIBUV_ROOT ${THIRD_PARTY_ROOT}/libuv)
set(LIBUV_INCDIR ${LIBUV_ROOT}/include)
set(LIBUV_BIN ${BIN_ROOT}/third_party/libuv)
set(LIBUV_LIB ${LIB_ROOT}/libuv.a)

add_custom_command(OUTPUT ${LIBUV_LIB}
                   COMMAND touch ${LIBUV_LIB})

add_custom_target(targetLibuv DEPENDS ${LIBUV_LIB})
