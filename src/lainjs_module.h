/* Copyright (c) 2016 by Lain.js authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LAINJS_MODULE_H
#define LAINJS_MODULE_H
#include "lainjs_binding.h"

typedef enum {
  MODULE_PROCESS,
  MODULE_CONSOLE,
  MODULE_BUFFER,
  MODULE_CONSTANTS,
  MODULE_FS,
  MODULE_TIMER,
  MODULE_TCP,
  MODULE_COUNT,
} lainjs_modules;

typedef void (*register_function)(duk_context *ctx);

typedef struct {
  lainjs_modules kind;
  char module[20];
  register_function register_func;
} module;

void lainjs_init_modules(duk_context *ctx);
module* lainjs_get_builtin_module(lainjs_modules kind);
#endif
