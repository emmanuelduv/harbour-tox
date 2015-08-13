#ifndef TOXFILE_H
#define TOXFILE_H

#include <QString>
#include <QFile>
#include "stdint.h"
#include "tox/tox.h"
#include "config.h"

struct ToxFile
{
    enum FileStatus
    {
        STOPPED,
        PAUSED,
        TRANSMITTING,
        BROKEN
    };

    enum FileDirection : bool
    {
        SENDING,
        RECEIVING
    };

    ToxFile()=default;
    ToxFile(uint32_t FileNum, uint32_t FriendId, QByteArray FileName, QString FilePath, FileDirection Direction);
    ~ToxFile(){
        if(file != NULL) delete file;
    }

    bool operator==(const ToxFile& other) const;
    bool operator!=(const ToxFile& other) const;

    void setFilePath(QString path);
    bool open(bool write);

    uint8_t fileKind; ///< Data file (default) or avatar
    uint32_t fileNum;
    uint32_t friendId;
    QByteArray fileName;
    QString filePath;
    QFile * file;
    quint64 bytesSent;
    quint64 filesize;
    FileStatus status;
    FileDirection direction;
    QByteArray avatarData;
    QByteArray resumeFileId;
};

#endif // TOXFILE_H
