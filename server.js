
var BufferDigester = require('common-utils').BufferDigester;
var WebSocketServer = require('ws').Server;
var WebSocket = require('ws');
var fs = require('fs');
var path = require('path');
var argv = require('minimist')(process.argv.slice(2));


// initialize connections parameters
var controllerID =  argv['uc'];
var uiID =          argv['ui'];
var bridgeID =      argv['br'];
var port =          argv['_'][0];

// initialize leg side
var side = argv['s'];

// Initialize trigger websocker client
var wstr = null;
if (argv['trsrv'] != undefined) {
    var options = {};
    if (argv['trcltid'] != undefined) {
        options = {headers: {id: argv['trcltid']}};
    }
    wstr = new WebSocket('ws://' + argv['trsrv'] + '/', options);
}

// start websocket server
var wss = new WebSocketServer({port: port});

// Initialize connection with websocket server
//var ws = null;

// initialize server connections list
var clients = {};

// initialize buffer
var buffer = [];

// initialize output stream
var wstream = null;

// initialize CONTROLLER connection start time
var timeStart = null;

// initialiaze function to send data to UI
var sendData = function(data) {};

// initialize variables for file saving
var fsFolderMain = null;
var fsFileName = null;
var fsFileSaving = false;

// for debugging
var cont = 0;
var handleSamplesMsg = true;


// Handle client connection to trigger server
if (wstr != null) {

    wstr.on('open', function() {
        console.log('connection with trigger server open');
    });

    wstr.on('close', function() {
        console.log('connection with trigger server closed');
    });

    wstr.on('message', function(message) {
        var msg = JSON.parse(message);
        fsFileSaving = (msg.value == 'start');
        if (fsFileSaving) {
            if (fsFileName != '') {
                wstream = createFileWriteStreamHelper(fsFolderMain, fsFileName);
                console.log("output file stream created");
            }
            if (uiID in clients) {
                clients[uiID].send(JSON.stringify({data: 'file-saving', port: 'cmd-data-saving-in'}));
            }
        } else {
            if (wstream != null) {
                wstream.end();
                wstream = null;
            }
            if (uiID in clients) {
                clients[uiID].send(JSON.stringify({data: 'no-file-saving', port: 'cmd-data-saving-in'}));
            }
        }
    });

}


