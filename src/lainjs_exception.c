#include "lainjs_exception.h"

#include <stdio.h>
#include "uv.h"

char* CreateUVException(int errorno, const char* syscall) {
    static char msg[256];
    snprintf(msg, sizeof(msg), "'%s' %s", syscall, uv_strerror(errorno));
    return msg;
}
