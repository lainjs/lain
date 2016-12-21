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

///// BINDING API START
#define JS_BINDING_OBJECT_ON_STASH(obj) \
  duk_push_global_stash(ctx); \
  duk_push_object(ctx); \
  duk_put_prop_string(ctx, -2, obj); \
  duk_pop(ctx);

#define JS_BINDING_FUNC_ON_STASH(func, name) \
  duk_push_global_stash(ctx); \
  duk_push_c_function(ctx, func, DUK_VARARGS); \
  duk_put_prop_string(ctx, -2, name); \
  duk_pop(ctx);

#define JS_BINDING_FUNC_WITH_STASH_AND_OBJECT(obj, func, name) \
  duk_push_global_stash(ctx); \
  duk_get_prop_string(ctx, -1, obj); \
  duk_push_c_function(ctx, func, DUK_VARARGS); \
  duk_put_prop_string(ctx, -2, name); \
  duk_pop_2(ctx);
///// BINDING API END

///// GET API START
#define JS_GET_OBJECT_ON_STASH(obj) \
  duk_push_global_stash(ctx); \
  duk_get_prop_string(ctx, -1, obj); \
  duk_remove(ctx, -2); \

#define JS_GET_GLOBAL_OBJECT \
  duk_push_global_object(ctx);
///// GET API END

///// DELETE API START
#define JS_DELETE_OBJECT_ON_STASH(obj) \
  duk_push_global_stash(ctx); \
  if (duk_has_prop_string(ctx, -1, obj)) \
    duk_del_prop_string(ctx, -1, obj); \
  duk_pop(ctx);
///// DELETE API END

///// EVAL API START
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
///// EVAL API END

///// THROW API START
#define JS_THROW(text) \
  duk_push_string(ctx, text); \
  duk_throw(ctx);
///// THROW API END

typedef struct {
  int args[20]; // maxium : 20
  int size;
} lainjs_args_t;

typedef struct {
  int function;
  lainjs_args_t args;
} lainjs_func_t;

lainjs_func_t* lainjs_create_func_t();
void lainjs_set_function(lainjs_func_t *this_, int idx);
void lainjs_add_argument(lainjs_func_t *this_, int idx);

void lainjs_call_mathod_with_this(duk_context *ctx, lainjs_func_t *this_);
void lainjs_eval_exception(duk_context *ctx, duk_int_t rc);

#endif
