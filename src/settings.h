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
#include <QPixmap>
#include <tox/tox.h>
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
    QString & getSettingsDirPath();
    QByteArray getAvatarHash(const QString& ownerId);
    void saveAvatar(QPixmap& pic, const QString& ownerId);
    static Settings * getInstance(QObject * parent);
private:
    explicit Settings(QObject *parent = 0);
    QList<DhtServer> server_list;
    QString avatar_path;
    static Settings* instance;
signals:

public slots:

};

#endif // SETTINGS_H
