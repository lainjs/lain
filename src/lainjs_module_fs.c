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
#include "lainjs_env.h"
#include "lainjs_exception.h"
#include "lainjs_module.h"
#include "lainjs_module_fs.h"
#include "lainjs_util.h"

// FIXME : We need to clean up it.
typedef struct {
  // Make wrapper...
  duk_context *ctx;
  void *callback;
  //
  uv_fs_t req;
} lainjs_fsreqwrap_t;

static void After(uv_fs_t* req) {
  lainjs_fsreqwrap_t *req_wrap = (lainjs_fsreqwrap_t*) req->data;
  assert(req_wrap != NULL);
  assert(req_wrap->req == req);
  duk_context *ctx = req_wrap->ctx;

  duk_push_global_stash(ctx);
  duk_get_prop_string(ctx, -1, req_wrap->callback);

  if (req->result < 0) {
    //FIXEM
    duk_push_string(ctx, CreateUVException(req->result, "uv_fs_function"));
  } else {
      duk_push_null(ctx);
    switch (req->fs_type) {
      case UV_FS_OPEN:
      case UV_FS_READ:
      {
        duk_push_number(ctx, req->result);
        break;
      }
      default:
        duk_push_null(ctx);
    }
  }

  // FIXME : we need changing to 'duk_pcall_method'.
  if (req->result < 0) {
    duk_call(ctx, 1);
  } else {
    duk_call(ctx, 2);
  }
  duk_pop_2(ctx);

  uv_fs_req_cleanup(req);

  // We must remove id and object registered on stash...
  free(req_wrap); 
}

#define FS_ASYNC(env, syscall, pcallback, ...) \
  lainjs_fsreqwrap_t *req_wrap = \
    (lainjs_fsreqwrap_t*)malloc(sizeof(lainjs_fsreqwrap_t)); \
  req_wrap->callback = pcallback; \
  uv_fs_t* fs_req = &(req_wrap->req); \
  req_wrap->ctx = ctx; \
  fs_req->data = req_wrap; \
  int err = uv_fs_ ## syscall(env->loop, \
                              fs_req, \
                              __VA_ARGS__, \
                              After); \
  if (err < 0) { \
    fs_req->result = err; \
    After(fs_req); \
  } \
  duk_push_null(ctx); \

#define FS_SYNC(env, syscall, ...) \
  uv_fs_t fs_req; \
  int err = uv_fs_ ## syscall(env->loop, \
                              &fs_req, \
                              __VA_ARGS__, \
                              NULL); \
  if (err < 0) { \
    JSTHROW(CreateUVException(err, #syscall)) \
  } else { \
    duk_push_number(ctx, err);\
  } \

int lainjs_fs_binding_open(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);

  if (args_lens < 1) {
    JSTHROW("path required");
  } else if (args_lens < 2) {
    JSTHROW("flags required");
  } else if (args_lens < 3) {
    JSTHROW("mode required");
  }

  if (!duk_is_string(ctx, 0)) {
    JSTHROW("path must be a string");
  } else if (!duk_is_number(ctx, 1)) {
    JSTHROW("flags must be an int");
  } else if (!duk_is_number(ctx, 2)) {
    JSTHROW("mode must be an int");
  }

  struct env *env = lainjs_get_envronment(ctx);

  const char* path = duk_get_string(ctx, 0);
  int flags = duk_get_int(ctx, 1);
  int mode = duk_get_int(ctx, 2);;

  if (args_lens > 3 && duk_is_function(ctx, 3)) {
    char* object_id = lainjs_random_generate_id(20);
    duk_push_global_stash(ctx);
    duk_dup(ctx, 3);
    duk_put_prop_string(ctx, -2, object_id);
    duk_pop(ctx);
    FS_ASYNC(env, open, object_id, path, flags, mode);
  } else {
    FS_SYNC(env, open, path, flags, mode);
  }

  return 1;
}

int lainjs_fs_binding_read(duk_context *ctx) {
  unsigned long args_lens = duk_get_top(ctx);

  if (args_lens < 1) {
    JSTHROW("fd required");
  } else if (args_lens < 2) {
    JSTHROW("buffer required");
  } else if (args_lens < 3) {
    JSTHROW("offset required");
  } else if (args_lens < 4) {
    JSTHROW("length required");
  } else if (args_lens < 5) {
    JSTHROW("position required");
  }

  if (!duk_is_number(ctx, 0)) {
    JSTHROW("fd must be an int");
  } else if (!duk_is_object(ctx, 1)) {
    JSTHROW("buffer must be a Buffer");
  } else if (!duk_is_number(ctx, 2)) {
    JSTHROW("offset must be an int");
  } else if (!duk_is_number(ctx, 3)) {
    JSTHROW("length must be an int");
  } else if (!duk_is_number(ctx, 4)) {
    JSTHROW("position must be an int");
  }

  struct env *env = lainjs_get_envronment(ctx);

  int fd = duk_get_number(ctx, 0);
  int offset = duk_get_number(ctx, 2);
  int length = duk_get_number(ctx, 3);
  int position = duk_get_number(ctx, 4);

  // "Check"
  duk_get_prop_string(ctx, 1, "##native##");
  char* buffer = (char*)duk_get_pointer(ctx, -1);

  duk_get_prop_string(ctx, 1, "length");
  int buffer_length = duk_get_number(ctx, -1);
  duk_pop_2(ctx);

  if (offset >= buffer_length) {
    JSTHROW("offset out of bound");
  }
 
  if (offset + length > buffer_length) {
    JSTHROW("length out of bound");
  }

  uv_buf_t uvbuf = uv_buf_init(buffer + offset, length);

  if (args_lens > 5 && duk_is_function(ctx, 5)) {
    char* object_id = lainjs_random_generate_id(20);
    duk_push_global_stash(ctx);
    duk_dup(ctx, 5);
    duk_put_prop_string(ctx, -2, object_id);
    duk_pop(ctx);
    FS_ASYNC(env, read, object_id, fd, &uvbuf, 1, position);
  } else {
    FS_SYNC(env, read, fd, &uvbuf, 1, position);
  }

  return 0;
}

void lainjs_init_fs(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_FS);

  STORE_OBJECT_ON_STASH(ctx, module->module)

  FUNC_BINDING_WITH_STASH(ctx, module->module,
                          lainjs_fs_binding_open,
                          "open")

  FUNC_BINDING_WITH_STASH(ctx, module->module,
                          lainjs_fs_binding_read,
                          "read")
}
