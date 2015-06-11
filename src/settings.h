#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QList>
#include <QStandardPaths>
#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#ifndef DEBUG
 #define DEBUG
#endif
#ifdef DEBUG
 #include <QDebug>
#endif

class Settings : public QObject
{
    Q_OBJECT
public:
    QString filePath;
    struct DhtServer
   {
       QString name;
       QString userId;
       QString address;
       quint16 port;
   };
    QList<DhtServer> &getBootstrapServers();
    static Settings * getInstance(QObject * parent);
private:
    explicit Settings(QObject *parent = 0);
    QList<DhtServer> server_list;
    static Settings* instance;
signals:

public slots:

};

#endif // SETTINGS_H
