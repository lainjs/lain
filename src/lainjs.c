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
#include <string.h>
#include <assert.h>

#include "lainjs_binding.h"
#include "lainjs_env.h"
#include "lainjs_js.h"
#include "lainjs_module.h"
#include "lainjs_module_console.h"
#include "lainjs_module_process.h"
#include "lainjs_module_timer.h"
#include "lainjs_util.h"
#include "uv.h"

void lainjs_release_js_context(duk_context *ctx) {
  duk_destroy_heap(ctx);
}

void lainjs_setup_modules(duk_context *ctx) {
  lainjs_init_modules(ctx);
  lainjs_init_process(ctx);
}

void lainjs_start_main_js(duk_context *ctx) {
  duk_eval_string(ctx, mainjs);
}

int lainjs_start(char** argv) {
  duk_context *ctx = duk_create_heap_default();

  struct env *env = (struct env*) malloc(sizeof(struct env));
  env->loop = uv_default_loop();
  lainjs_set_envronment(ctx, env);

  lainjs_setup_modules(ctx);

  // FIXME : cleanly make it.
  {
    duk_push_global_stash(ctx);
    duk_get_prop_string(ctx, -1, "process");

    duk_idx_t arr_idx;
    arr_idx = duk_push_array(ctx);
    duk_push_string(ctx, argv[0]);
    duk_put_prop_index(ctx, arr_idx, 0);
    duk_push_string(ctx, argv[1]);
    duk_put_prop_index(ctx, arr_idx, 1);
    duk_put_prop_string(ctx, -2, "argv");
    duk_pop_2(ctx);
  }

  lainjs_start_main_js(ctx);

  //duk_eval_string(ctx, "print(process['lainjs']);");

  int more;
  do {
    more = uv_run(env->loop, UV_RUN_ONCE);
    if (!more) {
      lainjs_on_next_tick(ctx);
      more = uv_loop_alive(env->loop);
    }
  } while (more);

  lainjs_release_js_context(ctx);
  return 1;
}

int lainjs_entry(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "usage > lainjs test.js\n");
    return 1;
  }

  lainjs_start(argv);

  return 1;
}
