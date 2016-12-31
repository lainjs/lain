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

var Timer = process.binding(process.binding.timer);
var TIMEOUT_MAX = 2147483647; // 2^31-1

var timers_list = [];

function TimerObject(after) {
  this.after = after;
  this.isrepeat = false;
  this.callback = null;
  this.handler = null;
}

var handleTimeout = function () {
  var timerobj = this.timerObj;
  if (timerobj && timerobj.callback) {
    timerobj.callback();
    if (!timerobj.isrepeat) {
      timerobj.close();
    }
  }
};

TimerObject.prototype.activate = function() {
  var repeat = 0;
  var handler = new Timer();
  if (this.isrepeat)
    repeat = this.after;
  handler.timerObj = this;
  this.handler = handler;
  handler.start(this.after, repeat, handleTimeout);

  timers_list.push(this);
};

TimerObject.prototype.close = function() {
  this.callback = undefined;
  if (this.handler) {
    this.handler.timerObj = undefined;
    this.handler.stop();
    this.handler = undefined;
  }

  var idx = timers_list.indexOf(this);
  if (idx > -1) {
    var j, leng = timers_list.length;
    for (j = idx; j < leng; j++) {
      timers_list[j] = timers_list[j+1];
    }
    timers_list.pop();
  }
};

exports.setTimeout = function(callback, delay) {
  if (!util.isFunction(callback))
    throw new TypeError('callback must be a function');

  delay *= 1;
  if (delay < 1 || delay > TIMEOUT_MAX) {
    delay = 1;
  }

  var timerobj = new TimerObject(delay);
  timerobj.callback = callback;
  timerobj.activate();

  return timerobj;
};

exports.clearTimeout = function(timerobj) {
  if (timerobj && timerobj.callback && (timerobj instanceof TimerObject))
    timerobj.close();
  else
    throw new Error('Error clearTimeout, not a valid timeout Object');
};

exports.setInterval = function(callback, after) {
  if (!util.isFunction(callback))
    throw new TypeError('callback must be a function');

  after *= 1;
  if (after < 1 || after > TIMEOUT_MAX) {
    after = 1;
  }

  var timerobj = new TimerObject(after);
  timerobj.callback = callback;
  timerobj.isrepeat = true;
  timerobj.activate();

  return timerobj;
};

exports.clearInterval = function(timerobj) {
  if (timerobj && timerobj.isrepeat)
    timerobj.close();
  else
    throw new Error('Error clearInterval, not a valid interval Object');
};
