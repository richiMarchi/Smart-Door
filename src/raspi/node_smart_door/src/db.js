const mysql = require('mysql2');

const connect = function(host, user, password, database) {
    const connection = mysql.createConnection({
        host: host,
        user: user,
        password: password,
        database: database
    });
    
    connection.connect(function(err) {
        if (err) throw err;
    });
    return connection;
}

module.exports.connect = connect;