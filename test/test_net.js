var net = require('net');

var server = net.createServer();

server.listen({port:1234}, function() {
  console.log("Server> listening");
});

server.on('connection', function(socket) {
  console.log('Server> a client connected');
  socket.on('read', function(err, buffer) {
    if (err) {
      console.log(err.toString());
    } else {
      console.log('Server - Client> recv: ' + buffer.toString());
    }
  });
  socket.on('close', function() {
    console.log('Server> a client disconnected');
  })
});


var socket = new net.Socket();

socket.connect(1234, "127.0.0.1", function(status) {
  if (status == 0) {
    console.log("Client> socket connect");
  } else {
    socket.destroy();
  }
});

socket.on('close', function() {
  console.log('Client> socket closed');
});

socket.write("hello - 1\n\r", function(status) {
  console.log("Client> after write");
});

socket.write("hello - 2\n\r", function(status) {
  console.log("Client> after write");
});
