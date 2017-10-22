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
#include "lainjs_module.h"
#include "lainjs_module_tcp.h"

typedef struct {
  duk_context *ctx;
  void *target;
  void *callback;
  uv_tcp_t req;
} lainjs_tcp_req_t;

typedef struct {
  duk_context *ctx;
  void *callback;
  uv_connect_t connect;
} lainjs_tcp_connect_t;

typedef struct {
  duk_context *ctx;
  void *callback;
  uv_write_t write;
} lainjs_tcp_write_t;

lainjs_tcp_req_t* lainjs_create_tcp_req(duk_context *ctx) {
  lainjs_tcp_req_t *req = malloc(sizeof(lainjs_tcp_req_t));
  uv_tcp_init(lainjs_get_envronment(ctx)->loop, &req->req);
  req->ctx = ctx;
  req->req.data = req;

  // Binding this.
  req->target = lainjs_gen_key_on_stach(ctx);
  lainjs_binding_this_on_stash(ctx, req->target);

  return req;
}

void lainjs_destroy_tcp_req(lainjs_tcp_req_t* req) {
  duk_context *ctx = req->ctx;

  if (req->target)
    JS_DELETE_OBJECT_ON_STASH(req->target)

  if (req->callback)
    JS_DELETE_OBJECT_ON_STASH(req->callback)

  free(req);
}

lainjs_tcp_connect_t* lainjs_create_tcp_connect(duk_context *ctx, char *callback) {
  lainjs_tcp_connect_t *connect = malloc(sizeof(lainjs_tcp_connect_t));
  connect->ctx = ctx;
  connect->callback = callback;
  return connect;
}

lainjs_tcp_write_t* lainjs_create_tcp_write(duk_context *ctx, char *callback) {
  lainjs_tcp_write_t *write = malloc(sizeof(lainjs_tcp_write_t));
  write->ctx = ctx;
  write->callback = callback;
  return write;
}

void OnShutdown(uv_shutdown_t* req, int status) {
  lainjs_tcp_req_t* tcp_req =
      (lainjs_tcp_req_t*)(req->handle->data);
  duk_context *ctx = tcp_req->ctx;

  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");
  lainjs_binding_get_object_on_index(ctx, -1, "_onclose");

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_call_mathod(ctx, func, LAIN_TRUE);

  lainjs_free_func_t(func);

  // TODO: delete 'tcp_req'.
}

static void OnConnection(uv_stream_t* handle, int status) {
  lainjs_tcp_req_t* tcp_req = (lainjs_tcp_req_t*)(handle->data);
  duk_context *ctx = tcp_req->ctx;

  if(!status) {
    lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
    // Get handle for server.
    // FIXME
    lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                        -1, "_socket");
    lainjs_binding_get_object_on_index(ctx, -1, "_createTCP");
    duk_dup(ctx, -2);
    duk_remove(ctx, -3);
    duk_new(ctx, 1);

    JS_GET_NATIVE_OBJECT_ON_INDEX(-1, lainjs_tcp_req_t* socket_tcp_req)
    // We are in charge of server sockets in vector or else.
    uv_stream_t* client_handle =
        (uv_stream_t*)(&socket_tcp_req->req);

    int err = uv_accept(handle, client_handle);
    if (err) {
      // FIXME : processing then failure.
      return;
    }

    // arguments.
    duk_dup(ctx, -1);
    duk_remove(ctx, -2);
    JS_PUSH_INT(status)
  } else {
    JS_PUSH_INT(status)
  }

  // this
  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");

  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_onconnection");

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_add_argument(func, -3);
  if(!status)
    lainjs_add_argument(func, -4);

  lainjs_call_mathod(ctx, func, LAIN_TRUE);
  lainjs_free_func_t(func);

  // Start 'Read' data ...
}

// server
int Listen(duk_context *ctx) {
  assert(JS_IS_NUMBER(0));

  // number of clients
  JS_GET_INT(0, backlog)

  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)
  int err = uv_listen((uv_stream_t*)&tcp_req->req,
                      backlog,
                      OnConnection);
  JS_PUSH_INT(err)
  return 1;
}

void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = lainjs_alloc_char_buffer(suggested_size);
  buf->len = suggested_size;
}

void OnRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
  lainjs_tcp_req_t* tcp_req = (lainjs_tcp_req_t*)(handle->data);
  duk_context *ctx = tcp_req->ctx;

  // Create Buffer and copy data.
  lainjs_internal_create_buffer(ctx, nread, buf->base);

  JS_PUSH_INT(nread)

  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");

  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");

  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_onread");

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_add_argument(func, -3);
  lainjs_add_argument(func, -4);

  lainjs_call_mathod(ctx, func, LAIN_TRUE);
  lainjs_free_func_t(func);
}

int ReadStart(duk_context *ctx) {
  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)

  int err = uv_read_start(
      (uv_stream_t*)&tcp_req->req,
      OnAlloc,
      OnRead);

  JS_PUSH_INT(err)

  return 1;
}

