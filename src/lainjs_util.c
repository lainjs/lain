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

#include "lainjs_util.h"

char* lainjs_read_file(const char* path) {
  FILE* file = fopen(path, "rb");
  assert(file != NULL);

  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buff = lainjs_alloc_char_buffer(len + 1);

  size_t read = fread(buff, 1, len, file);
  assert(read == len);

  fclose(file);

  return buff;
}


char* lainjs_alloc_char_buffer(size_t size) {
  return (char*)malloc(size);
}

void lainjs_release_char_buffer(char* buffer) {
  free(buffer);
}

char* lainjs_random_generate_id(int length) {
  char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  size_t string_len = 26 * 2;
  char *random_string;

  random_string = (char*)malloc(sizeof(char) * (length +1));

  if (!random_string)
    return (char*)0;

  unsigned int key = 0;

  for (int n = 0;n < length;n++) {
    key = rand() % string_len;
    random_string[n] = string[key];
  }

  random_string[length] = '\0';

  return random_string;
}
