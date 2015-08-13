#include "toxvideosurface.h"

ToxVideoSurface::ToxVideoSurface(QObject *parent) :
    QAbstractVideoSurface(parent)
{
#ifdef DEBUG
    qDebug() << "Créating ToxVideoSurface, parent = " << parent;
#endif
}

QList<QVideoFrame::PixelFormat> ToxVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const{
    QList<QVideoFrame::PixelFormat> l;
    l<< QVideoFrame::Format_RGB24 << QVideoFrame::Format_AYUV444 << QVideoFrame::Format_CameraRaw << QVideoFrame::Format_YUV444;
    l << QVideoFrame::Format_Jpeg << QVideoFrame::Format_YV12 << QVideoFrame::Format_Y16;
    return l;
}

void ToxVideoSurface::setPresentationSurface(QAbstractVideoSurface * s){
    presentation_surface = s;
}
QAbstractVideoSurface * ToxVideoSurface::getPresentationSurface(){
    return presentation_surface;
}

void ToxVideoSurface::setSource(QObject * sourceObj){
    bool ret =false;
    QMediaPlayer *player = qvariant_cast<QMediaPlayer*>(sourceObj->property("mediaObject"));
#ifdef DEBUG
    qDebug() << "ToxVideoSurface::setSource() player"<< player;
#endif
    ma_source = player;
    if(ma_source) ret = true;
#ifdef DEBUG
    if(ret == false) qDebug() << "ToxVideoSurface::setSource -> FALSE";
    else qDebug() << "ToxVideoSurface::setSource -> TRUE";
#endif
    //return ret;
}
QObject * ToxVideoSurface::getSource(){
    return ma_source;
}

bool ToxVideoSurface::present(const QVideoFrame& frame){
#ifdef DEBUG
    qDebug() << "Frame probed! " << frame.width() << " x " << frame.height();
    qDebug() << "FieldType = " << frame.fieldType();
    qDebug() << "PixelFormat = " << frame.pixelFormat();
#endif
    return true;
}
