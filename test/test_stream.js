var ReadableStream = require('stream').ReadableStream;

var readable = new ReadableStream();

readable.on('readable', function() {
  var data = readable.read();
  console.log('read: ' + data.toString());
})

readable.push('abcde');
readable.push('12345');
