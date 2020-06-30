//#pragma execution_character_set("utf-8")
#include "LogWorker.h"
#include <QFile>
#include <QDebug>

LogWorker::LogWorker(QObject *parent) : QObject(parent)
{

}

void LogWorker::slotWriteLog(const QString &fileName, const QString &message)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QString strMessage = "";
    file.write(message.toUtf8());
    file.close();
    qDebug() << message;
}
