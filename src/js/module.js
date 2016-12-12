var Native = require('native');

function Module(id) {
  this.id = id;
  this.exports = {};
  this.filename = id + ".js";
}

module.exports = Module;

Module.wrapper = Native.wrapper;
Module.wrap = Native.wrap;

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
  source = Module.wrap(source);
  var fn = process.compile(source);
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
