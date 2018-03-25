
var dgram = require('dgram');
var WebSocket = require('ws');


var hostIn = process.argv[2];
var portIn = parseInt(process.argv[3]);
var hostOut = process.argv[4];
var portOut = parseInt(process.argv[5]);
var hostId = process.argv[6];

// WebSocket data-forwarding client
var options = {};
if (hostId != undefined) {
    options = {headers: {id: hostId}};
}
var client = new WebSocket('ws://' + hostOut + ':' + portOut + '/', options);

client.on('open', function() {
    console.log('connection with WebSocket server open');
});

client.on('close', function() {
    console.log('connection with WebSocket server closed');
});


// UDP server listenting to uC
var server = dgram.createSocket('udp4');

server.on('listening', function() {
    var address = server.address();
    console.log('UDP server listening on ' + address.address + ":" + address.port);
});

server.on('message', function(message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
    if (client.readyState == WebSocket.OPEN) {
        client.send(message.toString());
    }
});

server.bind(portIn, hostIn);
