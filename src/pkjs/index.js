var Clay = require('pebble-clay');
var clayConfig = require('./config.json');
var clay = new Clay(clayConfig);
var messageKeys = require('message_keys');

Pebble.addEventListener('webviewclosed', function(e) {
  var claySettings = clay.getSettings(e.response);
  Pebble.sendAppMessage(claySettings, function() {
    console.log('Message sent successfully: ' + JSON.stringify(claySettings));
  }, function(e) {
    console.log('Message failed: ' + JSON.stringify(e));
  });
});

Pebble.addEventListener('appmessage', function(e) {
  var dict = e.payload;
  console.log('Got message: ' + JSON.stringify(dict));
});

