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

static void print(duk_context *ctx, FILE* out_f) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)
  register int index;
  for (index = 0; index < args_lens; index++) {
    duk_trim(ctx, index);
    const char* str = duk_to_string(ctx, index);
    fprintf(out_f, "%s", str);
  }
  fprintf(out_f, "\n");
}

int lainjs_console_binding_log(duk_context *ctx) {
  print(ctx, stdout);
  return 0;
}

void lainjs_init_console(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_CONSOLE);

  lainjs_binding_object_on_stash(ctx, module->module);

  // binding 'lainjs_console_binding_log' on 'console' object.
  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_console_binding_log,
                                        "log");
}
