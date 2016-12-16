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
    init_global();
    init_process();
    var module = Native.require('module');
    module.runMain();
  };

  function init_global() {
    global.process = process;
    global.global = global;
    global.GLOBAL = global;
    global.root = global;
    global.console =  process.binding(process.binding.console);
    global.Buffer = Native.require('buffer');
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

  function Native(id) {
    this.id = id;
    this.filename = id + '.js';
    this.exports = {};
  }

  Native.prototype.compile = function() {
    var source = process.native_sources[this.id];
    source = Native.wrap(source);
    var fn = process.compile(source);
    fn(this.exports, Native.require, this);
  };

  Native.require = function(id) {
    if (id == 'native') {
      return Native;
    }

    var nativeMod = new Native(id);
    nativeMod.compile();
    return nativeMod.exports;
  };

  Native.wrap = function(script) {
    var header = Native.wrapper[0] + script;
    footer = header + Native.wrapper[1];
    return footer;
  };

  Native.wrapper = [
    '(function (exports, require, module) {\n',
    '\n});'
  ];

  process.JSONParse = function(text) {
      return process.compile("(" + text + ");");
  };

  start_lainjs();
});
