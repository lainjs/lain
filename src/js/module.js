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

var Native = require('native');

function Module(id) {
  this.id = id;
  this.exports = {};
  this.filename = id + ".js";
}

module.exports = Module;

Module.load = function(id,isMain) {
  if(process.native_sources[id]){
    return Native.require(id);
  }

  var module = new Module(id);
  if(isMain){
    module.id = 'main';
    module.filename = id;
  }

  var source = process.readSource(module.filename);
  var fn = process.wrappingAsFunction(source);
  fn(module.exports, module.require, module);
  return module.exports;
};

Module.runMain = function() {
  Module.load(process.argv[1], true);
  process.onNextTick();
};

Module.prototype.require = function(id) {
  return Module.load(id);
};
