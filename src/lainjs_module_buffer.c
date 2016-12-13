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
#include "lainjs_module_buffer.h"
#include "lainjs_util.h"

int lainjs_buffer_binding_alloc(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lens == 2);
  assert(duk_is_object(ctx, 0));
  assert(duk_is_number(ctx, 1));

  int length = duk_get_number(ctx, 1);
  char* buffer = lainjs_alloc_char_buffer(length);
  duk_dup(ctx, 0);
  duk_push_pointer(ctx, buffer);
  duk_put_prop_string(ctx, -2, "##native##");

  duk_dup(ctx, 1);
  duk_put_prop_string(ctx, -2, "length");

  duk_pop(ctx);

  duk_dup(ctx, 1);
  return 1;
}

int Write(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lens == 3);
  assert(duk_is_string(ctx, 0));
  assert(duk_is_number(ctx, 1));
  assert(duk_is_number(ctx, 2));

  const char *str = duk_get_string(ctx, 0);
  int offset = duk_get_number(ctx, 1);
  int length = duk_get_number(ctx, 2);

  duk_push_this(ctx);
  duk_get_prop_string(ctx, -1, "##native##");
  char* buffer = (char*)duk_get_pointer(ctx, -1);
  duk_pop(ctx);
  assert(buffer != NULL);

  char buffer_length = duk_get_prop_string(ctx, -1, "length");
  duk_pop(ctx);
  assert(buffer_length >= offset + length);

  for (int i = 0; i < length; ++i) {
    *(buffer + offset + i) = *(str + i);
  }

  duk_dup(ctx, 2);
 
  return 1;
}

int ToString(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lens == 0);

  duk_push_this(ctx);
  duk_get_prop_string(ctx, -1, "##native##");
  char* buffer = (char*)duk_get_pointer(ctx, -1);

  duk_push_string(ctx, buffer);

  return 1;
}

int lainjs_buffer_binding_setup_buffer_js(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lengs == 1);
  assert(duk_is_function(ctx, 0));

  duk_get_prop_string(ctx, 0, "prototype");
  duk_push_c_function(ctx, Write, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "_write");

  duk_push_c_function(ctx, ToString, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "_toString");

  return 0;
}

void lainjs_init_buffer(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_BUFFER);

  STORE_OBJECT_ON_STASH(ctx, module->module)

  FUNC_BINDING_WITH_STASH(ctx, module->module,
                          lainjs_buffer_binding_alloc,
                          "alloc")

  FUNC_BINDING_WITH_STASH(ctx, module->module,
                          lainjs_buffer_binding_setup_buffer_js,
                          "setupBufferJs")
}
