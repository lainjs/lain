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

var util = require('util');


function EventEmitter() {
  this._events = {};
};

module.exports = EventEmitter;

EventEmitter.prototype.addListener = function(type, listener) {
  if (!util.isFunction(listener)) {
    throw new TypeError('linster must be a function');
  }

  if (!this._events)
    this._events = {};

  if (!this._events[type])
    this._events[type] = [];

  if (util.isObject(this._events[type])) {
    this._events[type].push(listener);
  }

  return this;
};

//TODO: https://github.com/nodejs/node/blob/master/lib/events.js#L170
EventEmitter.prototype.emit = function(type, arg1, arg2) {
  if (!this._events) {
    this._events = {};
  }

  var handler = this._events[type];
  if (util.isUndefined(handler)) {
    return false;
  } else if (util.isObject(handler)) {
    listeners = handler;
    for (i = 0; i < listeners.length; ++i) {
      listeners[i].call(this, arg1, arg2);
    }
  }

  return true;
};

EventEmitter.prototype.on = EventEmitter.prototype.addListener;
