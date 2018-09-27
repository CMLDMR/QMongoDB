#ifndef QMLMONGODB_H
#define QMLMONGODB_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QVector>
#include "qmongodb.h"

#include "qmlbson.h"
#include "qmlelement.h"
#include "qmlarray.h"


#define MAJOR   0
#define MINOR   7

class QMONGODBSHARED_EXPORT QMLMongoDB : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid )
public:
    explicit QMLMongoDB();
    QMLMongoDB(const QMLMongoDB& db):QObject (db.parent()) {}
    QMLMongoDB& operator=(const QMLMongoDB& db);
    virtual ~QMLMongoDB();


    ///
    /// \brief isValid Q_INVOKABLE
    /// \return
    /// Check Started DB
    bool isValid() const;


    ///
    /// \brief start
    /// \param mUrl
    /// \param database
    /// Start Database. Must be Call before execute operation
    Q_INVOKABLE void start( const QString& mUrl , const QString& database );



    ///
    /// \brief find
    /// \param collection
    /// \param filter
    /// \param option
    /// \return
    /// Return list of document that matched with "filter"
    Q_INVOKABLE QVariantList find(const QString& collection , QMLBSON *filter , QMLBSON *option );


    ///
    /// \brief find_one
    /// \param collection
    /// \param filter
    /// \param option
    /// \return
    /// Return First Document that matched with "filter"
    Q_INVOKABLE QMLBSON* find_one( const QString& collection , QMLBSON* filter , QMLBSON* option );


    ///
    /// \brief insert_one
    /// \param collection
    /// \param bson
    /// \return
    /// Insert "bson" Document to Collection "collection"
    Q_INVOKABLE bool insert_one( const QString& collection , QMLBSON* bson );


    ///
    /// \brief update_one
    /// \param collection
    /// \param filter
    /// \param updatebson
    /// \return
    /// Update Document with "updatebson" values by matched with "filter"
    Q_INVOKABLE bool update_one( const QString& collection , QMLBSON* filter , QMLBSON* updatebson );


    ///
    /// \brief delete_one
    /// \param collection
    /// \param filter
    /// \return
    /// Delete Document that matched with "filter"
    Q_INVOKABLE bool delete_one( const QString& collection , QMLBSON* filter );



    ///
    /// \brief fileurl
    /// \param oid
    /// \param fileNametoOid
    /// \return
    /// Download GridFS file by given file oid and return file Url type QString
    Q_INVOKABLE QString fileurl( const QString& oid , bool fileNametoOid = true );



    ///
    /// \brief uploadfile
    /// \param filename
    /// \param key
    /// \return
    /// Upload file and return information about file oid in QMLElement
    Q_INVOKABLE QMLElement* uploadfile( const QString& filename , QString key );



    ///
    /// \brief getDb
    /// \return
    /// Get C++ based db variable
    QMongoDB *getDb() const;





    ///
    /// \brief instance
    /// \param url
    /// \param dbName
    /// Call This Function before using QMongoDB Operation
    static void instance(const QString& url , const QString dbName );


private:
    QMongoDB* db;
    bool mStarted;



};

Q_DECLARE_METATYPE(QMLMongoDB);

#endif // QMLMONGODB_H
