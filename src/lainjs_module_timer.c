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

struct timer {
  uv_timer_t handle;
  void* obj; // FIXME
  int64_t timeout;
  int64_t repeat;
  duk_context *ctx;
};

static void lainjs_on_timeout(struct timer *timer) {
  duk_context *ctx = timer->ctx;
  duk_push_heapptr(ctx, timer->obj);
  duk_get_prop_string(ctx, -1, "##callback##");

  if (duk_is_callable(ctx, -1)) {
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0);

    //FIXME : move to process module.
    lainjs_on_next_tick(ctx);
    if (!timer->repeat)
      free(timer);
  } else {
    free(timer);
  }

  duk_pop_2(ctx);
}

static void lainjs_timer_timeout(uv_timer_t* handle) {
  struct timer *timer = (struct timer*)handle->data;
  assert(&timer->handle == handle);
  if (timer)
    lainjs_on_timeout(timer);
}

int lainjs_start_timer(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);
  assert(args_lens >= 3);
  if (!(args_lens >= 3)) return 0;
  if (!duk_is_function(ctx, 2)) return 0;
  double timeout = duk_to_number(ctx, 0);
  double repeat = duk_to_number(ctx, 1);

  duk_push_this(ctx);
  duk_get_prop_string(ctx, -1, "##native##");
  struct timer *timer = (struct timer *)duk_get_pointer(ctx, -1);
  duk_pop(ctx);

  duk_dup(ctx, 2);
  duk_put_prop_string(ctx, -2, "##callback##");

  timer->obj = duk_get_heapptr(ctx, -1);
  timer->timeout = timeout;
  timer->repeat = repeat;
  int err = uv_timer_start(&timer->handle, lainjs_timer_timeout,
                       timeout, repeat);
  duk_pop(ctx);
  return 0;
}

int lainjs_stop_timer(duk_context *ctx) {
  duk_push_this(ctx);
  duk_get_prop_string(ctx, -1, "##native##");
  struct timer *timer = (struct timer *)duk_get_pointer(ctx, -1);
  duk_pop_2(ctx);

  if (!timer)
    printf("ERR : invalid timer\n");

  int err = uv_timer_stop(&timer->handle);

  free(timer);
  return 0;
}

int lainjs_construct_timer(duk_context *ctx) {
  if (!duk_is_constructor_call(ctx))
    return 0;

  duk_push_object(ctx);
  duk_push_c_function(ctx, lainjs_start_timer, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "start");
  duk_push_c_function(ctx, lainjs_stop_timer, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, "stop");

  struct timer *timer = (struct timer*) malloc(sizeof(struct timer));
  if (!timer) {
    duk_pop(ctx);
    return 0;
  }

  uv_timer_init(lainjs_get_envronment(ctx)->loop, &(timer->handle));
  timer->handle.data = timer;
  timer->ctx = ctx;
  duk_push_pointer(ctx, (void *)timer);
  duk_put_prop_string(ctx, -2, "##native##");

  return 1;
}

void lainjs_init_timer_module(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_TIMER);

  duk_push_global_stash(ctx);
  duk_push_c_function(ctx, lainjs_construct_timer, DUK_VARARGS);
  duk_put_prop_string(ctx, -2, module->module);
  duk_pop(ctx);
}
