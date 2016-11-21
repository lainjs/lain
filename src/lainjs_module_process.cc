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

void lainjs_on_next_tick(struct v7 *v) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);
  assert(v7_is_object(module->module));

  v7_val_t callee_object = v7_get(v, module->module, "_onNextTick", ~0);
  if(v7_is_callable(v, callee_object)) {
    v7_val_t result;
    if (v7_apply(v, callee_object, V7_UNDEFINED, V7_UNDEFINED, &result) != V7_OK) {
      // TODO: We need log system to print information.
      printf("ERR : lainjs_on_timeout\n");
    } 
  }
}

void lainjs_request_callback(struct v7 *v, v7_val_t callback, v7_val_t _this, v7_val_t args, v7_val_t *result) {
  if (v7_apply(v, callback, _this, V7_UNDEFINED, result) != V7_OK) {
    // TODO: We need log system to print information.
    printf("ERR : lainjs_on_timeout\n");
  }

  lainjs_on_next_tick(v);
}

void lainjs_init_process_module(struct v7 *v) {
  module* module = lainjs_get_builtin_module(MODULE_PROCESS);
  assert(v7_is_object(module->module));
  v7_def(v, v7_get_global(v), "process", ~0, V7_DESC_ENUMERABLE(0), module->module);
}
