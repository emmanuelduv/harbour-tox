#ifndef TOXVIDEOSURFACE_H
#define TOXVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QList>

class ToxVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit ToxVideoSurface(QObject *parent = 0);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    bool present(const QVideoFrame &frame);
    QAbstractVideoSurface * getPresentationSurface();
    void setPresentationSurface(QAbstractVideoSurface * s);
signals:

public slots:

};

#endif // TOXVIDEOSURFACE_H
