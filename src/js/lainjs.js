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

(function (process) {
  this.global = this;
  global.process = process;

  function start_lainjs() {
    init_process();
  };

  function init_process() {

    initNextTick();

    function initNextTick() {
      var nextTickQueue = [];

      function onNextTick() {
        var callbacks = [];
        for (var i = 0; i < nextTickQueue.length; ++i) {
          callbacks.push(nextTickQueue[i]);
        }
        nextTickQueue = [];

        for (var i = 0; i < callbacks.length; ++i) {
          var callback = callbacks[i];
          callback();
        }
      }

      function nextTick(callback) {
        if (typeof callback === 'function') {
          nextTickQueue.push(callback);
        }
      }

      process.nextTick = nextTick;
      process.onNextTick = onNextTick;
    }
  };

  this.require = function(id) {
    return undefined;
  };

  start_lainjs();
})(process);
