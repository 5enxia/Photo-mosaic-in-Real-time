var tweet = require('twit');
var fs = require('fs');
var file = 'twitter.json';
var T = new tweet({
  consumer_key: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789',
  consumer_secret: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789',
  access_token: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789',
  access_token_secret: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
});

var stream = T.stream('statuses/filter', {track: 'ABCDE'});
stream.on('tweet', function(tw) {
  fs.writeFileSync(file, JSON.stringify(tw), null, null);
  fs.readFile(file, 'utf8', function (err, tw) {
    console.log(JSON.stringify(tw));
  });
});
