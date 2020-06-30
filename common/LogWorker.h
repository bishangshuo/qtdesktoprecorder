//#pragma execution_character_set("utf-8")
#ifndef LOGWORKER_H
#define LOGWORKER_H

#include <QObject>

class LogWorker : public QObject
{
    Q_OBJECT
public:
    explicit LogWorker(QObject *parent = nullptr);

signals:
public slots:
    void slotWriteLog(const QString &fileName, const QString &message);
};

#endif // LOGWORKER_H
