var MODULE_PROCESS = 0;
var MODULE_CONSOLE = 1;
this.global = this;
var global = this.global;
global.console = process.binding(MODULE_CONSOLE);

console.log('LainJS Process NextTick Test ...');

var index = 0;
process.nextTick(function() {
  index++;
  console.log('#'+index+' Process Tick ...');
});

process.nextTick(function() {
  index++;
  console.log('#'+index+' Process Tick ...');
});
