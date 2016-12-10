cmake_minimum_required(VERSION 2.8)

set(LIBDUKTAPE_ROOT ${THIRD_PARTY_ROOT}/duktape)
set(LIBDUKTAPE_INCDIR ${LIBDUKTAPE_ROOT})
set(LIBDUKTAPE_BIN ${BIN_ROOT}/third_party/libduktape)
set(LIBDUKTAPE_LIB ${LIB_ROOT}/libduktape.a)

add_custom_command(OUTPUT ${LIBDUKTAPE_BIN}
                   COMMAND touch ${LIBDUKTAPE_LIB})

add_custom_target(targetLibduktap DEPENDS ${LIBDUKTAPE_LIB})
