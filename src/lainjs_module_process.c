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
#include "lainjs_module_process.h"

int lainjs_process_binding_binding(duk_context *ctx) {
  assert(duk_get_top(ctx) == 1);
  int kind = (int)duk_to_number(ctx, 0);
  module* module = lainjs_get_builtin_module(kind);

  if (!module->obj)
    if (module->register_func)
      module->register_func(ctx);
    else  // FIXME
      return 0;

  duk_push_heapptr(ctx, module->obj);
  return 1;
}

int lainjs_process_binding_compile(duk_context *ctx) {
  return 0;
}

int lainjs_process_binding_read_source(duk_context *ctx) {
  return 0;
}

void lainjs_on_next_tick(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, module->module);
  duk_get_prop_string(ctx, -1, "onNextTick");

  if (duk_is_callable(ctx, -1))
    duk_call(ctx, 0);

  duk_pop_2(ctx);
}

void lainjs_init_process_module(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);

  if (!module->obj) {
    duk_push_global_stash(ctx);
    duk_push_object(ctx);
    module->obj = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, module->module);
    duk_pop(ctx);

    duk_push_heapptr(ctx, module->obj);
    duk_push_c_function(ctx, lainjs_process_binding_binding, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "binding");
    duk_pop(ctx);

    duk_push_heapptr(ctx, module->obj);
    duk_push_c_function(ctx, lainjs_process_binding_compile, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "compile");
    duk_pop(ctx);

    duk_push_heapptr(ctx, module->obj);
    duk_push_c_function(ctx, lainjs_process_binding_read_source, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "readSource");
    duk_pop(ctx);

    duk_push_global_object(ctx);
    duk_push_heapptr(ctx, module->obj);
    duk_put_prop_string(ctx, -2, module->module);
    duk_pop(ctx);
  }
}
