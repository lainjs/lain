var EventEmitter = require('events');

var e = new EventEmitter();

e.addListener('test', function() { console.log('Call event 1'); });
e.addListener('test', function() { console.log('Call event 2'); });
e.addListener('test', function() { console.log('Call event 3'); });

e.emit('test');
