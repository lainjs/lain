var stream = require('stream');
var util = require('util');
var Stream = stream.Stream;
var Duplex = stream.Duplex;

var streamBuiltin = process.binding(process.binding.stream);

function WriteReq(chunk, callback) {
  this.chunk = chunk;
  this.callback = callback;
}

function WritableState(options) {
  // buffer of WriteReq
  this.buffer = [];

  // 'true' if stream is ready to write.
  this.ready = false;

  // `true` if stream is writing down data underlying system.
  this.writing = false;

  // become true when `end()` called.
  this.ended = false;
}

function Writable(options) {
 // FIXME: There is inherits bug.
/*
  if (!(this instanceof Writable) && !(this instanceof stream.Duplex)) {
    return new Writable(options);
  }
*/

  this._writableState = new WritableState(options);

  Stream.call(this);
}


util.inherits(Writable, Stream);

// Write chunk of data to underlying system stream.
//  Control flow in general description
//    Writable.prototype.write()
//      ||
//    writeOrBuffer()
//      ||
//    doWrite()
//      ||
//    ConcreteStream.prototype._write()
//      |
//    underlying stream
//      |
//    Writable.prototype._onwrite()
Writable.prototype.write = function(chunk, callback) {
  var state = this._writableState;
  var res = false;

  if (state.ended) {
    //writeAfterEnd(this, callback);
  } else {
    res = writeOrBuffer(this, chunk, callback);
  }

  return res;
};

// This function object naver to be called. concrete stream should override
// this method.
Writable.prototype._write = function(chunk, callback, onwrite) {
  throw new Error('unreachable');
}

// When stream is ready to write, concrete stream implementation should call
// this method to inform it.
Writable.prototype._readyToWrite = function() {
  var state = this._writableState;

  state.ready = true;
  writeBuffered(this);
};

// A chunk of data has been written down to stream.
Writable.prototype._onwrite = function(status) {
  var state = this._writableState;

  state.writing = false;
  writeBuffered(this);
};

// A write call occured after end.
function writeAfterEnd(stream, callback) {
  var err = new Error('write after end');
  stream.emit('error', err);
  process.nextTick(function(){
    callback(err);
  });
}

function writeOrBuffer(stream, chunk, callback) {
  var state = stream._writableState;

  if (util.isString(chunk)) {
    chunk = new Buffer(chunk);
  }

/*
  if (!state.ready || state.writing || state.buffer.length > 0) {
    // stream not yet ready or there is pending request to write.
    // push this request into write queue.
    // state.buffer.push(new WriteReq(chunk, callback));
  } else {
    // here means there is no pending data. write out.
    // doWrite(stream, chunk, callback);
  }
*/
  doWrite(stream, chunk, callback);
}

function writeBuffered(stream) {
  var state = stream._writableState;
  if (state.buffer.length > 0 && !state.writing) {
    var req = state.buffer.shift();
    doWrite(stream, req.chunk, req.callback);
  }
}

function doWrite(stream, chunk, callback) {
  var state = stream._writableState;

  if (state.writing) {
    return new Error('write during writing');
  }

  // The stream is now writing.
  state.writing = true;

  // Write down the chunk data.
  stream._write(chunk, callback, stream._onwrite);
}

module.exports = Writable;
