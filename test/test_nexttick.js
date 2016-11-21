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
