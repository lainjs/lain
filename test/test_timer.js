var timerobj = new Timer();
timerobj.start(500, 0, function() {
  console.log("JS Timer fired");
});
console.log("JS Timer timeout in 500 msec...");

var repeat_cnt = 1;
var timerobj2 = new Timer();
timerobj2.start(400, 100, function() {
  console.log("Js Timer repeat #" + repeat_cnt);
  if (repeat_cnt >= 5) {
    timerobj2.stop();
  }
  repeat_cnt++;
});
console.log("JS Timer repeat in 400 msec, 100 msec interval, 5 times...");
