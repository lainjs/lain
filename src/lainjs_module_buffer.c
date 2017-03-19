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

int Write(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)
  assert(args_lens == 3);
  assert(duk_is_string(ctx, 0));
  assert(duk_is_number(ctx, 1));
  assert(duk_is_number(ctx, 2));

  JS_GET_STRING(0, str)
  JS_GET_INT(1, offset)
  JS_GET_INT(2, length)

  JS_GET_NATIVE_OBJECT_ON_THIS(char* buffer)
  assert(buffer != NULL);

  JS_GET_INT_ON_OBJECT_OF_THIS(buffer_length, "length")
  assert(buffer_length >= offset + length);

  int i;
  for (i = 0; i < length; ++i) {
    *(buffer + offset + i) = *(str + i);
  }

  JS_PUSH_INT(length)
 
  return 1;
}

int ToString(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lens == 0);

  JS_GET_NATIVE_OBJECT_ON_THIS(char *buffer)
  JS_PUSH_STRING(buffer)

  return 1;
}

int Copy(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);

  // target
  JS_GET_NATIVE_OBJECT_ON_INDEX(0, char *target_buffer)
  JS_GET_INT_ON_INDEX(0,  target_length, "length")

  // source
  JS_GET_NATIVE_OBJECT_ON_THIS(char* source_buffer)
  JS_GET_INT_ON_THIS(source_length, "length")

  int target_start = 0;
  int source_start = 0;
  int source_end = source_length;

  if (args_lens >= 2) {
    JS_GET_INT_WITHOUT_TYPE_DEFINE(1, target_start)
  }

  if (args_lens >= 3) {
    JS_GET_INT_WITHOUT_TYPE_DEFINE(2, source_start)
  }

  if (args_lens >= 4) {
    JS_GET_INT_WITHOUT_TYPE_DEFINE(3, source_end)
    if (source_end > source_length) {
      source_end = source_length;
    }
  }

  int i, j;
  int copied = 0;
  for (i = source_start, j = target_start;
       i < source_end && j < target_length;
       ++i, ++j) {
    *(target_buffer + j) = *(source_buffer + i);
    ++copied;
  }

  JS_PUSH_INT(copied)

  return 1;
}

int lainjs_buffer_binding_setup_buffer_js(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)
  assert(args_lengs == 1);
  assert(duk_is_function(ctx, 0));

  // object is prototype.
  lainjs_binding_get_object_on_index(ctx, 0, "prototype");
  lainjs_binding_func_on_top(ctx, Write, "_write");
  lainjs_binding_func_on_top(ctx, ToString, "_toString");
  lainjs_binding_func_on_top(ctx, Copy, "copy");

  return 0;
}

int lainjs_buffer_binding_alloc(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)
  assert(args_lens == 2);
  assert(duk_is_object(ctx, 0));
  assert(duk_is_number(ctx, 1));

  JS_GET_NUMBER(1, length)
  char* buffer = lainjs_alloc_char_buffer(length);

  lainjs_binding_native_on_index(ctx, 0, buffer, "##native##");
  lainjs_binding_get_object_on_index(ctx, 0, "##native##");

  return 1;
}

void lainjs_init_buffer(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_BUFFER);

  lainjs_binding_object_on_stash(ctx, module->module);

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_buffer_binding_alloc,
                                        "alloc");

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_buffer_binding_setup_buffer_js,
                                        "setupBufferJs");
}
