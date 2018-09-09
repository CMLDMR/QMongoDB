#include "socket.h"

Socket::Socket( QString mongourl , QObject *parent )
    : QObject( parent ),
      mUrl( mongourl )
{



    //Mongo Prefix
    {
        QString mongo = mUrl.mid(0,10); mUrl.remove(0,10);
    }

    //Username
    {
        int index = mUrl.indexOf(":",0);
        username = mUrl.mid(0,index); mUrl.remove(0,index+1);
    }

    //Pasword
    {
        int index = mUrl.indexOf("@",0);
        password = mUrl.mid(0,index); mUrl.remove(0,index+1);
    }

    //Ip
    {
        int index = mUrl.indexOf(":",0);
        ipadress = mUrl.mid(0,index); mUrl.remove(0,index+1);
    }

    //Port
    {
        int index = mUrl.indexOf("/",0);
        port = mUrl.mid(0,index); mUrl.remove(0,index+1);
    }

    //blank
    {
        int index = mUrl.indexOf("=",0);
        QString blank = mUrl.mid(0,index); mUrl.remove(0,index+1);
    }

    //database
    {
        database = mUrl;
    }


    mSocket = new QTcpSocket;

    QObject::connect(mSocket,&QTcpSocket::connected,this,&Socket::connected);
    QObject::connect(mSocket,&QTcpSocket::disconnected,this,&Socket::disconnected);

}

Socket::~Socket()
{
    delete mSocket;
}

void Socket::connect()
{
    mSocket->connectToHost(ipadress,port.toInt());
}

void Socket::connected()
{
    qDebug() << "Connected Socket";
}

void Socket::disconnected()
{
    qDebug() << "Disconnected Socket";
}
