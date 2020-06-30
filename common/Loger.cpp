//#pragma execution_character_set("utf-8")
#include "Loger.h"
#include <QDateTime>
#include <QDir>

Loger *Loger::_gLoger = NULL;

inline QString getDateTimeString()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString strTime = dateTime.toString("yyyy-MM-dd hh:mm::ss ");
    return strTime;
}

Loger::Loger(QObject *parent) : QObject(parent)
{
    m_strDate = getDateTimeString();
    m_nWorker.moveToThread(&m_nThread);
    connect(this, SIGNAL(sigWriteLog(const QString &, const QString &)),
            &m_nWorker, SLOT(slotWriteLog(const QString&, const QString &)));
    m_nThread.start();
    connect(this, SIGNAL(sigReceived(const QString &)), this, SLOT(slotReceived(const QString &)));
    initFile();
}

Loger *Loger::GetInstance()
{
    if(Loger::_gLoger == NULL)
    {
        Loger::_gLoger = new Loger();
    }
    return Loger::_gLoger;
}

void Loger::initFile()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString fileName = dateTime.toString("yyyy-MM-dd");
    fileName += ".log";

    QString dirPath = QDir::currentPath() + "/log";
    QDir dir;
    if(!dir.exists(dirPath)){
        dir.mkpath(dirPath);
    }

    QString filePath =  dirPath + "/" + fileName;
    m_strFileName = filePath;
}

Loger &Loger::operator << (const char *data)
{
    QString str(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const QByteArray &data)
{
    QString str(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const QString &data)
{
    emit sigReceived(data);
    return *this;
}

Loger &Loger::operator << (const qint32 &data)
{
    QString str = QString::number(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const quint32 &data)
{
    QString str = QString::number(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const qint64 &data)
{
    QString str = QString::number(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const quint64 &data)
{
    QString str = QString::number(data);
    emit sigReceived(str);
    return *this;
}

Loger &Loger::operator << (const qreal &data)
{
    QString str = QString::number(data);
    emit sigReceived(str);
    return *this;
}

void Loger::slotReceived(const QString &str)
{
    if(str.compare("\n") == 0){
        emit sigWriteLog(m_strFileName, str);
        m_strDate = getDateTimeString();
    }else{
        QString strMessage = str;
        if(m_strDate.length() > 0){
            strMessage = m_strDate + str;
        }
        emit sigWriteLog(m_strFileName, strMessage);
        m_strDate = "";
    }
}
