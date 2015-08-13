/*
    Copyright (C) 2013 by emmanuelduv <emmanuelduviviers49@hotmail.com>
    This file is part of harbour-tox Qt GUI.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the COPYING file for more details.
*/
#include "toxwrapper.h"
QHash<Tox*, toxWrapper*> toxWrapper::toxs;

toxWrapper::toxWrapper(QObject *parent)
    : QThread(parent){
    bool ir = false;
    save_data = NULL;
    my_tox = NULL;
    video_probe = VideoProbe::getInstance(this);
#ifdef DEBUG
    qDebug() << "chargé?";
#endif
    ir = true;//init();
    if(!ir) throw QString("ToxWrapper init failed!");
}

void toxWrapper::run(){
#ifdef DEBUG
    uint64_t iter = 0;
    qint64 debut = QDateTime::currentMSecsSinceEpoch();
    qDebug() << debut;
#endif
    if(my_tox == NULL) init();
    to_quit = false;
    while(to_quit == false){
        tox_iterate(my_tox);
        QThread::msleep(tox_iteration_interval(my_tox));
#ifdef DEBUG
        if(iter % 300 ==  0) qDebug() << iter << " : " << QDateTime::currentMSecsSinceEpoch();
        //if(QDateTime::currentMSecsSinceEpoch() > debut + 1000000) to_quit = true;//toxQuit(true);
        iter ++;
#endif
    }
#ifdef DEBUG
        qDebug() << iter << " : thread end at " << QDateTime::currentMSecsSinceEpoch();
#endif

}

void toxWrapper::toxQuit(bool _quit, bool wait =true){
#ifdef DEBUG
    qDebug() << "quit : " << _quit << ", wait : " << wait;
    qDebug() << "Time : " << QDateTime::currentMSecsSinceEpoch();
#endif
    to_quit = _quit;
    if(wait){
        this->wait(2000);
    }
#ifdef DEBUG
    qDebug() << "End time : " << QDateTime::currentMSecsSinceEpoch();
#endif

}

