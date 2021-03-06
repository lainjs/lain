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

#include "lainjs_js.h"
#include "lainjs_module.h"
#include "lainjs_module_process.h"
#include "lainjs_util.h"

int lainjs_process_binding_wrapping_as_function(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)

  assert(args_lengs == 1);
  JS_GET_STRING(0, source)

  const char *wrapper[2] = {"(function (exports, require, module) {\n", "\n});\n"};

  int len = strlen(source)+ strlen(wrapper[0]) + strlen(wrapper[1]);

  char *code = lainjs_alloc_char_buffer(len + 1);

  strcpy(code,wrapper[0]);
  strcat(code,source);
  strcat(code,wrapper[1]);

  JS_EVAL_WITHOUT_POP(code)

  free(code);

  return 1;
}

int lainjs_process_binding_binding(duk_context *ctx) {
  assert(duk_get_top(ctx) == 1);
  int kind = (int)duk_to_number(ctx, 0);

  if (kind >= MODULE_COUNT || kind < 0) {
    duk_push_undefined(ctx);
    return 1;
  }

  module* module = lainjs_get_builtin_module(kind);

  duk_push_global_stash(ctx);
  if (!duk_has_prop_string(ctx, -1, module->module))
    if (module->register_func)
      module->register_func(ctx);

  lainjs_binding_get_object_on_stash(ctx, module->module);

  return 1;
}

int lainjs_process_binding_compile(duk_context *ctx) {
  assert(duk_get_top(ctx) == 1);
  assert(duk_is_string(ctx, 0));

  JS_EVAL_WITHOUT_POP(duk_get_string(ctx, 0))

  return 1;
}

int lainjs_process_binding_read_source(duk_context *ctx) {
  assert(duk_get_top(ctx) == 1);
  assert(duk_is_string(ctx, 0));

  // TODO: We need to fix up 'lainjs_read_file' function.
  char* code = lainjs_read_file(duk_get_string(ctx, 0));

  if (!code)
    return 0;

  duk_push_string(ctx, code);
  duk_trim(ctx, -1);
  return 1;
}

int lainjs_on_next_tick(duk_context *ctx) {
  int ret = 0;
  {
    module* module = lainjs_get_builtin_module(MODULE_PROCESS);
    lainjs_binding_get_object_on_stash(ctx, module->module);
    lainjs_binding_get_object_on_index_and_remove_index(ctx, -1, "onNextTick");

    lainjs_func_t *func = lainjs_create_func_t();
    lainjs_set_function(func, -1);
    ret = lainjs_call_mathod(ctx, func, LAIN_FALSE);
    lainjs_free_func_t(func);
  }

  {
    // If ref count is '0', delete binding objects.
    int kind;
    for (kind = MODULE_TIMER; kind < MODULE_COUNT; kind++) {
      module* module = lainjs_get_builtin_module(kind);
      JS_DELETE_OBJECT_ON_STASH(module->module)
    }
  }

  return ret;
}

void lainjs_set_native_codes(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);
  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, "process");
  duk_push_object(ctx);
  // FIXME : cleanly make it, and have memory issue.
  int index;
  for (index = 0; natives[index].name; index++) {
    duk_push_string(ctx, natives[index].source);
    duk_put_prop_string(ctx, -2, natives[index].name);
  }
  duk_put_prop_string(ctx, -2, "native_sources");

  duk_pop_2(ctx);
}

void lainjs_init_process(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);

  lainjs_binding_object_on_stash(ctx, module->module);

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_process_binding_binding,
                                        "binding");

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_process_binding_compile,
                                        "compile");

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_process_binding_read_source,
                                        "readSource");

  lainjs_binding_func_on_stashed_object(ctx, module->module,
                                        lainjs_process_binding_wrapping_as_function,
                                        "wrappingAsFunction");

  //FIXME: move binding codes to 'lainjs_binding'.
  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, module->module);
  duk_push_global_object(ctx);
  duk_dup(ctx, -2);
  duk_put_prop_string(ctx, -2, module->module);
  duk_pop_3(ctx);

  lainjs_set_native_codes(ctx);

  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, module->module);
  duk_get_prop_string(ctx, -1, "binding");
  duk_push_int(ctx, MODULE_PROCESS);
  duk_put_prop_string(ctx, -2, "process");
  duk_push_int(ctx, MODULE_CONSOLE);
  duk_put_prop_string(ctx, -2, "console");
  duk_push_int(ctx, MODULE_TIMER);
  duk_put_prop_string(ctx, -2, "timer");
  duk_push_int(ctx, MODULE_BUFFER);
  duk_put_prop_string(ctx, -2, "buffer");
  duk_push_int(ctx, MODULE_FS);
  duk_put_prop_string(ctx, -2, "fs");
  duk_push_int(ctx, MODULE_CONSTANTS);
  duk_put_prop_string(ctx, -2, "constants");
  duk_push_int(ctx, MODULE_TCP);
  duk_put_prop_string(ctx, -2, "tcp");

  duk_pop_3(ctx);
}
