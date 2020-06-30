//#pragma execution_character_set("utf-8")
#ifndef LOGER_H
#define LOGER_H

#include <QObject>
#include <QFile>
#include <QThread>
#include "LogWorker.h"

class QFile;

#define EOL         "\n"

class Loger : public QObject
{
    Q_OBJECT
private:
    explicit Loger(QObject *parent = nullptr);
public:
    static Loger *GetInstance();
public:
    Loger &operator << (const char *data);
    Loger &operator << (const QByteArray &data);
    Loger &operator<<(const QString &data);
    Loger &operator << (const qint32 &data);
    Loger &operator << (const quint32 &data);
    Loger &operator << (const qint64 &data);
    Loger &operator << (const quint64 &data);
    Loger &operator << (const qreal &data);
signals:
    void sigReceived(const QString &str);
    void sigWriteLog(const QString &fileName, const QString &message);
public slots:
    void slotReceived(const QString &str);
private:
    void initFile();
private:
    static Loger *_gLoger;
    QString m_strFileName;

    QThread m_nThread;
    LogWorker m_nWorker;

    QString m_strDate;
};

#define LOGER   (*Loger::GetInstance())

#endif // LOGER_H