bool toxWrapper::init(){
    bool ir = true, bootstrap_ok = false;
    setSavePath(QStandardPaths::writableLocation(QStandardPaths::DataLocation).append("/.tox/save"));/*QStandardPaths::displayName(QStandardPaths::HomeLocation)*/
    QDir d(getSavePath());
    if(!d.exists()){
#ifdef DEBUG
        qDebug() << "Creating " << d.absolutePath().left(d.absolutePath().lastIndexOf("/"));
#endif
        ir = d.mkpath(d.absolutePath().left(d.absolutePath().lastIndexOf("/")));
#ifdef DEBUG
        qDebug() << ir;
#endif

    }
    if(save_data_filename.length() > 0)
     load();
    TOX_ERR_BOOTSTRAP err;
    Settings * s = Settings::getInstance(this);
    QList<Settings::DhtServer> & l = s->getBootstrapServers();
    int cpt = 0, bootstrap_cpt = 0;
    toxOptions.ipv6_enabled = false;
    toxOptions.udp_enabled = true;
    // No proxy by default
    toxOptions.proxy_type = TOX_PROXY_TYPE_NONE;
    //toxOptions.proxy_host = "";
    toxOptions.proxy_port = 0;
    uint8_t * address = new uint8_t[TOX_ADDRESS_SIZE];
    TOX_ERR_NEW error;
    my_tox = tox_new(&toxOptions, (uint8_t*)+save_data, save_data_size, &error);
    if (error != TOX_ERR_NEW_OK) ir = false;
    tox_self_get_address (my_tox, address);
    self_address = CFriendAddress::toString(address);
    delete [] address;

    tox_callback_friend_request(my_tox, &friendRequestArrived, this);
    tox_callback_friend_message(my_tox, &friendMessageArrived, this);
    tox_callback_friend_name (my_tox, &nameChanged, this);
    tox_callback_friend_status (my_tox, &statusChanged, this);
    tox_callback_friend_typing(my_tox, &typingChanged, this);
    tox_callback_file_recv_control(my_tox, &fileRecvControl, this);

    CUserId * i = NULL;
#ifdef DEBUG
    qDebug() << "TOX_PUBLIC_KEY_SIZE : " << TOX_PUBLIC_KEY_SIZE;
#endif
    uint8_t * uid = new uint8_t[TOX_PUBLIC_KEY_SIZE];
    while(bootstrap_cpt <= TOX_BOOTSRAP_NB_SRV && cpt < l.length()){
        const Settings::DhtServer& dhtServer = l[cpt];
        i = new CUserId(dhtServer.userId);
        if(i->size() > TOX_PUBLIC_KEY_SIZE){
            memcpy(uid, i->data(), TOX_PUBLIC_KEY_SIZE);
        }else{
            memcpy(uid, i->data(), i->size());
        }

#ifdef DEBUG
        qDebug() << "Bootstrapping from " << dhtServer.name << ", addr " << dhtServer.address.toLatin1().data()
         << ", port " << dhtServer.port << ", userId (" << i->size() << ")" << dhtServer.userId ;
#endif
        bootstrap_ok = tox_bootstrap(my_tox, dhtServer.address.toLatin1().data(), dhtServer.port, uid, &err);
#ifdef DEBUG
        qDebug() << " : ";
#endif
        if (bootstrap_ok == true && err == TOX_ERR_BOOTSTRAP_OK){
#ifdef DEBUG
            qDebug() << "Ok";
#endif
            bootstrap_cpt ++;
        }
#ifdef DEBUG
        else{
            qDebug() << "Error bootstrapping from " << dhtServer.name;
        }
#endif
        cpt++;
        if (i != NULL){
            delete i;
            i = NULL;
        }
    }
    delete uid;
//TOX_PUBLIC_KEY_SIZE
#ifdef DEBUG
    qDebug() << "toxOptions initialized";
#endif
    initContacts();
    toxWrapper::toxs.insert(my_tox, this);
    //connect(parent, SIGNAL(aboutToQuit()), tox, SLOT(quitter()));
    connect(this, SIGNAL(finished()), this, SLOT(quit()));
    connect(this, SIGNAL(friendMessageReceived(QVariant, QVariant, QVariant)), this, SLOT(friendMessageProcess(QVariant, QVariant, QVariant)), Qt::QueuedConnection);
    connect(video_probe, SIGNAL(videoFrameProbed(const QVideoFrame&)), this, SLOT(frameProbed(const QVideoFrame&)));
    return ir;
}
void toxWrapper::friendMessageProcess(QVariant friendNumber, QVariant message, QVariant type){
#ifdef DEBUG
    qDebug() << "friendNumber = " << friendNumber << ", type = " << type << ", message = ";
    qDebug() << message;
#endif
    emit messagesToUpdate();
}

void toxWrapper::frameProbed(const QVideoFrame &frame){
#ifdef DEBUG
    qDebug() << "Frame probed! " << frame.width() << " x " << frame.height();
    qDebug() << "FieldType = " << frame.fieldType();
    qDebug() << "PixelFormat = " << frame.pixelFormat();
#endif
}

void toxWrapper::updateMessages(){
#ifdef DEBUG
    qDebug() << "Ici!";
#endif
}

bool toxWrapper::load(){
    return load(getSavePath());
}

void toxWrapper::fileRecvControl(Tox *tox, uint32_t friend_number, uint32_t file_number, TOX_FILE_CONTROL control, void *user_data){

}

void toxWrapper::fileRecvControl(uint32_t friend_number, uint32_t file_number, TOX_FILE_CONTROL control, void *user_data){

}

QString & toxWrapper::getFriendName(uint32_t friendId){
    ToxFriend* f = v_contacts[friendId];
    if (f != 0){
        return f->getName();
    }
}

QString & toxWrapper::getFriendAddress(uint32_t friendId){
    ToxFriend* f = v_contacts[friendId];
    if (f != 0){
        return f->toxId();
    }
}

