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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <sailfishapp.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QtQml>
#include <QDebug>
#include <sailfishapp/sailfishapp.h>
#include "toxwrapper.h"
#include "settings.h"
#include "multimedia/videoprobe.h"

QObject* videoprobe_singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    return VideoProbe::getInstance(NULL);
}

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    qmlRegisterType<toxWrapper> ("harbour.tox", 1, 0, "ToxWrapper");
    qmlRegisterType<ToxFriend> ("harbour.tox", 1, 0, "ToxFriend");
    qmlRegisterSingletonType<VideoProbe>("harbour.tox", 1, 0, "VideoProbe", videoprobe_singleton_provider);
    QGuiApplication * s_app = SailfishApp::application(argc, argv);
    s_app->setOrganizationName("harbour-tox");
    s_app->setApplicationName("harbour-tox");
    QScopedPointer<QGuiApplication> app(s_app);
    //toxWrapper * tox;
    int r;
     QScopedPointer<QQuickView> view(SailfishApp::createView());
     /*Add to QML*/
    // view->rootContext()->setContextProperty("tox", tox);
     view->setSource(SailfishApp::pathTo("qml/qTest.qml"));
     /*Tox creation*/

     /*QString save(".tox/save");
     tox = new toxWrapper(s_app, save, contacts, v_contacts);
*/
     view->showFullScreen();
     /*Pour initialiser le Tox*/
     //tox->start();
     r = app->exec();
     //tox->toxQuit(true);
     //delete tox;
     return r;
}

