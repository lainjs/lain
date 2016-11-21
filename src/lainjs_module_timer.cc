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
#include <stdlib.h>
#include <assert.h>

#include "lainjs_binding.h"
#include "lainjs_env.h"
#include "lainjs_module.h"
#include "lainjs_module_process.h"
#include "lainjs_module_timer.h"

#include "uv.h"

extern struct env *_env;

struct timer {
  uv_timer_t handle;
  v7_val_t obj;
  v7_val_t callback;
  int64_t timeout;
  int64_t repeat;
  v7* v;
};

//void lainjs_request_callback(struct v7 *v, v7_val_t callback, v7_val_t _this, v7_val_t args)
static void lainjs_on_timeout(struct timer *timer) {
  if (v7_is_callable(timer->v, timer->callback)) {

    v7_val_t result;
    lainjs_request_callback(timer->v, timer->callback, timer->obj, V7_UNDEFINED, &result);

    if (!timer->repeat)
      free(timer);
  } else {
    free(timer);
  }
}

static void lainjs_timer_timeout(uv_timer_t* handle) {
  struct timer *timer = static_cast<struct timer*>(handle->data);
  assert(&timer->handle == handle);
  if (timer)
    lainjs_on_timeout(timer);
}

static enum v7_err lainjs_construct_timer(struct v7 *v, v7_val_t *res) {
  v7_val_t _this = v7_get_this(v);
  struct timer *timer = (struct timer*) malloc(sizeof(struct timer));
  if (!timer) {
    // TODO: We need log system to print information.
    printf("ERR : out of memory\n");
    return V7_EXEC_EXCEPTION;
  }

  uv_timer_init(lainjs_get_envronment(v)->loop, &(timer->handle));
  timer->handle.data = timer;
  timer->v = v;

  // Bind native instance.
  v7_def(v, _this, "_native", ~0, V7_DESC_ENUMERABLE(0),
         v7_mk_foreign(v, (void *)timer));

  return V7_OK;
}

static enum v7_err lainjs_start_timer(struct v7 *v, v7_val_t *res) {
  // Check argument
  unsigned long args_lens = v7_argc(v);
  assert(args_lens >= 3);
  assert(v7_is_callable(v, v7_arg(v, 2)));

  if (!(args_lens >= 3)) return V7_OK;
  if (!(v7_is_callable(v, v7_arg(v, 2)))) return V7_OK;

  v7_val_t _this = v7_get_this(v);
  int64_t timeout = v7_get_int(v, v7_arg(v, 0));
  int64_t repeat = v7_get_int(v, v7_arg(v, 1));

  v7_val_t native_object = v7_get(v, _this, "_native", ~0); 
  assert(v7_is_foreign(native_object));

  if (!(v7_is_foreign(native_object))) return V7_OK;

  struct timer *timer = reinterpret_cast<struct timer *>(v7_get_ptr(v, native_object));

  timer->timeout = timeout;
  timer->repeat = repeat;
  timer->obj = _this;
  timer->callback = v7_arg(v, 2);

  int err;
  err = uv_timer_start(&timer->handle, lainjs_timer_timeout,
                       timeout, repeat);

  return V7_OK;
}

static enum v7_err lainjs_stop_timer(struct v7 *v, v7_val_t *res) {
  v7_val_t _this = v7_get_this(v);

  v7_val_t native_object = v7_get(v, _this, "_native", ~0);
  assert(v7_is_foreign(native_object));

  if (!(v7_is_foreign(native_object))) return V7_OK;

  struct timer *timer = reinterpret_cast<struct timer *>(v7_get_ptr(v, native_object));
  if (!timer) {
    // TODO: We need log system to print information.
    printf("ERR : invalid timer\n");
  }

  int err = uv_timer_stop(&timer->handle);

  free(timer);

  return V7_OK;
}

void lainjs_init_timer_module(struct v7 *v) {
  module* module = lainjs_get_builtin_module(MODULE_TIMER);
  assert(v7_is_object(module->module));

  v7_val_t ctor_func = v7_mk_function_with_proto(v, lainjs_construct_timer, module->module);
  
  v7_set(v, v7_get_global(v), "Timer", ~0, ctor_func);
  v7_set_method(v, module->module, "start", &lainjs_start_timer);
  v7_set_method(v, module->module, "stop", &lainjs_stop_timer);
}