/*WARNING : load & save not thread safe (maybe others too)*/
bool toxWrapper::load(const QString & save){
    bool r = true;
#ifdef DEBUG
    qDebug() << "toxWrapper::load : from" << save;
#endif
    QFile * f = new QFile(save);
    r = f->open(QIODevice::ReadOnly);
    if (!r){
#ifdef DEBUG
        qDebug() << "Unable to open file " << save << " : error " << f->error() << " " << f->errorString();
#endif
        if(f) delete f;
        qDebug() << "Returning false";
        return false;
    }
    QByteArray d = f->readAll();
    f->close();
    delete f;
    save_data_size = d.size();
    if(save_data != NULL) delete [] save_data;
    save_data = new char[save_data_size];
    save_data = (char*)memcpy(save_data, d.constData(), d.size());
    return true;
}

bool toxWrapper::save(QString * file){
#ifdef DEBUG
    if (file == NULL)qDebug() << "saving to " << save_data_filename;
    else qDebug() << "Saving to " << file;
#endif
    QFile * f;
    qint64 w;
    bool r = false;
    save_data_size = tox_get_savedata_size(my_tox);
    if(save_data_size > 0){
        if(save_data != NULL){
            delete [] save_data;
            save_data = NULL;
        }
        save_data = new char[save_data_size];
        tox_get_savedata(my_tox, (uint8_t *)save_data);
        if(file == NULL)f = new QFile(save_data_filename);
        else f = new QFile(*file);
        r = f->open(QIODevice::WriteOnly| QIODevice::Truncate);
        if(!r){
            qDebug() << "Unable to open " << f->fileName() << " for writing : error " << f->error() << " " << f->errorString();
            return false;
        }
        w = f->write(save_data, save_data_size);
#ifdef DEBUG
        qDebug() << w <<" b written / " << save_data_size;
#endif
        f->close();
        if (w == save_data_size) r = true;
        delete f;
    }
    return r;
}

void toxWrapper::quitter(){
    toxQuit(true, true);
    save(&save_data_filename);
    save_data_filename.clear();
}

toxWrapper::~toxWrapper(){
    if(save_data_filename.length() >= 1)
     save(&save_data_filename);
    ToxFriend * f;
    int i;
    for(i=0; i < contacts.count(); i++){
        f = contacts.at(i);
        contacts.removeAt(i);
        v_contacts.remove(f->friendNumber());
        delete f;
    }
    toxWrapper::toxs.remove(my_tox);
    tox_kill(my_tox);
}

uint32_t toxWrapper::addContact(const QString & address, const QString & message, bool send_message){
#ifdef DEBUG
    qDebug() << "address = " << address;
#endif
    return addContact(CFriendAddress(address).data(), message, send_message);
}

uint32_t toxWrapper::addContact(const uint8_t * address, const QString & message, bool send_message){
#ifdef DEBUG
    qDebug() << "message = " << message << "(" << TOX_ERR_FRIEND_ADD_ALREADY_SENT << TOX_ERR_FRIEND_ADD_BAD_CHECKSUM << TOX_ERR_FRIEND_ADD_MALLOC
                << TOX_ERR_FRIEND_ADD_NO_MESSAGE << TOX_ERR_FRIEND_ADD_NULL << TOX_ERR_FRIEND_ADD_OWN_KEY << TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM
                << TOX_ERR_FRIEND_ADD_TOO_LONG << ")";
#endif
    uint32_t friend_number;
    CString msg(message);
    ToxFriend * f;
    TOX_ERR_FRIEND_ADD error;
    if(send_message) friend_number = tox_friend_add ( my_tox, address, msg.data(), msg.size(), &error );
    else friend_number = tox_friend_add_norequest(my_tox, address, &error);
    if(error == TOX_ERR_FRIEND_ADD_OK ){
        f = new ToxFriend(this, friend_number, my_tox);
        f->init();
        contacts.append(f);
        v_contacts.insert(friend_number, f);
    }
#ifdef DEBUG
    else qDebug() << "error : " << error;
#endif
    return friend_number;
}
//Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data
void toxWrapper::friendMessageArrived(Tox *tox, uint32_t friendnumber, TOX_MESSAGE_TYPE type, const uint8_t * messg, size_t length, void *userdata){
#ifdef DEBUG
    qDebug() << "friendMessageArrived from " << friendnumber << " : " << tox << ", length = " << length;
    for(size_t cpt=0; cpt < length; cpt++){
        qDebug() << messg[cpt];
    }
#endif
    toxWrapper* t;
    t = toxWrapper::toxs.value(tox);
    if(t != 0) t->friendMessageArrived(friendnumber, type, messg, length, userdata);
    //r.fromLocal8Bit((char*)message, length);
}

