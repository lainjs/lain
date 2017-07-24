var EventEmitter = require('events');
var util = require('util');

var TCP = process.binding(process.binding.tcp);

function createTCP(socket) {
  var tcp = new TCP();
  tcp._socket = socket;
  return tcp;
}

function Socket(options) {
  if (!(this instanceof Socket)) {
    return new Socket(options);
  }

  EventEmitter.call(this);

  if (util.isUndefined(options)) {
    options = {};
  }

  if (options.handle) {
    this._handle = options.handle;
    this._handle._socket = this;
  }
}

Socket.prototype = new EventEmitter();
exports.Socket = Socket;

Socket.prototype.connect = function(port, host, callback) {
  if (!this._handle) {
    this._handle = createTCP(this);
  }

  this._handle.connect(host, port, callback);

  return this;
};

Socket.prototype.destroy = function() {
  var self = this;
  self._handle.close();
};

Socket.prototype._onconnect = function() {
  // TODO: Implement stendby reading and writing data.
  // TODO: We should need data structure to implement
  // writing data.
  this._handle.readStart();
};

Socket.prototype._onclose = function() {
  this.emit('close');
};

function Server(options, connectionListener) {
  if (!(this instanceof Server)) {
    return new Server(options, connectionListener);
  }

  EventEmitter.call(this);

  if (util.isFunction(options)) {
    connectionListener = options;
    options = {};
  } else {
    options = options || {};
  }

  if (util.isFunction(connectionListener)) {
    this.on('connection', connectionListener);
  }

  this._handle = null;
}

Server.prototype = new EventEmitter();
module.exports.Server = Server;

exports.createServer = function(options, callback) {
  return new Server(options, callback);
};

Server.prototype._createTCP = createTCP;

Server.prototype.listen = function(options, callback) {
  var self = this;

  var address = "127.0.0.1";
  var port = 80;
  var backlog = 10;

  // FIXME: There is much more options for listen.
  if (util.isNumber(options.port)) {
    port = options.port;
  } else {
    throw new Error('Invalid listen argument');
  }

  // Create server handle.
  if (!self._handle) {
    self._handle = createTCP(self);
  }

  // bind port
  var err = self._handle.bind(address, port);
  if (err) {
    self._handle.close();
    return err;
  }

  // listen
  err = self._handle.listen(backlog);
  if (err) {
    self._handle.close();
    return err;
  }

  if (util.isFunction(callback)) {
    // FIXME: This should be `once`.
    self.on('listening', callback);
  }

  process.nextTick(function() {
    if (self._handle) {
      self.emit('listening');
    }
  });
};

Server.prototype._onconnection = function(status, clientHandle) {
  if (status) {
    this.emit('error', new Error('accept error: ' + status));
    return;
  }

  // Create socket object for connecting client.
  var socket = new Socket({
    handle: clientHandle,
  });

  socket.server = this;
  socket._onconnect();

  this.emit('connection', socket);
};
