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

var MODULE_BUFFER = 3;
var buffer = global.process.binding(MODULE_BUFFER);
var alloc = buffer.alloc;
var util = require('util');
var console = require('console');

function Buffer(subject, encoding) {
  if (!(this instanceof Buffer)) {
    return new Buffer(subject, encoding);
  }

  if (util.isNumber(subject)) {
    this.length = subject > 0 ? subject >>> 0 : 0;
  } else if (util.isString(subject)) {
    this.length = Buffer.byteLength(subject);
  }

  alloc(this, this.length);

  if (util.isString(subject)) {
    this.write(subject, encoding);
  } else if (subject instanceof Buffer) {
    //subject.copy(this, 0, 0, this.length);
  }
}

Buffer.byteLength = function(str, enc) {
  return str.length;
}

Buffer.prototype.write = function(string, offset, length, encoding) {
  if (util.isUndefined(offset)) {
    encoding = 'utf8';
    length = this.length;
    offset = 0;
  }
  if (util.isUndefined(length) && util.isString(offset)) {
    encoding = offset;
    length = this.length;
    offset = 0;
  }
  offset = offset >>> 0;
  if (util.isNumber(length)) {
    length = length >>> 0;
  } else {
    encoding = length;
    length = undefined;
  }

  var remaining = this.length - offset;
  if (util.isUndefined(length) || length > remaining) {
    length = remaining;
  }

  if (length < 0 || offset < 0) {
    throw 'attempt to write outside buffer bounds';
  }

  return this._write(string, offset, length);
};

Buffer.prototype.toString = function(encoding, start, end) {
  return this._toString();
};

buffer.setupBufferJs(Buffer);

module.exports = Buffer;