void toxWrapper::friendMessageArrived(uint32_t friendnumber, TOX_MESSAGE_TYPE type, const uint8_t * messg, size_t length, void *userdata){
    ToxFriend* f = v_contacts[friendnumber];
    if (f != 0){
        QString & _message = f->messageArrived(type, messg, length, userdata);
        //emit messagesToUpdate();
        emit friendMessageReceived(QVariant(friendnumber), QVariant(_message), QVariant(type));
    }
#ifdef DEBUG
    else qDebug() << "toxWrapper::friendMessageArrived, friend = 0!";
    for(size_t cpt=0; cpt < length; cpt++){
        qDebug() << messg[cpt];
    }
#endif
}

void toxWrapper::friendRequestArrived(Tox* tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data){
    toxWrapper* t = toxWrapper::toxs.value(tox);
#ifdef DEBUG
    qDebug() << "friendRequestArrived : " << tox;
#endif
    if(t != 0) t->friendRequestArrived(public_key, message, length, user_data);
    /*r.fromLocal8Bit((char*)message, length);*/

}

void toxWrapper::friendRequestArrived(const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data){
#ifdef DEBUG
    qDebug() << "toxWrapper::friendRequestArrived";
#endif
    QString pub_key = CUserId::toString(public_key);
    QString msg;
    /*pub_key.fromLocal8Bit((const char*)public_key, TOX_PUBLIC_KEY_SIZE);*/
    msg.fromLocal8Bit((const char*)message, length);
#ifdef DEBUG
    qDebug() << "toxWrapper::friendRequestArrived : ";// << msg << " from " << pub_key;
#endif
    friend_requests.insert(pub_key, msg);
}

void toxWrapper::nameChanged(Tox *tox, uint32_t friendnumber, const uint8_t *newname, size_t length, void *userdata){
#ifdef DEBUG
    qDebug() << "ToxWrapper::nameChanged()";
#endif
    toxWrapper* t = toxWrapper::toxs.value(tox);
    if(t != 0) t->nameChanged(friendnumber, newname, length, userdata);/*v_contacts[friendnumber];
    f->nameChanged(newname, length, userdata);*/
}
void toxWrapper::nameChanged(uint32_t friendnumber, const uint8_t *newname, size_t length, void *userdata){
    ToxFriend *f = v_contacts[friendnumber];
    if(f!=0) f->nameChanged(newname, length, userdata);
}

void toxWrapper::statusMessageChanged(Tox *tox, int32_t friendnumber, const uint8_t *newstatus, uint16_t length, void *userdata){
#ifdef DEBUG
    qDebug() << "statusMessageChanged for friend " << friendnumber;
#endif
    toxWrapper* t = toxWrapper::toxs.value(tox);
    if(t != 0) t->statusMessageChanged(friendnumber, newstatus, length, userdata);
#ifdef DEBUG
    else qDebug() << "No ToxWrapper for tox " << tox;
#endif
}

void toxWrapper::statusMessageChanged(int32_t friendnumber, const uint8_t *newstatus, uint16_t length, void *userdata){
#ifdef DEBUG
    qDebug() << "toxWrapper::statusMessageChanged";
#endif
    ToxFriend * f = v_contacts[friendnumber];
    if(f != 0) f->statusMessageChanged(newstatus, length, userdata);
#ifdef DEBUG
    else qDebug() << "No such friend " << friendnumber;
#endif

}

void toxWrapper::typingChanged(Tox *tox, uint32_t friendnumber, bool is_typing, void *userdata){
#ifdef DEBUG
    qDebug() << "typingChanged for friend " << friendnumber;
#endif
    toxWrapper* t = toxWrapper::toxs.value(tox);
    if(t != 0) t->typingChanged(friendnumber, is_typing, userdata);
#ifdef DEBUG
    else qDebug() << "No ToxWrapper for tox " << tox;
#endif
}

