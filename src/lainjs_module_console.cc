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

#include <stdio.h>
#include <assert.h>

#include "lainjs_binding.h"
#include "lainjs_module.h"
#include "lainjs_module_console.h"

static enum v7_err print(struct v7 *v, v7_val_t *res, FILE* out_f) {
  unsigned long args_lens = v7_argc(v);
  for (register int index = 0; index < args_lens; index++) {
    v7_val_t arg = v7_arg(v, index);
    size_t n;
    fprintf(out_f, "%s ", v7_get_string(v, &arg, &n));
  }
  fprintf(out_f, "\n");
  return V7_OK;
}

static enum v7_err log(struct v7 *v, v7_val_t *res) {
  return print(v, res, stdout);
}

void lainjs_init_console_module(struct v7 *v) {
  module* module = lainjs_get_builtin_module(MODULE_CONSOLE);
  assert(v7_is_object(module->module));
  v7_def(v, v7_get_global(v), "console", ~0, V7_DESC_ENUMERABLE(0), module->module);
  v7_set_method(v, module->module, "log", &log);
}
