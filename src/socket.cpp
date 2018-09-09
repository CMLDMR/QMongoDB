#include "socket.h"

#include <QBuffer>
#include <QDataStream>

Socket::Socket( QString mongourl , QObject *parent )
    : QObject( parent ),
      mUrl( mongourl )
{

    mByteavailable = -1;
    mValidConnection = false;


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




}

Socket::~Socket()
{
    delete mSocket;
}

void Socket::connect()
{
    qDebug() <<"Connecting...";

    mSocket = new QTcpSocket;

    QObject::connect(mSocket,&QTcpSocket::connected,this,&Socket::connected);
    QObject::connect(mSocket,&QTcpSocket::disconnected,this,&Socket::disconnected);
//    QObject::connect(mSocket,&QTcpSocket::readyRead,this,&Socket::readyRead);


    mSocket->connectToHost(ipadress,port.toInt());


}



void Socket::ValidateDatabase()
{
    QBSON bson;

    bson.append("username",this->username);
    bson.append("password",this->password);
    bson.append("db",this->database);

    QBuffer buffer;

    buffer.open(QIODevice::ReadWrite);

    QDataStream out(&buffer);
    out.setVersion(QDataStream::Version::Qt_5_10);

    out << int(0);
    out << static_cast<int>(NetworkCommand::connectInfo);
    out << bson;

    out.device()->seek(0);
    out << buffer.data().size();

    buffer.close();
    qDebug() <<"validate write: " << mSocket->write(buffer.data());

    if( mSocket->waitForReadyRead() )
    {
        qDebug() << "Byte Available";
    }

}

bool Socket::validConnection() const
{
    return mValidConnection;
}

void Socket::ValidConnection(QDataStream &out)
{
    bool valid;
    out >> valid;
    this->mValidConnection = valid;
    qDebug() << "Connection Valid: " << this->mValidConnection;
}

void Socket::send(QString collection, NetworkCommand command, QBSON filter, QOption option)
{
    QBuffer buffer;

    buffer.open(QIODevice::ReadWrite);

    QDataStream out(&buffer);
    out.setVersion(QDataStream::Version::Qt_5_10);

    out << int(0);
    out << static_cast<int>(command);
    out << collection;
    out << filter;
    out << option.getBson();

    out.device()->seek(0);

    out << buffer.data().size();

    buffer.close();
    qDebug() <<"Written: " <<mSocket->write(buffer.data());

    if( mSocket->waitForReadyRead() )
    {
        qDebug() << "Byte Available";
    }
}

void Socket::sendupdateone(QString collection, QBSON filter, QBSON updateDoc)
{
    QBuffer buffer;

    buffer.open(QIODevice::ReadWrite);

    QDataStream out(&buffer);
    out.setVersion(QDataStream::Version::Qt_5_10);

    out << int(0);
    out << static_cast<int>(NetworkCommand::update_one);
    out << collection;
    out << filter;
    out << updateDoc;

    out.device()->seek(0);

    out << buffer.data().size();

    buffer.close();
    qDebug() <<"Written: " << mSocket->write(buffer.data());

    if( mSocket->waitForReadyRead() )
    {
        qDebug() << "Byte Available";
    }
}

void Socket::connected()
{
    qDebug() << "Connected Socket, Validating";
//    this->ValidateDatabase();
}

void Socket::disconnected()
{
    qDebug() << "Disconnected Socket";
}

int Socket::byteavailable() const
{
    return mByteavailable;
}

void Socket::readyRead()
{

    qDebug() <<"Ready Read";

    QDataStream out(mSocket);
    out.setVersion(QDataStream::Qt_5_11);


    if ( mSocket->bytesAvailable() && mByteavailable == -1 ){
        out >> mByteavailable;
        qDebug() << "Bytes Waiting: " << mByteavailable;
    }


    if(mByteavailable - mSocket->bytesAvailable()- static_cast<int>(sizeof(int)) != 0){

      return;
    }


    int commandType;
        out >> commandType;
        NetworkCommand command = static_cast<NetworkCommand>(commandType);


        switch (command) {

        case NetworkCommand::connectInfo:
            qDebug() << "Connection info";
            this->ValidConnection(out);
            break;

        case NetworkCommand::find:
            qDebug() << "find";
            break;

        case NetworkCommand::find_one:
            qDebug() << "find one";
            break;

        case NetworkCommand::update_many:
            qDebug() << "update many";
            break;

        case NetworkCommand::update_one:
            qDebug() << "update one";
            break;

        case NetworkCommand::insert_many:
            qDebug() << "insert many";
            break;

        case NetworkCommand::insert_one:
            qDebug() << "insert one";
            break;

        case NetworkCommand::delete_many:
            qDebug() << "Delete Many";
            break;

        case NetworkCommand::delete_one:
            qDebug() << "Delete One";
            break;

        default:
            qDebug() << "unknow Command";
            break;
        }
        mByteavailable = -1;
}

bool Socket::bufferCompleted( QDataStream &stream )
{
    qDebug() <<"Ready Read";

    QDataStream out(mSocket);
    stream.setDevice(mSocket);
    stream.setVersion(QDataStream::Qt_5_11);


    if ( mSocket->bytesAvailable() && mByteavailable == -1 ){
        stream >> mByteavailable;
        qDebug() << "Bytes Waiting: " << mByteavailable;
    }


    if(mByteavailable - mSocket->bytesAvailable()- static_cast<int>(sizeof(int)) != 0){
        return false;
    }else{
        mByteavailable = -1;
        return true;
    }
}



















