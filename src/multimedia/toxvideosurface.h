#ifndef TOXVIDEOSURFACE_H
#define TOXVIDEOSURFACE_H
#include "../config.h"
#include <QAbstractVideoSurface>
#include <QList>
#include <QMediaPlayer>

class ToxVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface * videoSurface READ getPresentationSurface WRITE setPresentationSurface)
    Q_PROPERTY(QObject * source READ getSource WRITE setSource)
public:
    explicit ToxVideoSurface(QObject *parent = 0);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    bool present(const QVideoFrame &frame);
    QAbstractVideoSurface * getPresentationSurface();
    void setPresentationSurface(QAbstractVideoSurface * s);
    void setSource(QObject * sourceObj);
    QObject * getSource();
signals:

public slots:
private:
    QAbstractVideoSurface * presentation_surface;
    QMediaPlayer * ma_source;
    int nb_frames;
    static QList<QVideoFrame::PixelFormat> l;
};

#endif // TOXVIDEOSURFACE_H
