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

#ifndef LAINJS_BINDING_H
#define LAINJS_BINDING_H
#include <stdio.h>
#include "duktape.h"

#include "lainjs_util.h"

typedef int (*JS_NATIVE_CALLBACK)(duk_context*);

#define JS_GET_GLOBAL_OBJECT \
  duk_push_global_object(ctx);

#define JS_GET_THIS \
  duk_push_this(ctx);

#define JS_GET_NATIVE_OBJECT_ON_THIS(var) \
  JS_GET_THIS \
  duk_get_prop_string(ctx, -1, "##native##"); \
  duk_remove(ctx, -2); \
  var = duk_get_pointer(ctx, -1); \
  duk_pop(ctx);

#define JS_GET_NATIVE_OBJECT_ON_INDEX(idx, var) \
  duk_get_prop_string(ctx, idx, "##native##"); \
  var = duk_get_pointer(ctx, -1); \
  duk_pop(ctx);

#define JS_GET_INT(idx, var) \
  int var = duk_get_int(ctx, idx);

#define JS_GET_INT_ON_THIS(var, name) \
  JS_GET_THIS \
  duk_get_prop_string(ctx, -1, name); \
  int var = duk_get_int(ctx, -1); \
  duk_pop_2(ctx);

#define JS_GET_INT_ON_OBJECT_OF_THIS(var, name) \
  JS_GET_THIS \
  lainjs_binding_get_object_on_index_and_remove_index(ctx, -1, name); \
  JS_GET_INT(-1, var) \
  duk_pop(ctx); \

#define JS_GET_INT_ON_INDEX(idx, var, name) \
  duk_get_prop_string(ctx, idx, name); \
  int var = duk_get_int(ctx, -1); \
  duk_pop(ctx);

#define JS_GET_INT_WITHOUT_TYPE_DEFINE(idx, var) \
  var = duk_get_int(ctx, idx);

#define JS_GET_NUMBER(idx, var) \
  double var = duk_get_number(ctx, idx);

#define JS_GET_STRING(idx, var) \
  const char* var = duk_get_string(ctx, idx);

#define JS_GET_FUNCTION_ARGS_LENGS(name) \
  unsigned long name = duk_get_top(ctx);

#define JS_PUSH_INT(val) \
  duk_push_int(ctx, val);

#define JS_PUSH_STRING(val) \
  duk_push_string(ctx, val);

#define JS_PUSH_NULL \
  duk_push_null(ctx);

#define JS_DELETE_OBJECT_ON_STASH(obj) \
  duk_push_global_stash(ctx); \
  if (duk_has_prop_string(ctx, -1, obj)) \
    duk_del_prop_string(ctx, -1, obj); \
  duk_pop(ctx);

#define JS_IS_STRING(idx) \
  duk_is_string(ctx, idx)

#define JS_IS_NUMBER(idx) \
  duk_is_number(ctx, idx)

#define JS_IS_OBJECT(idx) \
  duk_is_object(ctx, idx)

#define JS_IS_FUNCTION(idx) \
  duk_is_function(ctx, idx)

#define JS_EVAL_WITH_RESULT(src) \
  duk_eval_string(ctx, src);

#define JS_EVAL(src) \
  duk_eval_string(ctx, src); \
  duk_pop(ctx);

#define JS_PEVAL(src) \
  duk_int_t rc = duk_peval_string(ctx, src); \
  lainjs_eval_exception(ctx, rc); \
  duk_pop(ctx);

#define JS_EVAL_WITHOUT_POP(src) \
  duk_eval_string(ctx, src); \

#define JS_PEVAL_WITHOUT_POP(src) \
  duk_int_t rc = duk_peval_string(ctx, src); \
  lainjs_eval_exception(ctx, rc);

#define JS_THROW(text) \
  duk_push_string(ctx, text); \
  duk_throw(ctx);

typedef enum {
  LAIN_FALSE,
  LAIN_TRUE
} LAIN_BOOL;

typedef struct {
  // maxium : 20
  int args[20];
  int size;
} lainjs_args_t;

typedef struct {
  int function;
  lainjs_args_t args;
} lainjs_func_t;

// 'create' type function
void lainjs_binding_create_object(duk_context* ctx);

// 'binding' type function
void lainjs_binding_object_on_stash(duk_context* ctx, char* name);

void lainjs_binding_object_on_module_object(duk_context* ctx, char* module, char* name);

void lainjs_binding_this_on_stash(duk_context* ctx, char* name);

void lainjs_binding_index_on_stash(duk_context* ctx, int idx, char* name);

void lainjs_binding_func_on_stash(duk_context* ctx, JS_NATIVE_CALLBACK func, char* name);

void lainjs_binding_func_on_stashed_object(duk_context* ctx, char* obj,
                                           JS_NATIVE_CALLBACK func, char* name);

void lainjs_binding_index_on_this(duk_context* ctx, int idx, char* name);

void lainjs_binding_native_on_index(duk_context* ctx, int idx, void* native, char* name);

void lainjs_binding_func_on_top(duk_context* ctx, JS_NATIVE_CALLBACK func, char* name);

void lainjs_binding_native_on_top(duk_context* ctx, void* native);

void lainjs_binding_get_object_on_stash(duk_context* ctx, char* name);

void lainjs_binding_get_object_on_index(duk_context* ctx, int index, char* name);

void lainjs_binding_get_object_on_index_and_remove_index(duk_context* ctx,
                                                         int index,
                                                         char* name);

lainjs_func_t* lainjs_create_func_t();
void lainjs_free_func_t(lainjs_func_t *this_);
void lainjs_set_function(lainjs_func_t *this_, int idx);
void lainjs_add_argument(lainjs_func_t *this_, int idx);

int lainjs_call_mathod(duk_context *ctx, lainjs_func_t *this_, LAIN_BOOL has_this);
void lainjs_eval_exception(duk_context *ctx, duk_int_t rc);

char* lainjs_gen_key_on_stach(duk_context *ctx);

#endif
