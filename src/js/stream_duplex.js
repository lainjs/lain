
var util = require('util');
var Readable = require('stream_readable');
var Writable = require('stream_writable');


function Duplex(options) {
  // FIXME: There is inherits bug.
/*
  if (!(this instanceof Duplex)) {
    return new Duplex(options);
  }
*/

  Readable.call(this, options);
  Writable.call(this, options);
}


// Duplex inherits Redable.
util.inherits(Duplex, Readable);

// Duplex should also inherits Writable but there are no way for inheriting
// from mutiple parents. Copy properties from Writable.
var keys = Object.keys(Writable.prototype);
for (var i = 0; i < keys.length; ++i) {
    var key = keys[i];
    if (!Duplex.prototype[key]) {
        Duplex.prototype[key] = Writable.prototype[key];
    }
}

module.exports = Duplex;