// Handle server
wss.on('connection', function connection(ws) {

    /* NOTE:
    Pinging the client is the trick to get a connection closed if the client
    does not repond anymore (for instance when controller is manually reset).
    But closing automaticcally the connection from the server side can prevent
    from creating a new one from the same client. This is an unexplained
    behaviour.
    The solution is not to ping at all. The controller, when reset, will begin
    a new connection with the server, and the connection will overwrite the one
    in clients[clientID]. This is seems to work in a stable way.
    */

    /*
    ws.pingssent = 0;
    var interval = setInterval(function() {
        if (ws.pingssent >= 1) {
            ws.close();
        } else {
            ws.ping();
            ws.pingssent++;
        }
    }, 5000);
    ws.on('pong', function() {
        ws.pingssent = 0;
        console.log("received: pong");
    })
    */

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

    // set time start
    if (clientID == controllerID) {
        timeStart = Date.now();
    }

    // set function to send data to UI
    if (clientID == controllerID || clientID == bridgeID) {
        sendData = function(data) {
            if (uiID in clients) {
                clients[uiID].send(JSON.stringify({port: 'uc-samples', data: data}), function() {  });
                console.log('message sent to UI');
            } else {
                console.log('UI not connected, impossible to send the signal');
            }
        };
        bd.setDigestCallback(sendData);
    }

    // set close callback
    ws.on('close', function() {
        delete clients[clientID];
        console.log('connection closed with ' + clientID);
        if (clientID == uiID && wstream != null) {
            wstream.end();
            console.log('output file stream closed');
        }
    })

    // check if unwanted clients are connected
    if (!(clientID == uiID || clientID == controllerID || clientID == bridgeID)) {
        console.log('client not UI, CONTROLLER or BRIDGE. It will be disconnected.');
        ws.close();
        return;
    }

    // set message received callback
    ws.on('message', function(message) {
        // notify message arrived
        console.log('message arrived from ' + clientID);
        if (clientID == controllerID || clientID == bridgeID) {
            // split in type and payload
            var parts = message.split('|');
            var type = parts[0], payload = parts[1];
            // manage data samples from CONTROLLER
            if (type == 'SAMPLES') {
                console.log(cont);
                cont++;
                if (handleSamplesMsg) {
                    // split package in samples
                    var samples = payload.split(';');
                    samples.pop(); // last one is garbage
                    // from each sample, take first (common X) and second (Y signal 1) measure
                    for (var i = 0; i < samples.length; i++) {
                        // extract time and measure for each signal
                        var measures = samples[i].split(',');
                        measures.pop(); // last one is garbage
                        console.log(JSON.stringify(measures));
                        // parse single measures
                        var TS = parseInt(measures[0]);
                        var MT = parseFloat(measures[1]);
                        var MP = parseFloat(measures[2]);
                        var SPC = parseFloat(measures[3]);
                        var SVAN = parseFloat(measures[4]);
                        var AAN =  parseFloat(measures[5]);
                        var VAC = parseFloat(measures[6]);
                        var FVAN = parseFloat(measures[7]);
                        var MTP = parseFloat(measures[8]);
                        var AANT = parseFloat(measures[9]);
                        var MDET = parseFloat(measures[10]);
                        var GP = parseInt(measures[11]);
                        // create nex data point
                        var newSample = {
                            SIDE: side,
                            TS: TS / 1000.,
                            MT: MT,
                            MP: MP,
                            SPC: SPC,
                            SVAN: SVAN,
                            AAN: AAN,
                            FVAN: FVAN,
                            VAC: VAC,
                            MTP: MTP,
                            AANT: AANT,
                            MDET: MDET,
                            GP: GP,
                        };
                        var metadata = {
                            SIDE: "Side",
                            TS: "Time",
                            MT: "Motor torque",
                            MP: "Motor position",
                            SPC: "Spring compression",
                            SVAN: "Shank angle with vertical",
                            AAN: "Ankle angle",
                            FVAN: "Foot angle with vertical",
                            VAC: "IMU vertical acceleration",
                            MTP: "Motor target position",
                            AANT: "Ankle angle target",
                            MDET: "Motor detach",
                            GP: "Gait phase",
                        }
                        // write measure to text file
                        if (fsFileSaving && wstream != null) {
                            wstream.write(JSON.stringify(newSample) + '\n');
                        }
                        newSample.metadata = metadata;
                        // add new data point to buffer
                        buffer.push(newSample);
                        // estimate delay
                        var delay = (Date.now() - timeStart) - TS;
                        console.log('estimated delay for current sample (ms): ' + delay);
                    }
                }
            } else {
                if (uiID in clients) {
                    clients[uiID].send(JSON.stringify({port: 'cmd-uC-in', data: {param: type, msg: payload}}));
                }
            }
        } else if (clientID == uiID) {
            console.log(message);
            // parse message
            var msg = JSON.parse(message);
            // manage commands from UI
            if (msg.uri == 'send-to-uC') {
                // forward command from UI to CONTROLLER
                clients[controllerID].send(msg.payload);
            } else if (msg.uri == 'edit-Ns-Ts') {
                // adjust buffer digester settings
                var bds = msg.payload;
                bd.setN(bds.Ns);
                bd.setTs(bds.Ts);
                // send new parameters back to UI
                var bdsNew = {Ns: bd.getN(), Ts: bd.getTs()};
                clients[uiID].send(JSON.stringify({port: 'cmd-tune-bd-in', data: bdsNew}));
                console.log("buffer digester parameters set");
            } else if (msg.uri == 'edit-folder-main') {
                fsFolderMain = msg.payload;
                console.log("folder for file saving set");
            } else if (msg.uri == 'edit-file-name') {
                fsFileName = msg.payload;
                console.log("file name for file saving set");
            } else if (msg.uri == 'edit-file-saving') {
                fsFileSaving = msg.payload;
                if (fsFileSaving) {
                    if (fsFileName != '') {
                        wstream = createFileWriteStreamHelper(fsFolderMain, fsFileName);
                        console.log("output file stream created");
                    }
                    if (wstr != null) {
                        wstr.send(JSON.stringify({type: 'trigger', value: 'start', fileName: fsFileName}));
                    }
                } else {
                    if (wstream != null) {
                        wstream.end();
                        wstream = null;
                    }
                    if (wstr != null) {
                        wstr.send(JSON.stringify({type: 'trigger', value: 'stop', fileName: fsFileName}));
                    }
                }
            /*} else if (msg.uri == 'query-uC-th') {
                clients[controllerID].send('ALGOPAR|TH0|R');*/
            } else if (msg.uri == 'edit-uC-th') {
                clients[controllerID].send('ALGOPAR|TH0|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-ALGMOD') {
                clients[controllerID].send('ALGOPAR|ALGMOD|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-FVANTHS') {
                clients[controllerID].send('ALGOPAR|FVANTHS|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-VACTHS') {
                clients[controllerID].send('ALGOPAR|VACTHS|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-FVANTTO') {
                clients[controllerID].send('ALGOPAR|FVANTTO|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-VACTTO') {
                clients[controllerID].send('ALGOPAR|VACTTO|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-HOMOD') {
                clients[controllerID].send('ALGOPAR|HOMOD|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-FVANTHO') {
                clients[controllerID].send('ALGOPAR|FVANTHO|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-SVANTHO') {
                clients[controllerID].send('ALGOPAR|SVANTHO|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-TWNOHO') {
                clients[controllerID].send('ALGOPAR|TWNOHO|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-MAD') {
                clients[controllerID].send('ALGOPAR|MAD|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-MAP') {
                clients[controllerID].send('ALGOPAR|MAP|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-MASW') {
                clients[controllerID].send('ALGOPAR|MASW|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-E2R') {
                clients[controllerID].send('ALGOPAR|E2R|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-E3R') {
                clients[controllerID].send('ALGOPAR|E3R|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-E1SW') {
                clients[controllerID].send('ALGOPAR|E1SW|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-AANE') {
                clients[controllerID].send('ALGOPAR|AANE|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-ST') {
                clients[controllerID].send('ALGOPAR|ST|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-MECHBLOCK') {
                clients[controllerID].send('SETT|MECHBLOCK|W|' + msg.payload);
            } else if (msg.uri == 'edit-uC-SERVOBEH') {
                clients[controllerID].send('SETT|SERVOBEH|W|' + msg.payload);
            }
        }
    });

});

function createFileWriteStreamHelper(folderPath, fileName) {
    return createFileWriteStream(path.join(folderPath, fileName + '.txt'));
};

function createFileWriteStream(filePath) {
    var wstream = fs.createWriteStream(filePath);
    wstream.on('finish', function(){
        console.log('file has been written');
    });
    console.log('output file stream open');
    return wstream;
}


// Following values tuned to send data at N/Ts FPS (supported by UI)


// OK (FPS: 24 Hz)
var Ts = 250; // ms
var N = 6;


/*
// OK (FPS: 35 Hz)
var Ts = 200; // ms
var N = 7;
*/

/*
// OK (FPS: 33 Hz)
var Ts = 300; // ms
var N = 10;
*/

// --- without charts

/*
// OK (100+ Hz)
var Ts = 100; // ms
var N = 10;
*/

/* In the following digest loop, the data buffer must be emptied.
The buffer contains Nc frames (depending on Fc). Only N can be sent.
*/

/*
var cont = 0;

var updateFrameNumber = function(p) {
    cont++;
    p.frameNumber = cont;
};*/

var bd = new BufferDigester(
    buffer, sendData, {
        N: N,
        Ts: Ts,
        //preDigestPckgEditCallback: updateFrameNumber,
        selectPckgMethod: 'equi-sample',
});

bd.startDigestLoop();
