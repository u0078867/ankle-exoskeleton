
var dgram = require('dgram');
var WebSocket = require('ws');

var udp = dgram.createSocket('udp4');

var url = process.argv[2];
var port = parseInt(process.argv[3]);
var portUDP = parseInt(process.argv[4]);

var ws = new WebSocket('ws://' + url + ':' + port + '/', {headers: {id:"uc"}});

var timeStart;

ws.on('open', function() {
    console.log('connection open');
    timeStart = Date.now();
});

ws.on('close', function() {
    console.log('connection closed');
});

ws.on('message', function(message) {
    console.log('message arrived: ' + message);
    var parts = message.split('|');
    var context = parts[0];
    var param = parts[1];
    var action = parts[2];
    var msg = param + '|';
    if (action == 'R') {
        msg += Math.random();
    } else {
        msg += 'Written '+ parts[3];
    }
    ws.send(msg);
});

// "SAMPLES|xxxxxxx,x,x,x,x,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,x;"
var msg = '';
var id = setInterval(function () {
    msg = 'SAMPLES|';
    var ts = Date.now() - timeStart;
    msg += ts + ',';
    for (var i = 0; i < 9; i++) {
        msg += ((Math.random() * 10) + 10) + ',';
    }
    msg += '0,0,;';
    if (!isNaN(portUDP)) {
        // Use UDP
        var message = new Buffer(msg);
        udp.send(message, 0,
            message.length, portUDP,
            url, function (err, bytes) {
                if (err) {
                    throw err;
                }
                //udp.close();
            }
        );
    } else {
        // Use TCP
        ws.send(msg);
    }
}, 10);
