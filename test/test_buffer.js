var console = require('console');
var MODULE_BUFFER = 3;

// make module 'util'
function init_util() {
  var exports = {};

  function isNull(arg) {
    return arg === null;
  };
  exports.isNull = isNull;

  function isUndefined(arg) {
    return arg === undefined;
  };
  exports.isUndefined = isUndefined;

  function isNullOrUndefined(arg) {
    return isNull(arg) || isUndefined(arg);
  };
  exports.isNullOrUndefined = isNullOrUndefined;

  function isNumber(arg) {
    return typeof arg === 'number';
  };
  exports.isNumber = isNumber;

  function isString(arg) {
    return typeof arg === 'string';
  };
  exports.isString = isString;

  function isObject(arg) {
    return typeof arg === 'object' && arg != null;
  };
  exports.isObject = isObject;

  function isFunction(arg) {
    return typeof arg === 'function';
  };
  exports.isFunction = isFunction;

  function inherits(ctor, superCtor) {
    ctor.prototype = new superCtor();
  };
  exports.inherits = inherits;

  global.util = exports;
};

init_util();


function init_buffer() {
  var buffer = global.process.binding(MODULE_BUFFER);
  var alloc = buffer.alloc;
  var kMaxLength = buffer.kMaxLength;

  function Buffer(subject, encoding) {
    if (!(this instanceof Buffer)) {
      return new Buffer(subject, encoding);
    }

    if (util.isNumber(subject)) {
      this.length = subject > 0 ? subject >>> 0 : 0;
    } else if (util.isString(subject)) {
      this.length = Buffer.byteLength(subject);
    }

    alloc(this, this.length);

    if (util.isString(subject)) {
      this.write(subject, encoding);
    } else if (util.isBuffer(subject)) {
      subject.copy(this, 0, 0, this.length);
    }
  };

  Buffer.byteLength = function(str, enc) {
    return str.length;
  }

  Buffer.prototype.write = function(string, offset, length, encoding) {
    if (util.isUndefined(offset)) {
      encoding = 'utf8';
      length = this.length;
      offset = 0;
    }
    if (util.isUndefined(length) && util.isString(offset)) {
      encoding = offset;
      length = this.length;
      offset = 0;
    }
    offset = offset >>> 0;
    if (util.isNumber(length)) {
      length = length >>> 0;
    } else {
      encoding = length;
      length = undefined;
    }

    var remaining = this.length - offset;
    if (util.isUndefined(length) || length > remaining) {
      length = remaining;
    }

    if (length < 0 || offset < 0) {
      throw 'attempt to write outside buffer bounds';
    }

    return this._write(string, offset, length);
  };

  Buffer.prototype.toString = function(encoding, start, end) {
    return this._toString();
  };

  buffer.setupBufferJs(Buffer);

  global.Buffer = Buffer;
};
init_buffer();


var buffer = new Buffer("Hello,      ");
buffer.write("Lain", 7, 4);
console.log(buffer.toString());
