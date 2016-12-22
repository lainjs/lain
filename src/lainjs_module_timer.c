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
#include "lainjs_util.h"

#include "uv.h"

typedef struct timer {
  uv_timer_t handle;
  char* obj;
  int64_t timeout;
  int64_t repeat;
  duk_context *ctx;
} lainjs_timer_t;

void lainjs_destroy_timer_t(lainjs_timer_t *timer) {
  if (timer) {
    free(timer->obj);
    free(timer);
  }
}

static void lainjs_on_timeout(lainjs_timer_t *timer) {
  duk_context *ctx = timer->ctx;
  JS_GET_OBJECT_ON_STASH(timer->obj)

  JS_GET_OBJECT_ON_STASH(timer->obj)
  JS_GET_PROP_ON_OBJECT_AND_REMOVE("##callback##")

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_call_mathod(ctx, func, LAIN_TRUE);
  lainjs_free_func_t(func);

  lainjs_on_next_tick(ctx);

  if (!timer->repeat)
    lainjs_destroy_timer_t(timer);
}

static void lainjs_timer_timeout(uv_timer_t* handle) {
  lainjs_timer_t *timer = (lainjs_timer_t*)handle->data;
  assert(&timer->handle == handle);
  if (timer)
    lainjs_on_timeout(timer);
}

int lainjs_start_timer(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)

  assert(args_lens >= 3);
  if (!(args_lens >= 3)) return 0;
  if (!duk_is_function(ctx, 2)) return 0;

  JS_GET_NUMBER(0, timeout)
  JS_GET_NUMBER(1, repeat)

  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_timer_t *timer)
  JS_BINDING_OBJECT_ON_THIS(2, "##callback##");
 
  timer->obj = lainjs_gen_key_on_stach(ctx);
  JS_BINDING_THIS_ON_STASH(timer->obj)

  timer->timeout = timeout;
  timer->repeat = repeat;
  int err = uv_timer_start(&timer->handle, lainjs_timer_timeout,
                       timeout, repeat);
  return 0;
}

int lainjs_stop_timer(duk_context *ctx) {
  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_timer_t *timer)

  if (!timer)
    printf("ERR : invalid timer\n");

  JS_DELETE_OBJECT_ON_STASH(timer->obj)

  int err = uv_timer_stop(&timer->handle);

  if (timer)
    lainjs_destroy_timer_t(timer);

  return 0;
}

int lainjs_construct_timer(duk_context *ctx) {
  if (!duk_is_constructor_call(ctx))
    return 0;

  lainjs_timer_t *timer = (lainjs_timer_t*) malloc(sizeof(lainjs_timer_t));

  if (!timer)
    return 0;

  JS_CREATE_OBJECT
  JS_BINDING_FUNC_ON_OBJECT(lainjs_start_timer, "start")
  JS_BINDING_FUNC_ON_OBJECT(lainjs_stop_timer, "stop")
  JS_BIDNING_NATIVE_ON_OBJECT(timer)

  // uv
  uv_timer_init(lainjs_get_envronment(ctx)->loop, &(timer->handle));
  timer->handle.data = timer;
  timer->ctx = ctx;

  return 1;
}

void lainjs_init_timer(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_TIMER);

  JS_BINDING_FUNC_ON_STASH(lainjs_construct_timer, module->module)
}
