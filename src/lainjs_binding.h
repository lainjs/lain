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

#define JS_DELETE_OBJECT_ON_STASH(obj) \
  duk_push_global_stash(ctx); \
  if (duk_has_prop_string(ctx, -1, obj)) \
    duk_del_prop_string(ctx, -1, obj); \
  duk_pop(ctx);

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

#define JS_THROW(text) \
  duk_push_string(ctx, text); \
  duk_throw(ctx);

#endif
