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
  duk_push_global_object(ctx);
  duk_push_object(ctx);
  duk_put_prop_string(ctx, -2, module->module);
  duk_pop(ctx);
}
