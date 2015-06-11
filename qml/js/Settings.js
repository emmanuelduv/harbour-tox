.pragma library
.import QtQuick.LocalStorage 2.0 as Sql

var privateScope = {
    getDatabase : function() {
        return Sql.LocalStorage.openDatabaseSync("harbourTox", "1.0", "Database of application harbourTox", 100000);
    }
}

function initialize(){
    var db = privateScope.getDatabase();
    db.transaction(function (tx) {
        var defaultSettings = {
            toxId: "abcd1a7b4454d5545f554e1412c",
            name: "Nemo",
            status: "Toxing on my Jolla",
            audio: 1,
            video: 1,
            proxy: "",
            proxyPort: ""
        }
        initializeSettings(defaultSettings, tx);
    });
}

function getValue(key, tx){
    var retval = undefined;
    var res = tx.executeSql('SELECT value FROM settings WHERE key = ?;', [key]);
    if(res.rows.length > 0){
        retval = res.rows.item(0).value;
    }
    return retval;
}

function initializeSettings(defaultSettings, tx) {
    tx.executeSql('CREATE TABLE IF NOT EXISTS settings(key TEXT, value TEXT, PRIMARY KEY (key))');
    for (var key in defaultSettings) {
        if(getValue(key, tx) === undefined){
            tx.executeSql('INSERT OR IGNORE INTO settings (key, value) VALUES (?, ?);', [key, defaultSettings[key]]);
            console.log('Setting default value ', defaultSettings[key], ' for key ', key)
        }
    }
    console.debug("Table SETTINGS initialized");
}

function get(key){
    var db = privateScope.getDatabase();
    var retval = undefined;
    db.transaction(function(tx){
        retval = getValue(key, tx);
        console.log('get ', key, ' => ', retval);
    })
   return retval;
}

function set(key, value){
    var db = privateScope.getDatabase();
    db.transaction(function (tx) {
    tx.executeSql('INSERT OR REPLACE INTO settings VALUES(?, ?);', [key, value]);
    });
    if(key === 'name') setName(value);
    if(key === 'status') setStatusMessage(value);
}
function setName(name){
    console.log('setting ', name, ' as name');
}
function setStatusMessage(message){
    console.log('setting ', message, ' as message');
}
