#ifndef VIDEOPROBE_H
#define VIDEOPROBE_H
#include "../config.h"
#include <QVideoProbe>
#include <QDebug>
#include <QMediaPlayer>

//http://www.qtcentre.org/threads/57981-Using-QVideoProbe-from-QML

class VideoProbe : public QVideoProbe
{
    Q_OBJECT
    Q_PROPERTY(QObject* source READ getSource WRITE setSource)
public:
    explicit VideoProbe(QObject *parent = 0);
    static VideoProbe *getInstance(QObject * parent);
signals:

public slots:
    void setSource(QObject* sourceObj);
    QObject * getSource();

private:
    static VideoProbe * instance;
    QObject * ma_source;

};

#endif // VIDEOPROBE_H