void toxWrapper::typingChanged(int32_t friendnumber, bool is_typing, void *userdata){
#ifdef DEBUG
    qDebug() << "toxWrapper::typingChanged, new value : " << is_typing;
#endif
    ToxFriend * f = v_contacts[friendnumber];
    if(f != 0) f->typingChanged(is_typing, userdata);
#ifdef DEBUG
    else qDebug() << "No such friend " << friendnumber;
#endif
}

void toxWrapper::statusChanged(Tox *tox, uint32_t friendnumber, TOX_USER_STATUS status, void *userdata){
#ifdef DEBUG
    qDebug() << "toxStatusChanged for friend " << friendnumber;
#endif
    toxWrapper* t = toxWrapper::toxs.value(tox);
    if(t != 0) t->statusChanged(friendnumber, status, userdata);
#ifdef DEBUG
    else qDebug() << "No ToxWrapper for tox " << tox;
#endif
}

void toxWrapper::statusChanged(uint32_t friendnumber, TOX_USER_STATUS status, void *userdata){
#ifdef DEBUG
    qDebug() << "toxWrapper::statusChanged, new status : " << status;
#endif
    ToxFriend * f = v_contacts[friendnumber];
    if(f != 0) f->statusChanged(status, userdata);
#ifdef DEBUG
    else qDebug() << "No such friend " << friendnumber;
#endif
}

bool toxWrapper::typingTo(uint32_t friend_number, bool typing){
    TOX_ERR_SET_TYPING err;
    bool r = tox_self_set_typing(my_tox, friend_number, typing, &err);
    if(r == false || err != TOX_ERR_SET_TYPING_OK) return false;
    return true;
}

bool toxWrapper::setName(const QString& name){
    bool r;
    TOX_ERR_SET_INFO e;
    CString * cn = new CString(name);
#ifdef DEBUG
    qDebug() << "setName";
#endif
    r = tox_self_set_name (my_tox, cn->data(), cn->size(), &e);
    delete cn;
    return r;
}
QString toxWrapper::getAddress(){
#ifdef DEBUG
    qDebug() << "self_address : " << self_address;
#endif
    return self_address;
}
bool toxWrapper::initContacts(){
#ifdef DEBUG
    qDebug() << "toxWrapper::initContacts";
#endif
    size_t nb_contact = 0;
    size_t contact_num=0;
    uint32_t * _contacts;
    ToxFriend * le_contact;
    nb_contact = tox_self_get_friend_list_size ( my_tox);
#ifdef DEBUG
    qDebug() << nb_contact << "contacts";
#endif
    _contacts = new uint32_t[nb_contact];
    tox_self_get_friend_list(my_tox, _contacts);
    for(contact_num = 0; contact_num < nb_contact; contact_num++){
        le_contact = new ToxFriend(this, _contacts[contact_num], my_tox);
        le_contact->init();
        this->contacts.append(le_contact);
        this->v_contacts.insert(_contacts[contact_num], le_contact);
    }
    delete [] _contacts;
    return true;
}

QList<ToxFriend*>& toxWrapper::getContacts(){
#ifdef DEBUG
    qDebug() << "length = " << contacts.length();
#endif
    return contacts;
}

int toxWrapper::count(){
    return contacts.length();
}

ToxFriend* toxWrapper::getContact(int index){
    if(index >= contacts.length() || index < 0)
        return NULL;
    return contacts[index];
}

bool toxWrapper::isTyping(uint32_t friendId){
    qDebug() << "toxWrapper::isTyping";
    ToxFriend * f = v_contacts[friendId];
    bool typing = false;
    if(f != 0) typing = f->typing();
#ifdef DEBUG
    else qDebug() << "No such friend " << friendId;
#endif
    return typing;
}

uint64_t toxWrapper::getLastOnline(uint32_t friendId){
    qDebug() << "toxWrapper::getLastOnline";
    ToxFriend * f = v_contacts[friendId];
    uint64_t last_online = 0;
    if(f != 0) last_online = f->lastOnline();
#ifdef DEBUG
    else qDebug() << "No such friend " << friendId;
    qDebug() << "last_online = " << last_online;
#endif
    return last_online;
}

