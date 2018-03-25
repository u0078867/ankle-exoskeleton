

var WebSocketServer = require('ws').Server;
var ViconRemoteTrigger = require('vicon-utils').ViconRemoteTrigger;


// initialize connections parameters
var port = parseInt(process.argv[2]);

// initialize master
var masterID = process.argv[3];

// initialize slaves
var slaveID = [];
for (var i = 0; i < process.argv.length-4; i++) {
    slaveID[i] = process.argv[i+4];
}

// start websocket server
var wss = new WebSocketServer({port: port});

// initialize server connections list
var clients = {};

// initiaize Vicon remote slave trigger
var triggerVicon = new ViconRemoteTrigger('255.255.255.255', 30);
triggerVicon.createUDPClient();


wss.on('connection', function connection(ws) {

    // Get client info ad add it to clients list
    console.log(JSON.stringify(ws.upgradeReq.headers));
    var clientID = ws.upgradeReq.connection.remoteAddress;
    var ID = ws.upgradeReq.headers['id'];
    if (ID != undefined) {
        clientID += ":" + ID;
    }
    clients[clientID] = ws;

    // notify connection open
    console.log('connection open with ' + clientID);

    // set close callback
    ws.on('close', function() {
        delete clients[clientID];
        console.log('connection closed with ' + clientID);
    })

    // check if unwanted clients are connected
    var connect = false;
    for (var i = 0; i < slaveID.length; i++) {
        if (slaveID[i] != 'vicon') {
            connect = connect || (clientID == slaveID[i]);
        }
    }
    connect = connect || (clientID == masterID);
    if (!connect) {
        console.log('client neither master nor any slave. It will be disconnected.');
        ws.close();
        return;
    }

    // set message received callback
    ws.on('message', function(message) {
        console.log('message arrived from ' + clientID);
        var msg = JSON.parse(message);
        if (clientID == masterID) {
            for (var i = 0; i < slaveID.length; i++) {
                // forward message to slaves
                if (slaveID[i] in clients) {
                    clients[slaveID[i]].send(message);
                } else {
                    if (slaveID[i] == 'vicon') {
                        triggerVicon.setFileName(msg.fileName);
                        if (msg.value == 'start') {
                            triggerVicon.sendStart();
                        } else if (msg.value == 'stop') {
                            triggerVicon.sendStop();
                        }
                    }
                }
            }
        } else {
            console.log('Messages from slaves are discarded');
        }
    });

});
