#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include "qbson.h"
#include <chrono>
#include <thread>

enum class NetworkCommand : int
{
    connectInfo = 0,
    find,
    find_one,
    update_many,
    update_one,
    insert_many,
    insert_one,
    delete_many,
    delete_one
};


class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket( QString mongourl , QObject *parent = nullptr );
    virtual ~Socket();



    void send( QString collection , NetworkCommand command , QBSON filter , QOption option = QOption() );

    void sendupdateone( QString collection , QBSON filter , QBSON updateDoc );


    bool validConnection() const;

    void ValidateDatabase();

    int byteavailable() const;

    bool bufferCompleted(QDataStream &stream);

signals:

public slots:
    void connect();


private:
    void connected();
    void disconnected();

    int mByteavailable;

private slots:
    void readyRead();


private:



private:
    QString mUrl;
    QString username;
    QString password;
    QString ipadress;
    QString port;
    QString database;
    QTcpSocket* mSocket;


    bool mValidConnection;
    void ValidConnection(QDataStream& out);

};

#endif // SOCKET_H
