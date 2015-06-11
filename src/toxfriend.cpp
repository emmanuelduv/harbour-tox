#include "toxfriend.h"

ToxFriend::ToxFriend(QObject *parent, int friend_number, Tox * tox) :
    QObject(parent), my_tox(tox), my_friend_number(friend_number), name(* new QString()), status(* new QString())
{
#ifdef DEBUG
    qDebug() << "ToxFriend " << friend_number << " créé.";
#endif
}

QString ToxFriend::getStatusMessage(){
#ifdef DEBUG
 qDebug() << "Returning "<< status;
#endif
    return status;
}

QString ToxFriend::getName(){
#ifdef DEBUG
    qDebug() << "Returning " << name;
#endif
    return name;
}
TOX_USER_STATUS ToxFriend::getStatus(){
    TOX_ERR_FRIEND_QUERY error;
    TOX_USER_STATUS r = TOX_USER_STATUS_NONE;
    r = tox_friend_get_status (my_tox, my_friend_number, &error);
    return r;
}

void ToxFriend::nameChanged(const uint8_t *newname, size_t length, void *userdata){
    name = QString::fromUtf8((const char*)newname, length);
}
QString ToxFriend::toxId(){
    return QString("TOXID()_TO_IMPLEMENT");
}

void ToxFriend::statusMessageChanged(const uint8_t *newstatus, uint16_t length, void *userdata){
    status = QString::fromUtf8((const char*)newstatus, length);
#ifdef DEBUG
    qDebug() << "ToxFriend::statusChanged, new status = " << status;
#endif
}

void ToxFriend::statusChanged(TOX_USER_STATUS status, void *userdata){
    connect_status = status;
}

void ToxFriend::typingChanged(bool is_typing, void *userdata){
    friend_typing = is_typing;
}

bool ToxFriend::typing(){
    return friend_typing;
}

uint64_t ToxFriend::lastOnline(){
    return last_online;
}

void ToxFriend::lastOnline(uint64_t timestamp){
    last_online = timestamp;
}

QString & ToxFriend::messageArrived(TOX_MESSAGE_TYPE type, const uint8_t * message, size_t length, void *userdata){
    arrived.append(CString::toString(message, length));
#ifdef DEBUG
    qDebug() << "type=" << type << ", length=" << length << ", size=" << arrived.last().size() << ", message=";
    qDebug() << arrived.last();
#endif
    return arrived.last();
}

uint32_t ToxFriend::friendNumber(){return my_friend_number;}

QList<QString> ToxFriend::receivedMessages(){
    return arrived;
}

QList<QString> ToxFriend::sentMessages(){
    return sent.values();
}

bool ToxFriend::sendMessage(const QString & message, TOX_MESSAGE_TYPE t= TOX_MESSAGE_TYPE_NORMAL, int * id_msg = 0){
    uint32_t /*nb_msg= 0, cpt_msg = 0,*/ msg_id = 0;
    TOX_ERR_FRIEND_SEND_MESSAGE err;
    CString msg(message);
#ifdef DEBUG
    qDebug() << "message = " << message << "length = " << message.length() << ", size = " << msg.size();
#endif
    if(msg.size() <= TOX_MAX_MESSAGE_LENGTH){
        msg_id = tox_friend_send_message(my_tox, friendNumber(), t, msg.data(), msg.size(), &err);
        if(err == TOX_ERR_FRIEND_SEND_MESSAGE_OK){
            sent.insert(msg_id, message);
            if(id_msg != 0) *id_msg = msg_id;
            return true;
        }else{
#ifdef DEBUG
            qDebug() << "ToxFriend::sendMessage : tox_friend_send_message returned err " << err;
#endif
            return false;
        }
    }else{
#ifdef DEBUG
        qDebug() << "ToxFriend::sendMessage : message too long " << message;
#endif
        return false;
    }
    /* TODO : couper le message
    nb_msg = msg.size() / TOX_MAX_MESSAGE_LENGTH;
    if(nb_msg == 0 && msg.size() > 0) nb_msg = 1;
    for(cpt_msg = 0; cpt_msg < nb_msg; cpt_msg ++){
        tox_friend_send_message(my_tox, friendNumber(), t, msg.data(), msg.size());
    }
    */
    return false;
}

bool ToxFriend::init(){
    size_t len = 0, len_n=0;
    bool r=true, _r;
    TOX_ERR_FRIEND_QUERY error;
    TOX_ERR_FRIEND_GET_LAST_ONLINE err;
    uint8_t *message = 0;
    len = tox_friend_get_status_message_size (my_tox, my_friend_number, &error);
    if(error == TOX_ERR_FRIEND_QUERY_OK && len > 0){
        message = new uint8_t[len];
        _r = tox_friend_get_status_message (my_tox, my_friend_number, message, &error);
        if(_r && error == TOX_ERR_FRIEND_QUERY_OK){
            status = QString::fromUtf8((const char*)message, len);
#ifdef DEBUG
            qDebug() << "Status = " << status << ", Len = " << len;
#endif
        }else{
            r = false;
#ifdef DEBUG
            qDebug()<< "Error " << error;
#endif
        }
    }else{
#ifdef DEBUG
        qDebug() << "Error " << error << "Len = " << len;
#endif
        r= false;
    }
    len_n = tox_friend_get_name_size (my_tox, my_friend_number, &error);
    if(error == TOX_ERR_FRIEND_QUERY_OK && len_n > 0){
        if(len_n > len){
            if (message != 0) delete [] message;
            message = new uint8_t[len_n];
        }
        _r = tox_friend_get_name (my_tox, my_friend_number, message, &error);
        if(_r && error == TOX_ERR_FRIEND_QUERY_OK){
            name = QString::fromUtf8((const char*)message, len_n);
#ifdef DEBUG
            qDebug() << "Name = " << name << ", Len_n = " << len_n;
#endif
        }else{
            r = false;
#ifdef DEBUG
            qDebug() << "Error = " << error;
#endif
        }
        delete [] message;
    }else{
#ifdef DEBUG
        qDebug() << "Error = " << error << ", len_n = " << len_n;
#endif
        r= false;
    }
    lastOnline(tox_friend_get_last_online(my_tox, my_friend_number, &err));
    if(err != TOX_ERR_FRIEND_GET_LAST_ONLINE_OK) r = false;
    connect_status = getStatus();
    return r;
}
