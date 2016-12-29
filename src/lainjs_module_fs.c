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
  duk_context *ctx;
  void *callback;
  uv_fs_t req;
} lainjs_fs_req_t;

lainjs_fs_req_t* lainjs_create_fs_req() {
  return malloc(sizeof(lainjs_fs_req_t));
}

void lainjs_destroy_fs_req(lainjs_fs_req_t* req) {
  duk_context *ctx = req->ctx;

  JS_DELETE_OBJECT_ON_STASH(req->callback)

  free(req);
}

static void After(uv_fs_t* req) {
  lainjs_fs_req_t *req_wrap = (lainjs_fs_req_t*) req->data;
  assert(req_wrap != NULL);
  assert(req_wrap->req == req);
  duk_context *ctx = req_wrap->ctx;

  if (req->result < 0) {
    JS_PUSH_STRING(CreateUVException(req->result, "uv_fs_function"))
  } else {
    switch (req->fs_type) {
      case UV_FS_OPEN:
      case UV_FS_READ:
      {
        JS_PUSH_INT(req->result)
        break;
      }
      default:
        JS_PUSH_NULL
    }
    JS_PUSH_NULL
  }

  JS_GET_PROP_ON_STASH(req_wrap->callback)

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  if (req->result < 0) {
    lainjs_add_argument(func, -2);
  } else {
    lainjs_add_argument(func, -2);
    lainjs_add_argument(func, -3);
  }
  lainjs_call_mathod(ctx, func, LAIN_FALSE);
  lainjs_free_func_t(func);

  uv_fs_req_cleanup(req);

  lainjs_destroy_fs_req(req_wrap);
}

#define FS_ASYNC(env, syscall, pcallback, ...) \
  lainjs_fs_req_t *req_wrap = \
    lainjs_create_fs_req(); \
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
  JS_PUSH_NULL \

#define FS_SYNC(env, syscall, ...) \
  uv_fs_t fs_req; \
  int err = uv_fs_ ## syscall(env->loop, \
                              &fs_req, \
                              __VA_ARGS__, \
                              NULL); \
  if (err < 0) { \
    JS_THROW(CreateUVException(err, #syscall)) \
  } else { \
    JS_PUSH_INT(err) \
  } \

int lainjs_fs_binding_open(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)

  if (args_lens < 1) {
    JS_THROW("path required");
  } else if (args_lens < 2) {
    JS_THROW("flags required");
  } else if (args_lens < 3) {
    JS_THROW("mode required");
  }

  if (!JS_IS_STRING(0)) {
    JS_THROW("path must be a string");
  } else if (!JS_IS_NUMBER(1)) {
    JS_THROW("flags must be an int");
  } else if (!JS_IS_NUMBER(2)) {
    JS_THROW("mode must be an int");
  }

  struct env *env = lainjs_get_envronment(ctx);

  JS_GET_STRING(0, path)
  JS_GET_INT(1, flags)
  JS_GET_INT(2, mode)

  if (args_lens > 3 && JS_IS_FUNCTION(3)) {
    char* object_id = lainjs_gen_key_on_stach(ctx);
    // Binding function on stash.
    JS_BINDING_INDEX_ON_STASH(3, object_id)
    FS_ASYNC(env, open, object_id, path, flags, mode);
  } else {
    FS_SYNC(env, open, path, flags, mode);
  }

  return 1;
}

int lainjs_fs_binding_read(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)

  if (args_lens < 1) {
    JS_THROW("fd required");
  } else if (args_lens < 2) {
    JS_THROW("buffer required");
  } else if (args_lens < 3) {
    JS_THROW("offset required");
  } else if (args_lens < 4) {
    JS_THROW("length required");
  } else if (args_lens < 5) {
    JS_THROW("position required");
  }

  if (!JS_IS_NUMBER(0)) {
    JS_THROW("fd must be an int");
  } else if (!JS_IS_OBJECT(1)) {
    JS_THROW("buffer must be a Buffer");
  } else if (!JS_IS_NUMBER(2)) {
    JS_THROW("offset must be an int");
  } else if (!JS_IS_NUMBER(3)) {
    JS_THROW("length must be an int");
  } else if (!JS_IS_NUMBER(4)) {
    JS_THROW("position must be an int");
  }

  struct env *env = lainjs_get_envronment(ctx);

  JS_GET_INT(0, fd)
  JS_GET_INT(2, offset)
  JS_GET_INT(3, length)
  JS_GET_INT(4, position)

  //TODO : Check it.
  JS_GET_NATIVE_OBJECT_ON_INDEX(1, char* buffer)

  JS_GET_INT_ON_INDEX(1, buffer_length, "length")

  if (offset >= buffer_length) {
    JS_THROW("offset out of bound");
  }
 
  if (offset + length > buffer_length) {
    JS_THROW("length out of bound");
  }

  uv_buf_t uvbuf = uv_buf_init(buffer + offset, length);

  if (args_lens > 5 && JS_IS_FUNCTION(5)) {
    char* object_id = lainjs_gen_key_on_stach(ctx);
    // Binding function on stash.
    JS_BINDING_INDEX_ON_STASH(5, object_id)

    FS_ASYNC(env, read, object_id, fd, &uvbuf, 1, position);
  } else {
    FS_SYNC(env, read, fd, &uvbuf, 1, position);
  }

  return 0;
}

void lainjs_init_fs(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_FS);

  JS_BINDING_OBJECT_ON_STASH(module->module)

  JS_BINDING_FUNC_WITH_STASH_AND_OBJECT(module->module,
                                        lainjs_fs_binding_open,
                                        "open")

  JS_BINDING_FUNC_WITH_STASH_AND_OBJECT(module->module,
                                        lainjs_fs_binding_read,
                                        "read")
}
