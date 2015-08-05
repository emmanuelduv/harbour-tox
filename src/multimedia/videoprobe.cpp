//http://www.qtcentre.org/threads/57981-Using-QVideoProbe-from-QML
//http://www.omg-it.works/how-to-grab-video-frames-directly-from-qcamera/ ??
#include "videoprobe.h"
VideoProbe * VideoProbe::instance;

VideoProbe::VideoProbe(QObject *parent) :
    QVideoProbe(parent),ma_source(NULL)
{
    if(VideoProbe::instance == NULL) VideoProbe::instance = this;
    else throw QString("VideoProbe must be instanciated one time!");
}

bool VideoProbe::setSource(QObject* sourceObj)
{
    bool ret =false;
    QMediaPlayer *player = qvariant_cast<QMediaPlayer*>(sourceObj->property("mediaObject"));
#ifdef DEBUG
    qDebug() << "VideoProbe::setSource() player"<< player;
#endif
    ma_source = player;
    ret = QVideoProbe::setSource(player);
#ifdef DEBUG
    if(ret == false) qDebug() << "QVideoProbe::setSource -> FALSE";
    else qDebug() << "QVideoProbe::setSource -> TRUE";
#endif
    return ret;
}

VideoProbe * VideoProbe::getInstance(QObject * parent){
    if(VideoProbe::instance == NULL) VideoProbe::instance = new VideoProbe(parent);
    return VideoProbe::instance;
}

QObject * VideoProbe::getSource(){
    return ma_source;
}
