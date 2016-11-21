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

#include "lainjs_env.h"

struct env *lainjs_get_envronment(v7 *v) {
  v7_val_t native_object = v7_get(v, v7_get_global(v), "_native_env", ~0);
  if (!(v7_is_foreign(native_object))) return NULL;

  return reinterpret_cast<struct env *>(v7_get_ptr(v, native_object));;
}

void lainjs_set_envronment(v7 *v, struct env *env) {
  if (lainjs_get_envronment(v)) return;

  v7_def(v, v7_get_global(v), "_native_env", ~0, V7_DESC_ENUMERABLE(0),
         v7_mk_foreign(v, (void *)env));
}
