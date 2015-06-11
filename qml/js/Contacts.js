.pragma library
.import QtQuick.LocalStorage 2.0 as Sql
var privateScope = {
    getDatabase : function() {
        return Sql.LocalStorage.openDatabaseSync("harbourTox", "1.0", "Database of application harbourTox", 100000);
    }
}

function getContacts(){
    var retval = [];
    var cts = tox.getContacts();
    for(var i = 0; i< cts.length(); i++){
        c = cts[i];
        ct = {rowid: i, friendId: c.friendNumber, toxId: c.friendNumber, name: c.getName(), status: c.getStatusMessage()};
        retval.push(ct);
    }
    return retval;
}

/*
function getContacts(){
    var db = privateScope.getDatabase();
    var retval = [];
    db.transaction(function (tx) {
        var res = tx.executeSql('SELECT rowid, toxId, name from contact');
        for (var i = 0; i < res.rows.length; i++) {
            var item = {
                rowid: res.rows.item(i).rowid,
                toxId: res.rows.item(i).toxId,
                name: res.rows.item(i).name
            }
            retval.push(item);
        }
    });
    return retval;
}*/

function initializeContact(tx) {
    tx.executeSql('CREATE TABLE IF NOT EXISTS contact(toxId varchar(128), name varchar(128))');
    console.debug("Table contact initialized");
}

function contactAdd(toxId, friend_number){
    console.log("Adding contact ", toxId, "(", name, ")");
    var db = privateScope.getDatabase();
    db.transaction(function (tx){
       tx.executeSql('INSERT INTO contact(toxId, name) VALUES(?, ?);', [toxId, friend_number]);
    });
//    tox.addContact(toxId, name);
}

function removeContactValue(contactValue) {
    console.log("remove history value, rowid: ", contactValue.rowid, ", toxId: ", contactValue.toxId);
    var db = privateScope.getDatabase();
    db.transaction(function (tx) {
        tx.executeSql('DELETE FROM contact WHERE ROWID = ?;', [contactValue.rowid]);
    });
}
function copyValue(contactValue){
    var copy = {
        rowid: contactValue.rowid,
        toxId: contactValue.toxId,
        name: contactValue.name
    }
}

function initialize(){
    var db = privateScope.getDatabase();
    db.transaction(function(tx){
        initializeContact(tx);
    });

}
