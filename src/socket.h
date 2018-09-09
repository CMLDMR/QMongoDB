#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QTcpSocket>
#include <QDebug>

class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QString mongourl , QObject *parent = nullptr);
    virtual ~Socket();

    void connect();

signals:

public slots:


private:
    void connected();
    void disconnected();


private:
    QString mUrl;
    QString username;
    QString password;
    QString ipadress;
    QString port;
    QString database;
    QTcpSocket* mSocket;
};

#endif // SOCKET_H
