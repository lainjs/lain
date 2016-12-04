var MODULE_PROCESS = 0;
var MODULE_CONSOLE = 1;
this.global = this;
var global = this.global;
var binding_process = process.binding(MODULE_PROCESS);
global.console = binding_process.binding(MODULE_CONSOLE);

console.log("Hello, Lain!!");
