#ifndef TOXWRAPPER_H
#define TOXWRAPPER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QList>
#include <QHash>
#include <QFile>
#include <tox/tox.h>
#include <stdint.h>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#define DEBUG 1
#include "toxfriend.h"
#include "cstring.h"
#include "cdata.h"
#include "settings.h"

#ifdef DEBUG
 #include <QtDebug>
#endif
#ifndef UINT64_MAX
 # define UINT64_MAX		(__UINT64_C(184467443073709551615))
#endif
#ifndef TOX_BOOTSRAP_NB_SRV
 #define TOX_BOOTSRAP_NB_SRV 2
#endif
class toxWrapper : public QThread
{
    Q_OBJECT
private:
    Tox *my_tox;
    QList<ToxFriend*> contacts;
    QHash<uint32_t, ToxFriend*> v_contacts;
    bool initContacts();
    static QHash<Tox*, toxWrapper*> toxs;
    QHash<QString, QString> friend_requests;
    Tox_Options toxOptions;
    QString self_address;
    char * save_data;
    qint64 save_data_size;
    QString save_data_filename;
    TOX_CONNECTION c;
    bool to_quit;

public:
    explicit toxWrapper(QObject *parent=NULL);
    /*explicit toxWrapper(QObject *parent = 0);*/
    uint32_t addContact(const uint8_t *address, const QString & message, bool send_message);
    Q_INVOKABLE uint32_t addContact(const QString & address, const QString & message, bool send_message);
    Q_INVOKABLE bool setName(const QString& name);
    Q_INVOKABLE QString getAddress();
    Q_INVOKABLE int count();
    Q_INVOKABLE ToxFriend* getContact(int index);
    Q_INVOKABLE QList<ToxFriend *> & getContacts();
    Q_INVOKABLE bool isTyping(uint32_t friendId);
    Q_INVOKABLE uint64_t getLastOnline(uint32_t friendId);
    Q_INVOKABLE QString getSelfStatusMessage();
    Q_INVOKABLE bool setStatusMessage(const QString &message);
    Q_INVOKABLE TOX_USER_STATUS getSelfStatus();
    Q_INVOKABLE QString getStatusMessage(uint32_t friendId);
    Q_INVOKABLE int getStatus(uint32_t friendId);
    Q_INVOKABLE TOX_CONNECTION connected();
    //void friendMessageArrived(Tox *tox, int32_t friendnumber, TOX_MESSAGE_TYPE type, const uint8_t * message, size_t length, void *userdata);
    static void friendMessageArrived(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data);
    void friendMessageArrived(uint32_t friendnumber, TOX_MESSAGE_TYPE type, const uint8_t * message, size_t length, void *userdata);
    //void friendRequestArrived(Tox *tox, const uint8_t * public_key, const uint8_t * data, uint16_t length, void *userdata);
    //Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data
    static void friendRequestArrived(Tox* tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data);
    void friendRequestArrived(const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data);
    //(Tox *tox, uint32_t friend_number, const uint8_t *name, size_t length, void *user_data)
    static void nameChanged(Tox *tox, uint32_t friendnumber, const uint8_t *newname, size_t length, void *userdata);
    void nameChanged(uint32_t friendnumber, const uint8_t *newname, size_t length, void *userdata);
    static void statusMessageChanged(Tox *tox, int32_t friendnumber, const uint8_t *newstatus, uint16_t length, void *userdata);
    void statusMessageChanged(int32_t friendnumber, const uint8_t *newstatus, uint16_t length, void *userdata);
    static void typingChanged(Tox *tox, uint32_t friendnumber, bool is_typing, void *userdata);
    void typingChanged(int32_t friendnumber, bool is_typing, void *userdata);
    static void statusChanged(Tox *tox, uint32_t friendnumber, TOX_USER_STATUS status, void *userdata);
    void statusChanged(uint32_t friendnumber, TOX_USER_STATUS status, void *userdata);
    Q_INVOKABLE int removeContact(int friendNumber);
    Q_INVOKABLE bool typingTo(uint32_t friend_number, bool typing);
    Q_INVOKABLE bool sendMessage(uint32_t friend_number, const QString & message);
    bool sendMessage(uint32_t friend_number, const QString & message, TOX_MESSAGE_TYPE t);
    Q_INVOKABLE bool save(QString * file);
    Q_INVOKABLE bool load(const QString & file);
    Q_INVOKABLE bool load();
    Q_INVOKABLE bool init();
    ~toxWrapper();
    static void connected(Tox *tox, TOX_CONNECTION connection_status, void *user_data);
    void connected(TOX_CONNECTION connection_status, void *user_data);
    void run();
    Q_INVOKABLE void toxQuit(bool _quit, bool wait);
    Q_INVOKABLE void setSavePath(const QString & save);
    Q_INVOKABLE QString & getSavePath();
/*    Q_INVOKABLE uint64_t getLastOnline();*/
/*    Q_INVOKABLE QList getFriendList();*/

signals:
    Q_INVOKABLE void friendMessageReceived(QVariant friendnumber, QVariant message, QVariant type);
    Q_INVOKABLE void messagesToUpdate();

public slots:
    void quitter();
    void updateMessages();
    void friendMessageProcess(QVariant friendNumber, QVariant message, QVariant type);
};

#endif // TOXWRAPPER_H
