#ifndef TOXFRIEND_H
#define TOXFRIEND_H

#include <QObject>
#include <tox/tox.h>
#include <QList>
#include <QHash>
#include <stdint.h>
#include "cstring.h"
#define DEBUG 1
#ifdef DEBUG
 #include <QtDebug>
#endif
class ToxFriend : public QObject
{
    Q_OBJECT
private:
    Tox*  my_tox;
    uint32_t my_friend_number;
    QString& name, status;
    QList<QString> arrived;
    QHash<uint32_t, QString> sent;
    TOX_USER_STATUS connect_status;
    bool friend_typing;
    uint64_t last_online;

public:
    explicit ToxFriend(QObject *parent = 0, int friend_number=0, Tox * tox=0);
    Q_INVOKABLE QString getStatusMessage();
    Q_INVOKABLE QString getName();
    Q_INVOKABLE TOX_USER_STATUS getStatus();
    Q_INVOKABLE uint32_t friendNumber();
    Q_INVOKABLE void nameChanged(const uint8_t *newname, size_t length, void *userdata);
    Q_INVOKABLE QString toxId();
    QString & messageArrived(TOX_MESSAGE_TYPE type, const uint8_t * message, size_t length, void *userdata);
    void statusChanged(const uint8_t *newstatus, uint16_t length, void *userdata);
    void statusMessageChanged(const uint8_t *newstatus, uint16_t length, void *userdata);
    void statusChanged(TOX_USER_STATUS status, void *userdata);
    void typingChanged(bool is_typing, void *userdata);
    bool typing();
    bool init();
    Q_INVOKABLE uint64_t lastOnline();
    Q_INVOKABLE void lastOnline(uint64_t timestamp);
    Q_INVOKABLE QList<QString> receivedMessages();
    Q_INVOKABLE QList<QString> sentMessages();
    Q_INVOKABLE bool sendMessage(const QString & message, TOX_MESSAGE_TYPE t, int *id_msg);
signals:

public slots:

};

#endif // TOXFRIEND_H
