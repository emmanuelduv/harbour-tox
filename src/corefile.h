#ifndef COREFILE_H
#define COREFILE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include "toxwrapper.h"
#include "tox/tox.h"

class toxWrapper;

class CoreFile : public QObject
{
    Q_OBJECT
    friend class toxWrapper;
private:
    CoreFile()=delete;

    // Internal file sending APIs, used by Core. Public API in core.h
private:
    static void sendFile(toxWrapper *core, uint32_t friendId, QString Filename, QString FilePath, long long filesize);
    static void sendAvatarFile(toxWrapper* core, uint32_t friendId, const QByteArray& data);
    static void pauseResumeFileSend(toxWrapper* core, uint32_t friendId, uint32_t fileId);
    static void pauseResumeFileRecv(toxWrapper* core, uint32_t friendId, uint32_t fileId);
    static void cancelFileSend(toxWrapper* core, uint32_t friendId, uint32_t fileId);
    static void cancelFileRecv(toxWrapper* core, uint32_t friendId, uint32_t fileId);
    static void rejectFileRecvRequest(toxWrapper* core, uint32_t friendId, uint32_t fileId);
    static void acceptFileRecvRequest(toxWrapper* core, uint32_t friendId, uint32_t fileId, QString path);
    static ToxFile *findFile(uint32_t friendId, uint32_t fileId);
    static void addFile(uint32_t friendId, uint32_t fileId, const ToxFile& file);
    static void removeFile(uint32_t friendId, uint32_t fileId);
    /// Returns the maximum amount of time in ms that Core should wait between two
    /// tox_iterate calls to get good file transfer performances
    static unsigned corefileIterationInterval();

private:
    static void onFileReceiveCallback(Tox*, uint32_t friendnumber, uint32_t fileId, uint32_t kind,
                                      uint64_t filesize, const uint8_t *fname, size_t fnameLen, void *core);
    static void onFileControlCallback(Tox *tox, uint32_t friendId, uint32_t fileId,
                                      TOX_FILE_CONTROL control, void *core);
    static void onFileDataCallback(Tox *tox, uint32_t friendId, uint32_t fileId,
                                   uint64_t pos, size_t length, void *core);
    static void onFileRecvChunkCallback(Tox *tox, uint32_t friendId, uint32_t fileId, uint64_t position,
                                        const uint8_t *data, size_t length, void *core);
    static void onConnectionStatusChanged(toxWrapper *core, uint32_t friendId, bool online);

private:
    static QMutex fileSendMutex;
    static QHash<uint64_t, ToxFile> fileMap;

};

#endif // COREFILE_H
