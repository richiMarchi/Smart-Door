const express = require('express');
const app = express();
const path = require('path')
const server = require('http').Server(app);
const io = require('socket.io')(server);
const dateFormat = require('dateformat');
const findUp = require('find-up');
const fs = require('fs');
const db = require('./db');
const Led = require('./mocks/Led');
const AccessType = require('./AccessType');

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const configPath = findUp.sync(['.smartdoorrc', '.smartdoorrc.json'], {cwd: __dirname});
const config = configPath ? JSON.parse(fs.readFileSync(configPath)) : {};

var username;
var lvalue;
var temp;

const argv = require('yargs')
            .usage('Usage: $0 [options]')
            .config(config)
            .config()
            .options({
                'serverPort': {
                    alias: 'svp',
                    demandOption: true,
                    describe: 'Server listening port',
                    type: 'number'
                },
                'serialPort': {
                    alias: 'srp',
                    demandOption: true,
                    describe: 'Serial port to open',
                    type: 'string'
                },
                'baudRate': {
                    alias: 'br',
                    demandOption: true,
                    describe: 'Serial port baud rate',
                    type: 'number'
                },
                'ledInsidePin': {
                    alias: 'lin',
                    demandOption: true,
                    describe: 'Led inside pin number',
                    type: 'number'
                },
                'ledAccessFailedPin': {
                    alias: 'lfail',
                    demandOption: true,
                    describe: 'Led access failed pin number',
                    type: 'number'
                },
                'databaseHostName': {
                    alias:'dbhost',
                    demandOption: true,
                    describe: 'Database hostname',
                    type: 'string'
                },
                'databaseUserName': {
                    alias: 'dbuser',
                    demandOption: true,
                    describe: 'Database username',
                    type: 'string'
                },
                'databasePassword': {
                    alias: 'dbpass',
                    demandOption: true,
                    describe: 'Database password',
                    type: 'string'
                },
                'databaseName': {
                    alias: 'dbname',
                    demandOption: true,
                    describe: 'Database name',
                    type: 'string'
                }
            })
            .strict()
            .check(function(args, optsArray) {
              if (!Number.isInteger(args.serverPort) || args.serverPort < 1 || args.serverPort > 65535) {
                  throw new Error("Server port must be an integer and must be > 0 and < 65536. Received: " + args.serverPort);
              }
              if (!Number.isInteger(args.ledInsidePin)) {
                  throw new Error("Led inside pin must be an integer. Received: " + args.ledInsidePin);
              }
              if (!Number.isInteger(args.ledAccessFailedPin)) {
                  throw new Error("Led access failed pin must be an integer. Received: " + args.ledAccessFailedPin);
              }
              return true;
            })
            .argv;

const conn = db.connect(argv.databaseHostName, argv.databaseUserName, argv.databasePassword, argv.databaseName);

const ledInside = new Led(argv.ledInsidePin, 'L_INSIDE');
const ledAccessFailed = new Led(argv.ledAccessFailedPin, 'L_FAILED_ACCESS');

const port = new SerialPort(argv.serialPort, { baudRate: 9600 }, err => {
    if (err) {
        console.log("Error: Couldn't open serial port", argv.serialPort);
        process.exit(1);
    }
});

const parser = port.pipe(new Readline({ delimiter: '\r\n' }));

function writeAccessLog(date, username, accessType) {
    conn.execute('INSERT INTO `logs` (`date`, `username`, `access_type`) VALUES (?, ?, ?)', [date, username, accessType.name], function(err) {
        if (err) throw err;
    });
}

function checkAccess(user, password) {
    conn.execute('SELECT * FROM `users` WHERE `username` = ? AND `password` = ?', [user, password], function(err, results, fields) {
        if (err) throw err;
        console.log("results: ", results);
        // console.log("fields: ", fields);
        if (results.length > 0) {
            console.log(results[0].username);
            port.write("aOK\n");
        } else {
            port.write("aFAIL\n");
            ledAccessFailed.blink(100);
            const now = new Date();
            writeAccessLog(now, user, AccessType.ACCESS_FAILED);
            io.sockets.emit('newAccess', createAccessLogEvent(now, user, AccessType.ACCESS_FAILED));
        }
    });
}

function getTodayLogs(socket) {
    conn.query('SELECT * FROM `logs` WHERE DATE(`date`) = CURDATE()', function(err, results, fields) {
        if (err) throw err;
        // console.log(results);
        const resArr = [];
        results.forEach(res => resArr.push({date:dateFormat(res.date, "dd-mm-yyyy HH:MM:ss"),
                                            username:res.username,
                                            description:AccessType.enumValueOf(res.access_type).description}));
        const todayLogs = {count:results.length, logs:resArr};
        // console.log(todayLogs);
        socket.emit('todayLogs', todayLogs);
    });
}

function createAccessLogEvent(date, username, accessType) {
     return {
        date:dateFormat(date, "dd-mm-yyyy HH:MM:ss"),
        username:username,
        status:accessType.status,
        description:accessType.description
    };
}

function doAction(message) {
    console.log(message);
    switch (parseInt(message.charAt(0))) {
        case 1: { // LOGIN REQUEST
            const msg = message.split("|");
            username = msg[1];
            console.log(username);
            checkAccess(msg[1], msg[2]); // 2nd token is username, 3rd is password
            break;
        }
        case 2: { // TEMPERATURE UPDATE
            const msg = message.split("|");
            temp = msg[1];
            io.sockets.emit('tempUpdate', msg[1]);
            break;
        }
        case 3: { // LVALUE UPDATE
            const msg = message.split("|");
            lvalue = msg[1];
            io.sockets.emit('lvalueUpdate', msg[1]);
            break;
        }
        case 4: { // ACCESS FAILED (PIR HAS NOT DETECTED ANYTHING)
            // flash L_FAILED
            ledAccessFailed.blink(100);
            const now = new Date();
            writeAccessLog(now, username, AccessType.ACCESS_FAILED_PIR);
            io.sockets.emit('newAccess', createAccessLogEvent(now, username, AccessType.ACCESS_FAILED_PIR));
            break;
        }
        case 5: { // SOMEONE INSIDE
            // switch on L_INSIDE
            // log event
            ledInside.switchOn();
            console.log(username);
            const now = new Date();
            writeAccessLog(now, username, AccessType.ACCESS_SUCCESS);
            io.sockets.emit('newAccess', createAccessLogEvent(now, username, AccessType.ACCESS_SUCCESS));
            break;
        }
        case 6: { // SESSION TERMINATED
            // switch off L_INSIDE
            ledInside.switchOff();
            break;
        }
        case 7: { // LOG MESSAGES FROM ARDUINO
            const msg = message.split("|");
            console.log("LOG: ", msg[1]);
            break;
        }
    }
}

console.log(new Date());

port.on('open', () => console.log("Arduino connesso"));

// try to detect if Arduino gets disconnected. It seems not to be working with virtual serial ports on Windows.
// port.on('close', function(err) {
//     if (err.disconnected) {
//         console.log("Arduino disconnected!")
//     }
// });

app.use(express.static(path.join(__dirname, 'public')));

server.listen(argv.serverPort, () => console.log("Server listening on port", argv.serverPort));

app.get('/', function (req, res) {
	res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

io.on('connection', function (socket) {
    console.log("Qualcuno si è connesso");
    console.log(new Date());
    getTodayLogs(socket);
    if (typeof lvalue !== 'undefined') {
        socket.emit('lvalueUpdate', lvalue);
    }
    if (typeof temp !== 'undefined') {
        socket.emit('tempUpdate', temp);
    }
});

parser.on('data', data => doAction(data));