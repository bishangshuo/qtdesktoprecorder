#ifndef ENCODEAUDIO_H
#define ENCODEAUDIO_H

#include <QObject>

class EncodeAudio : public QObject
{
    Q_OBJECT
public:
    explicit EncodeAudio(QObject *parent = nullptr);

signals:

};

#endif // ENCODEAUDIO_H
