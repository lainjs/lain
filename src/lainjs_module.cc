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

#include <assert.h>

#include "lainjs_module.h"

static module _modules[MODULE_COUNT];

void lainjs_init_modules(struct v7 *v) {
  // MODULE_CONSOLE
  _modules[MODULE_CONSOLE].kind = MODULE_CONSOLE;
  _modules[MODULE_CONSOLE].module = v7_mk_object(v);

  // MODULE_PROCESS
  _modules[MODULE_PROCESS].kind = MODULE_PROCESS;
  _modules[MODULE_PROCESS].module = v7_mk_object(v);

  // MODULE_TIMER
  _modules[MODULE_TIMER].kind = MODULE_PROCESS;
  _modules[MODULE_TIMER].module = v7_mk_object(v);
}

module* lainjs_get_builtin_module(lainjs_modules kind) {
  assert(kind < MODULE_COUNT);
  return &_modules[kind];
}
