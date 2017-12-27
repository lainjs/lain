var net = require('net');

var server = net.createServer();

server.listen({port:1234}, function() {
  console.log("Server> listening");
});

server.on('connection', function(socket) {
  console.log('Server> a client connected');
  socket.on('close', function() {
    console.log('Server> a client disconnected');
  });
  socket.destroy();
});

var socket = new net.Socket();

socket.connect(1234, "127.0.0.1", function(status) {
  socket.destroy();
});

socket.on('close', function() {
  console.log('Client> socket closed');
});
