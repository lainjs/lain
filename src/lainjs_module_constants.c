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

#include "uv.h"

#include "lainjs_binding.h"
#include "lainjs_module.h"
#include "lainjs_module_fs.h"

void lainjs_init_constants(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_CONSTANTS);

  lainjs_binding_object_on_stash(ctx, module->module);

  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, module->module);

  duk_push_int(ctx, O_APPEND);
  duk_put_prop_string(ctx, -2, "O_APPEND");
  duk_push_int(ctx, O_CREAT);
  duk_put_prop_string(ctx, -2, "O_CREAT");
  duk_push_int(ctx, O_EXCL);
  duk_put_prop_string(ctx, -2, "O_EXCL");
  duk_push_int(ctx, O_RDONLY);
  duk_put_prop_string(ctx, -2, "O_RDONLY");
  duk_push_int(ctx, O_RDWR);
  duk_put_prop_string(ctx, -2, "O_RDWR");
  duk_push_int(ctx, O_SYNC);
  duk_put_prop_string(ctx, -2, "O_SYNC");
  duk_push_int(ctx, O_TRUNC);
  duk_put_prop_string(ctx, -2, "O_TRUNC");
  duk_push_int(ctx, O_WRONLY);
  duk_put_prop_string(ctx, -2, "O_WRONLY");

  duk_pop(ctx);
}
