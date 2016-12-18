var fs = module.exports;

var constants = process.binding(process.binding.constants);
var util = require('util');

var binding = process.binding(process.binding.fs);

var O_APPEND = constants.O_APPEND;
var O_CREAT = constants.O_CREAT;
var O_EXCL = constants.O_EXCL;
var O_RDONLY = constants.O_RDONLY;
var O_RDWR = constants.O_RDWR;
var O_SYNC = constants.O_SYNC;
var O_TRUNC = constants.O_TRUNC;
var O_WRONLY = constants.O_WRONLY;

fs.open = function(path, flags, mode, callback) {
  if (!util.isNumber(mode)) {
    mode = 0777;
  }

  binding.open(path,
               convertFlags(flags),
               mode,
               callback);
};

fs.read = function(fd, buffer, offset, length, position, callback) {
  if (!util.isBuffer(buffer)) {
    throw new TypeError('Bad arguments');
  }

  var cb_wrap = function(err, bytesRead) {
    callback && callback(err, bytesRead || 0, buffer);
  };

  binding.read(fd, buffer, offset, length, position, cb_wrap);
};

function convertFlags(flag) {
  if (!util.isString(flag)) {
    return flag;
  }

  switch (flag) {
    case 'r': return O_RDONLY;
    case 'rs':
    case 'sr': return O_RDONLY | O_SYNC;

    case 'r+': return O_RDWR;
    case 'rs+':
    case 'sr+': return O_RDWR | O_SYNC;

    case 'w': return O_TRUNC | O_CREAT | O_WRONLY;
    case 'wx':
    case 'xw': return O_TRUNC | O_CREAT | O_WRONLY | O_EXCL;

    case 'w+': return O_TRUNC | O_CREAT | O_RDWR;
    case 'wx+':
    case 'xw+': return O_TRUNC | O_CREAT | O_RDWR | O_EXCL;

    case 'a': return O_APPEND | O_CREAT | O_WRONLY;
    case 'ax':
    case 'xa': return O_APPEND | O_CREAT | O_WRONLY | O_EXCL;

    case 'a+': return O_APPEND | O_CREAT | O_RDWR;
    case 'ax+':
    case 'xa+': return O_APPEND | O_CREAT | O_RDWR | O_EXCL;
  }
  return new Error('unknown file open flag: ' + flag);
}

