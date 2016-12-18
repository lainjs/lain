var fs = require('fs');

var onRead = function(err, bytesRead, buffer) {
  process.nextTick(function() {
    console.log(buffer.toString());
  });
};

var onOpen = function(err, fd) {
  if (err) {
    console.log(err);
  } else {
    var buffer = new Buffer(64);
    fs.read(fd, buffer, 0, 64, 0, onRead);
  }
};

fs.open("./test/lain.txt", "r", 0777, onOpen);