TOX_USER_STATUS toxWrapper::getSelfStatus(){
#ifdef DEBUG
    qDebug() << "getSelfStatus";
#endif
    return tox_self_get_status ( my_tox) 	;
}
TOX_CONNECTION toxWrapper::connected(){
#ifdef DEBUG
    qDebug() << "toxWrapper::connected";
#endif
    c = tox_self_get_connection_status (my_tox);
    return c;
}

void toxWrapper::connected(TOX_CONNECTION connection_status, void *user_data){
    c = connection_status;
}

void toxWrapper::connected(Tox *tox, TOX_CONNECTION connection_status, void *user_data){
#ifdef DEBUG
    qDebug() << "connected";
#endif
    toxWrapper* t = toxWrapper::toxs.value(tox);
    if(t != 0) t->connected(connection_status, user_data);
#ifdef DEBUG
    else qDebug() << "No ToxWrapper for tox " << tox;
#endif
}

int toxWrapper::getStatus(uint32_t friendId){
    TOX_ERR_FRIEND_QUERY e;
    int friend_status = tox_friend_get_status( my_tox, friendId, &e) ;
    if(e==TOX_ERR_FRIEND_QUERY_OK ){
        return friend_status;
    }else{
        return e;
    }
}
int toxWrapper::removeContact(int friendNumber){
    return friendNumber;
}

QString toxWrapper::getStatusMessage(uint32_t friendId){
    TOX_ERR_FRIEND_QUERY e;
    QString r("");
    uint8_t * m;
    bool mr;
#ifdef DEBUG
    qDebug() << "toxWrapper::getStatusMessage, friendId = " << friendId;
#endif
    size_t s = tox_friend_get_status_message_size( my_tox, friendId, &e);
    if(e == TOX_ERR_FRIEND_QUERY_OK ){
        m = new uint8_t[s];
        mr = tox_friend_get_status_message ( my_tox, friendId, m, &e);
        if(e == TOX_ERR_FRIEND_QUERY_OK && mr == true){
            r = QString::fromUtf8((char*)m, s);
        }
#ifdef DEBUG
        else qDebug() << "tox_friend_get_status_message failed!";
#endif
        delete[] m;
    }
#ifdef DEBUG
    else{
        qDebug() << "tox_friend_get_status_message_size failed!";
        r = "";
    }
#endif
    return r;
}
bool toxWrapper::setStatusMessage(const QString & message){
#ifdef DEBUG
    qDebug() << "toxWrapper::setStatusMessage, setting message " << message;
#endif
    CString msg (message);
    bool r = true;
    TOX_ERR_SET_INFO err;
    r = tox_self_set_status_message(my_tox, msg.data(), msg.size(), &err);
    if(err != TOX_ERR_SET_INFO_OK) r = false;
#ifdef DEBUG
    if(r) qDebug() << "Ok";
    else qDebug() << "Failed!";
#endif
    return r;
}

QString toxWrapper::getSelfStatusMessage(){
    TOX_ERR_FRIEND_QUERY e = TOX_ERR_FRIEND_QUERY_OK;
    QString r;
    uint8_t * m;
    size_t s = tox_self_get_status_message_size( my_tox);
    if(e == TOX_ERR_FRIEND_QUERY_OK ){
        m = new uint8_t[s];
        tox_self_get_status_message ( my_tox, m);
        if(e == TOX_ERR_FRIEND_QUERY_OK ){
            r = QString::fromUtf8((char*)m, s);
        }
        delete[] m;
    }else{
        r = "";
    }
    return r;
}

bool toxWrapper::sendMessage(uint32_t friend_number, const QString & message){
#ifdef DEBUG
    qDebug() << "message = " << message;
#endif
    return sendMessage(friend_number, message, TOX_MESSAGE_TYPE_NORMAL);
}
bool toxWrapper::sendMessage(uint32_t friend_number, const QString & message, TOX_MESSAGE_TYPE t){
    ToxFriend * f = v_contacts[friend_number];
    if(f != 0) return f->sendMessage(message, t, 0);
    return false;
}

void toxWrapper::setSavePath(const QString & save){
    save_data_filename = save;
}

QString & toxWrapper::getSavePath(){
    return save_data_filename;
}
