'use strict';

var mysql = require('mysql2');

var connect = function connect(host, user, password, database) {
    var connection = mysql.createConnection({
        host: host,
        user: user,
        password: password,
        database: database
    });

    connection.connect(function (err) {
        if (err) throw err;
    });
    return connection;
};

module.exports.connect = connect;