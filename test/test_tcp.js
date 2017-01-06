var net = require('net');

var server = net.createServer();

server.listen({port:8888}, function() {
  console.log("Server Log ------- listening");
});

server.on('connection', function(socket) {
  console.log('Server Log ------- a client connected');
  socket.on('close', function() {
    console.log('Server Log ------- a client disconnected');
  })
});

var socket = new net.Socket();

socket.connect(8888, "127.0.0.1", function(status) {
  if (status == 0) {
    console.log("Client Log ------- socket connect");
  }
  socket.destroy();
});

socket.on('close', function() {
  console.log('Client Log ------- socket closed');
});