static void AfterConnect(uv_connect_t* req, int status) {
  lainjs_tcp_connect_t* connect = (lainjs_tcp_connect_t*)(req->data);
  lainjs_tcp_req_t* tcp_req = (lainjs_tcp_req_t*)(req->handle->data);
  assert(tcp_req != NULL);
  assert(connect != NULL);

  duk_context *ctx = connect->ctx;

  JS_PUSH_INT(status)
  lainjs_binding_get_object_on_stash(ctx, connect->callback);

  // Need add this.
  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_call_mathod(ctx, func, LAIN_FALSE);
  lainjs_free_func_t(func);
}

// client need after callback
int Connect(duk_context *ctx) {
  assert(JS_IS_STRING(0));
  assert(JS_IS_NUMBER(1));
  assert(JS_IS_FUNCTION(2));

  JS_GET_STRING(0, address)
  JS_GET_INT(1, port)

  struct sockaddr_in addr;
  int result = uv_ip4_addr(address, port, &addr);

  if (!result) {
    char* object_id = lainjs_gen_key_on_stach(ctx);
    lainjs_binding_index_on_stash(ctx, 2, object_id);
    JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)
    lainjs_tcp_connect_t *connect = lainjs_create_tcp_connect(ctx, object_id);
    connect->connect.data = connect;

    result = uv_tcp_connect(&connect->connect,
                            &tcp_req->req,
                            (const struct sockaddr*)(&addr),
                            AfterConnect);
  }

  JS_PUSH_INT(result);
  return 1;
}

static void AfterClose(uv_handle_t* handle) {
  lainjs_tcp_req_t* tcp_req =
      (lainjs_tcp_req_t*)(handle->data);
  duk_context *ctx = tcp_req->ctx;
  lainjs_binding_get_object_on_stash(ctx, tcp_req->target);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "_socket");
  lainjs_binding_get_object_on_index(ctx, -1, "_onclose");

  lainjs_func_t *func = lainjs_create_func_t();
  lainjs_set_function(func, -1);
  lainjs_add_argument(func, -2);
  lainjs_call_mathod(ctx, func, LAIN_TRUE);

  lainjs_free_func_t(func);

  // TODO: delete 'tcp_req'.
}

// server, client socket
int Close(duk_context *ctx) {
  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)

  uv_close((uv_handle_t*)&tcp_req->req, AfterClose);
  return 0;
}

// server
int Bind(duk_context *ctx) {
  assert(JS_IS_STRING(0));
  assert(JS_IS_NUMBER(1));

  JS_GET_STRING(0, address)
  JS_GET_INT(1, port)

  struct sockaddr_in addr;
  int result = uv_ip4_addr(address, port, &addr);

  if (!result) {
    JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)
    result = uv_tcp_bind(&tcp_req->req,
                         (const struct sockaddr*)&addr,
                         0);
  } 
  JS_PUSH_INT(result)

  return 1;
}

void AfterWrite(uv_write_t* req, int status) {
  // TODO: Write this function in order to process
  // after calling write.
}

int WriteTCP(duk_context *ctx) {
  JS_GET_FUNCTION_ARGS_LENGS(args_lens)
  assert(args_lens == 2);
  assert(JS_IS_OBJECT(0));
  assert(JS_IS_FUNCTION(1));
  JS_GET_NATIVE_OBJECT_ON_THIS(lainjs_tcp_req_t* tcp_req)
  JS_GET_STRING_FROM_OBJECT(0, msg)

  char* object_id = lainjs_gen_key_on_stach(ctx);
  lainjs_binding_index_on_stash(ctx, 1, object_id);
  lainjs_tcp_write_t *write = lainjs_create_tcp_write(ctx, object_id);
  write->write.data = write;

  uv_buf_t buf;
  buf.base = (char*)msg;
  buf.len = strlen(msg);

  int result = uv_write(&write->write,
                        (uv_stream_t*)&tcp_req->req,
                        &buf,
                        1,
                        AfterWrite);

  JS_PUSH_INT(result)

  return 1;
}

int lainjs_construct_tcp(duk_context *ctx) {
  if (!duk_is_constructor_call(ctx))
    return 0;

  lainjs_tcp_req_t* tcp_req = lainjs_create_tcp_req(ctx);
  JS_GET_THIS
  lainjs_binding_native_on_top(ctx, tcp_req);

  return 1;
}

void lainjs_init_tcp(duk_context *ctx) {
  module* module = lainjs_get_builtin_module(MODULE_TCP);
  lainjs_binding_func_on_stash(ctx, lainjs_construct_tcp, module->module);

  // get prototype on 'function'
  // and binding 'bind', 'close', 'connect' and 'listen'.
  lainjs_binding_object_on_module_object(ctx, module->module, "prototype");

  lainjs_binding_get_object_on_stash(ctx, module->module);
  lainjs_binding_get_object_on_index_and_remove_index(ctx,
                                                      -1, "prototype");

  lainjs_binding_func_on_top(ctx, Bind, "bind");
  lainjs_binding_func_on_top(ctx, Close, "close");
  lainjs_binding_func_on_top(ctx, Connect, "connect");
  lainjs_binding_func_on_top(ctx, Listen, "listen");
  lainjs_binding_func_on_top(ctx, ReadStart, "readStart");
  lainjs_binding_func_on_top(ctx, WriteTCP, "doWrite");
}
