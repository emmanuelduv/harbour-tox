#include "toxvideosurface.h"
QList<QVideoFrame::PixelFormat> ToxVideoSurface::l;

ToxVideoSurface::ToxVideoSurface(QObject *parent) :
    QAbstractVideoSurface(parent),presentation_surface(NULL)
{
#ifdef DEBUG
    qDebug() << "Creating ToxVideoSurface, parent = " << parent;
#endif
    if(l.empty()){
        l<< QVideoFrame::Format_RGB24 << QVideoFrame::Format_AYUV444 << QVideoFrame::Format_CameraRaw << QVideoFrame::Format_YUV444;
        l << QVideoFrame::Format_Jpeg << QVideoFrame::Format_YV12 << QVideoFrame::Format_Y16;
    }
}

QList<QVideoFrame::PixelFormat> ToxVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const{
#ifdef DEBUG
    qDebug() << "ToxVideoSurface::supportedPixelFormats() -> "<< l;
#endif
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
    if(nb_frames % 10 ==  0){
        qDebug() << "Frame " << nb_frames << " presented! " << frame.width() << " x " << frame.height();
        qDebug() << "FieldType = " << frame.fieldType();
        qDebug() << "PixelFormat = " << frame.pixelFormat();
    }
    nb_frames++;
#endif
    bool ret = true;
    if(presentation_surface != NULL){
        ret = presentation_surface->present(frame);
#ifdef DEBUG
        qDebug() << "Frame presented to " << presentation_surface;
#endif
    }
#ifdef DEBUG
    else{
        qDebug() << "Frame not presented to " << presentation_surface;
    }
#endif

    return ret;
}
