#ifndef QMONGODB_H
#define QMONGODB_H


#include <QtCore/qglobal.h>


#include <QString>
#include <QMap>
#include <QVariantMap>
#include "qbson.h"
#include <QObject>
#include <QtCore>
#include <QMimeDatabase>


//#ifdef Q_OS_WIN // Q_OS_IOS
#ifdef Q_OS_IOS // Q_OS_WIN
#define MAC_IOS
#endif

#ifdef Q_OS_MACOS
#define MAC_IOS
#endif

#ifdef Q_OS_TVOS
#define MAC_IOS
#endif

#ifdef Q_OS_WATCHOS
#define MAC_IOS
#endif





#ifdef MAC_IOS
class Socket;
#else

#endif




class QMONGODBSHARED_EXPORT QMongoDB : public QObject
{
    Q_OBJECT
public:

    ///
    /// \brief QMongoDB
    /// \param mongourlorswitchip
    /// \param database
    /// \param parent
    /// if Platform Android or Windows you should pass mongodb url else Switch ip
    /// Mongodb url/Switch ip format: mongodb://username:passworkd@ipaddress:port/?authSource=DATABASE
    QMongoDB( QString mongourl , QString database , QObject* parent = nullptr );

    ~QMongoDB();


    ///
    /// \brief isValid
    /// \return
    /// only available on MACOSX or iOS
    /// do not use another platform
    bool isValid();



    ///
    /// \brief instance
    /// Must be Called if Platform is Android or Windows else Nothing to be done
    static void instance();



    QVector<QBSON> find(QString collection , QBSON filter , QOption option = QOption() );
    QVector<QBSON> find(std::string collection , QBSON filter , QOption option = QOption() );
    QVector<QBSON> find(const char* collection , QBSON filter , QOption option = QOption() );

    QBSON find_one( QString collection , QBSON filter , QOption option = QOption() );
    QBSON find_one( std::string collection , QBSON filter , QOption option = QOption() );
    QBSON find_one( const char* collection , QBSON filter , QOption option = QOption() );

    bool insert_one(QString collection , QBSON document );
    bool insert_one(std::string collection , QBSON document );
    bool insert_one(const char* collection , QBSON document );

    bool update_one(QString collection , QBSON filter , QBSON updateDocument );
    bool update_one(std::string collection , QBSON filter , QBSON updateDocument );
    bool update_one(const char* collection , QBSON filter , QBSON updateDocument );

    bool Delete(QString collection , QBSON filter);
    bool Delete(std::string collection , QBSON filter);
    bool Delete(const char* collection , QBSON filter);

    int64_t count( const QString& collection , QBSON filter );


    QElement uploadfile( QString filename , QString key );

    QString downloadfile(QOid fileoid , bool fileNametoOid = true );

    ///
    /// \brief getfilesize if QElement type is not oid throw error
    /// \param fileoid
    /// \return
    ///
    qlonglong getfilesize(QOid fileoid);

    QString getfilename(QOid fileoid);


    QString getLastError() const;


    ///
    /// \brief getUrl getMongoDB url
    /// \return
    ///
    QString getUrl() const;

    ///
    /// \brief getDb get DataBase Name
    /// \return
    ///
    QString getDb() const;

Q_SIGNALS:
    void gridfsbytereceived(int);

private:

    QString mUrl;
    QString db;
    QString mLastError;



#ifdef MAC_IOS
    Socket* mSocket;
#endif



};

#endif // QMONGODB_H
