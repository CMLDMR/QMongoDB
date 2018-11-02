#include "qmongodb.h"



#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <assert.h>


#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>



#ifdef MAC_IOS
#include "src/socket.h"
#else
#include <mongoc.h>
#endif





#ifdef MAC_IOS

#else
    static mongoc_client_t* client;
    static mongoc_gridfs_t* gridfs;
    void _find(_mongoc_cursor_t* cursor , QVector<QBSON>* list);
    bson_t *convert(QBSON &obj);
    void convertArray(QArray &array , bson_t *child);
    void RecursiveDocument(bson_iter_t *iter , QBSON &obj_ );
    void RecursiveArray(bson_iter_t *iter , QArray &array_ );
#endif





QMongoDB::QMongoDB(QString mongourl , QString database , QObject *parent )
    :QObject ( parent ),
      mUrl( mongourl ),
      db( database )
{

#ifdef MAC_IOS

    mSocket = new Socket(mUrl);
    mSocket->connect();

#else

    client = mongoc_client_new (this->mUrl.toStdString().c_str());
#endif
}


QMongoDB::~QMongoDB()
{
#ifdef MAC_IOS
    delete mSocket;
#else
    mongoc_client_destroy (client);
#endif

}

bool QMongoDB::isValid()
{
#ifdef MAC_IOS
    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    this->mSocket->ValidateDatabase();

    while ( !returnValue ) {

        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) return false;

    if( returnValue )
    {
            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);


            switch (command) {

            case NetworkCommand::connectInfo:
                qDebug() << "Connection info";
                bool valid;
                out >> valid;
                return valid;
                break;

            default:
                qDebug() << "No Value" << commandType;
                return false;
                break;
            }
    }else{
        return false;
    }

#else
    return false;
#endif


}



void QMongoDB::instance()
{
#ifdef MAC_IOS

#else
    mongoc_init();
#endif

}




QVector<QBSON> QMongoDB::find(QString collection, QBSON filter, QOption option)
{

#ifdef MAC_IOS

    QVector<QBSON> list;

    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    mSocket->send( collection , NetworkCommand::find , filter , option );

    while ( !returnValue ) {

        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) return list;;

    if( returnValue )
    {

            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);

            if( command != NetworkCommand::find )
            {
                qDebug() << "invalid data";

                return list;
            }

            int bsonCount = 0;
            out >> bsonCount;
            for( int i = 0 ; i < bsonCount ; i++ )
            {
                QBSON bson;
                out >> bson;
                list.append(bson);
            }

            return list;
    }else{
        return list;
    }

#else

    QVector<QBSON> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    auto _filter = convert(filter);

    QBSON bson = option.getBson();

    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(cursor,&list);

    return list;

#endif

}

QVector<QBSON> QMongoDB::find(std::string collection, QBSON filter, QOption option)
{

#ifdef MAC_IOS

    return this->find(QString(collection.c_str()),filter,option);

#else
    QVector<QBSON> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    auto _filter = convert(filter);

    QBSON bson = option.getBson();
    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(cursor,&list);

    return list;
#endif


}

QVector<QBSON> QMongoDB::find(const char *collection, QBSON filter, QOption option)
{
#ifdef MAC_IOS
    return this->find(QString(collection),filter,option);
#else
    QVector<QBSON> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    auto _filter = convert(filter);

    QBSON bson = option.getBson();
    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(cursor,&list);


    return list;
#endif

}

QBSON QMongoDB::find_one(QString collection, QBSON filter, QOption option)
{

#ifdef MAC_IOS

    option.setLimit(1);

    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    mSocket->send( collection , NetworkCommand::find_one , filter , option );

    while ( !returnValue ) {

        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) QBSON();

    if( returnValue )
    {

            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);

            QBSON bson;

            if( command != NetworkCommand::find_one )
            {
                qDebug() << "invalid data";

                return bson;
            }

            int bsonCount = 0;
            out >> bsonCount;
            out >> bson;
            return bson;
    }else{
        return QBSON();
    }

#else
    option.setLimit(1);

    auto list = this->find(collection,filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBSON();
    }
#endif


}

QBSON QMongoDB::find_one(std::string collection, QBSON filter, QOption option)
{
#ifdef MAC_IOS
    return this->find_one(QString(collection.c_str()),filter,option);
#else
    option.setLimit(1);

    auto list = this->find(collection.c_str(),filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBSON();
    }
#endif

}

QBSON QMongoDB::find_one(const char *collection, QBSON filter, QOption option)
{
#ifdef MAC_IOS
    return this->find_one(QString(collection),filter,option);
#else
    option.setLimit(1);

    auto list = this->find(collection,filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBSON();
    }
#endif

}

bool QMongoDB::insert_one(QString collection, QBSON document)
{
#ifdef MAC_IOS
    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    qDebug() << "insert_one " << document.tojson().c_str();
    mSocket->send( collection , NetworkCommand::insert_one , document );


    while ( !returnValue ) {



        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) return false;;

    if( returnValue )
    {

            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);

            if( command != NetworkCommand::insert_one )
            {
                qDebug() << "invalid data";
                return false;
            }

            bool inserted = false;
            out >> inserted;
            return inserted;

    }else{
        return false;
    }
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _document = convert(document);

    if( !mongoc_collection_insert(col,mongoc_insert_flags_t::MONGOC_INSERT_NONE,_document,nullptr,&error) )
    {
        qDebug() << "ERROR insert_one: "<<error.message;
        return false;
    }else{
        return true;
    }
#endif

}

bool QMongoDB::insert_one(std::string collection, QBSON document)
{
#ifdef MAC_IOS
return this->insert_one(QString(collection.c_str()),document);
#else
return this->insert_one(QString(collection.c_str()),document);
#endif

}

bool QMongoDB::insert_one(const char *collection, QBSON document)
{
#ifdef MAC_IOS
    return this->insert_one(QString(collection),document);
#else
    return this->insert_one(QString(collection),document);
#endif

}

bool QMongoDB::update_one(QString collection, QBSON filter, QBSON updateDocument)
{
#ifdef MAC_IOS
    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    qDebug() << "insert_one " << updateDocument.tojson().c_str();
    mSocket->sendupdateone( collection , filter , updateDocument );

    while ( !returnValue ) {

        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) return false;;

    if( returnValue )
    {

            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);

            if( command != NetworkCommand::update_one )
            {
                qDebug() << "invalid data";
                return false;
            }

            bool updated = false;
            out >> updated;
            return updated;

    }else{
        return false;
    }
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update(col,mongoc_update_flags_t::MONGOC_UPDATE_UPSERT,_filter,_updateDocument,nullptr,&error) )
    {
        QString err;
        for( int i = 0 ; i < 504 ; i++ )
        {
            if( error.message[i] )
            {
                err += error.message[i];
            }else{
                break;
            }
        }
        mLastError = err;
        return false;
    }else{
        return true;
    }
#endif

}

bool QMongoDB::update_one(std::string collection, QBSON filter, QBSON updateDocument)
{
#ifdef MAC_IOS
    return this->update_one(QString(collection.c_str()),filter,updateDocument);
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update(col,mongoc_update_flags_t::MONGOC_UPDATE_UPSERT,_filter,_updateDocument,nullptr,&error) )
    {
        QString err;
        for( int i = 0 ; i < 504 ; i++ )
        {
            if( error.message[i] )
            {
                err += error.message[i];
            }else{
                break;
            }
        }
        mLastError = err;;
        return false;
    }else{
        return true;
    }
#endif

}

bool QMongoDB::update_one(const char *collection, QBSON filter, QBSON updateDocument)
{
#ifdef MAC_IOS
    return this->update_one(QString(collection),filter,updateDocument);
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update_one (col,_filter,_updateDocument,nullptr,nullptr,&error) )
    {
        QString err;
        for( int i = 0 ; i < 504 ; i++ )
        {
            if( error.message[i] )
            {
                err += error.message[i];
            }else{
                break;
            }
        }
        mLastError = err;
        return false;
    }else{
        return true;
    }
#endif

}

bool QMongoDB::Delete(QString collection, QBSON filter)
{

#ifdef MAC_IOS
    bool returnValue = false;
    bool timeout = false;
    int counter = 0;
    int control = 0;
    QDataStream out;

    qDebug() << "delete_one " << filter.tojson().c_str();
    mSocket->send( collection ,NetworkCommand::delete_one , filter );

    while ( !returnValue ) {

        switch (control) {

        case 0:
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            control = 1;
            counter++;
            break;

        case 1:
            if( mSocket->bufferCompleted(out) )
            {
                returnValue = true;
            }

            if( !returnValue )
            {
                if( counter >= 30 )
                {
                    returnValue = true;
                    timeout = true;
                }
            }
            control = 0;
            break;

        default:
            break;
        }
    }

    if( timeout ) return false;;

    if( returnValue )
    {

            int commandType;
            out >> commandType;
            NetworkCommand command = static_cast<NetworkCommand>(commandType);

            if( command != NetworkCommand::delete_one )
            {
                qDebug() << "invalid data";
                return false;
            }

            bool deleted = false;
            out >> deleted;
            return deleted;

    }else{
        return false;
    }
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
#endif



}

bool QMongoDB::Delete(std::string collection, QBSON filter)
{
#ifdef MAC_IOS
    return true;
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
#endif

}

bool QMongoDB::Delete(const char *collection, QBSON filter)
{
#ifdef MAC_IOS
    return true;
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
#endif

}

int64_t QMongoDB::count(const QString &collection, QBSON filter)
{
#ifdef MAC_IOS
    return 0;
#else
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _filter = convert(filter);

    bson_t* opts = bson_new();

    int64_t count = 0;

    count = mongoc_collection_count_documents(col,_filter,opts,nullptr,nullptr,&error);
    bson_destroy (opts);
    bson_destroy (_filter);

    return count;
#endif
}

QElement QMongoDB::uploadfile(QString filename, QString key)
{

#ifdef MAC_IOS
    return QElement();
#else

    QFileInfo info(filename);

    QFile qfile(filename);

    QByteArray ar;

    if( qfile.open(QIODevice::ReadOnly) )
    {
        ar = qfile.readAll();
        qfile.close();
    }else{
        qDebug() << "file can not open";
        return QElement();
    }

    auto hash = QCryptographicHash::hash(ar,QCryptographicHash::Algorithm::Md5);

    QMimeDatabase mimedb;

    QMimeType mime = mimedb.mimeTypeForFile(filename, QMimeDatabase::MatchContent);

    QBSON file;

    file.append("length",ar.size(),QElementType::b_int64);
    file.append("chunkSize",261120);
    file.append("md5",QString::fromUtf8(hash.toHex()));
    file.append("filename",info.fileName());



    if( this->count("fs.files",file) )
    {
        auto val = this->find_one("fs.files",file);
        return QElement(QOid(val["_id"].getOid().oid()),key);
    }else{
        if( !this->insert_one("fs.files",file) )
        {
            qDebug() << "Error insert File Description";
            return QElement();
        }
    }





    auto cursor = this->find("fs.files",file);

    if( !cursor.count() )
    {
        qDebug() << "no inserted file";
        return QElement();
    }

    int __Counter = 0;
    for( auto bson : cursor )
    {
        qDebug() << __Counter++ << bson.tojson().c_str();
    }

    QBSON val = cursor.last();


    int diveded = ar.size()/261120;
    int atik = ar.size()%261120;


    if( diveded )
    {
        bool insertedFail = false;
        for( int i = 0 ; i < diveded ; i++ )
        {
            auto part = ar.mid(0,261120);
            ar.remove(0,261120);
            QBSON chunk;

            chunk.append("files_id",QOid(val["_id"].getOid().oid()));
            chunk.append("n",i);
            chunk.append("data",part);
            if( !this->insert_one("fs.chunks",chunk) )
            {
                insertedFail = true;
                break;
            }
        }
        if( insertedFail )
        {
            qDebug() << "insert Fail multi chunk";
            return QElement();
        }
    }


    if( atik )
    {
        auto part = ar;
        QBSON chunk;
        chunk.append("files_id",QOid(val["_id"].getOid().oid()));
        chunk.append("n",diveded);
        chunk.append("data",part);
        if( !this->insert_one("fs.chunks",chunk) )
        {
            qDebug() << "insert Fail atik";
            return QElement();
        }
    }

    qDebug() << "Uploaded File Oid: " << val["_id"].getOid().oid() << key;

    return QElement(QOid(val["_id"].getOid().oid()),key);

#endif




}


QString QMongoDB::downloadfile( QOid fileoid, bool fileNametoOid )
{
#ifdef MAC_IOS
    return "";
#else

    if( QFile::exists("temp/"+fileoid.oid() ) )
    {
        QString returnFileName;
        QFile file("temp/"+fileoid.oid() );

        if( file.open(QIODevice::ReadOnly) )
        {
            auto ar = file.readAll();
            returnFileName = QString::fromUtf8(ar);
            file.close();
            file.flush();
            return returnFileName;
        }
    }

    QBSON filter;

    filter.append("_id",fileoid);

    auto filedoc = this->find_one("fs.files",filter);
    QString files_id;
    try {
        files_id = filedoc["_id"].getOid().oid();
    } catch (QError &e) {
        return QString("Error:") + e.what();
    }


    filter.clear();

    filter.append("files_id",filedoc["_id"].getOid());

    QOption findOption;

    QProjection projection;

    projection.set("data",false);

    findOption.setProjection(projection);

    auto chunks = this->find("fs.chunks",filter,findOption);

     QString filename;

     QDir dir;

     if( !dir.exists("temp") )
     {
         dir.mkdir("temp");
     }

     if( fileNametoOid )
     {
         QFileInfo info(filedoc["filename"].getValue().toString());
         filename = "temp/"+filedoc["_id"].getOid().oid()+"."+info.suffix();
     }else{
         filename = "temp/"+filedoc["filename"].getValue().toString();
     }

    QFile file(filename);

    if( file.open(QIODevice::WriteOnly) )
    {
        quint64 size = 0;
        int skip = 0;



        auto length = filedoc["length"].getValue().toLongLong();
        auto chunksize = filedoc["chunkSize"].getValue().toLongLong();

        auto counter = length / chunksize + 1;

        for( auto i = 0 ; i < counter ; i++ )
        {

            QOption dataOption;

            QSort sort;
            sort.sortByAscending("n");

            dataOption.setSort(sort);
            dataOption.setSkip(skip);

            QBSON fileFilter;

            fileFilter.append("files_id",filedoc["_id"].getOid());

            auto val = this->find_one("fs.chunks",fileFilter,dataOption);

            file.seek(size);

            QByteArray ar = val["data"].getBinary();

            file.write(ar);
            size += ar.size();
            emit gridfsbytereceived(size);
            skip++;
        }
        file.close();
        file.flush();
    }

    {
        QFile file("temp/"+fileoid.oid() );

        if( file.open(QIODevice::ReadWrite) )
        {
            file.write(filename.toUtf8());
            file.close();
            file.flush();
        }
    }
    return filename;
#endif

}




qlonglong QMongoDB::getfilesize(QOid fileoid)
{
#ifdef MAC_IOS
    return 0;
#else
    QBSON filter;

    filter.append("_id",fileoid);

    auto filedoc = this->find_one("fs.files",filter);

    auto files_id = filedoc["length"].getValue().toInt();

    return files_id;
#endif


}

QString QMongoDB::getfilename(QOid fileoid)
{
#ifdef MAC_IOS
    return "";
#else
    QBSON filter;

    filter.append("_id",fileoid);

    auto filedoc = this->find_one("fs.files",filter);

    auto files_id = filedoc["filename"].getValue().toString();

    return files_id;
#endif


}

QString QMongoDB::getLastError() const
{
#ifdef MAC_IOS
    return "";
#else
    return mLastError;
#endif

}

QString QMongoDB::getUrl() const
{
    return mUrl;
}

QString QMongoDB::getDb() const
{
    return db;
}

#ifdef MAC_IOS

#else
void convert(QBSON &obj,bson_t* parent);

bson_t *convert(QBSON &obj){

    auto doc = bson_new();
    bson_init (doc);

    for( auto element : obj )
    {
        switch (element.getType()) {
        case QElementType::b_double:
            BSON_APPEND_DOUBLE( doc , element.getKey().toStdString().c_str() , element.getValue().toDouble() );
            break;
        case QElementType::b_utf8:
            BSON_APPEND_UTF8( doc , element.getKey().toStdString().c_str() , element.getValue().toString().toStdString().c_str() );
            break;
        case QElementType::b_oid:
        {
            bson_oid_t oid;
            bson_oid_init_from_string (&oid, element.getOid().oid().toStdString().c_str());
            BSON_APPEND_OID( doc , element.getKey().toStdString().c_str() , &oid );
        }
            break;
        case QElementType::b_int64:
            BSON_APPEND_INT64( doc , element.getKey().toStdString().c_str() , element.getValue().toLongLong()) ;
            break;
        case QElementType::b_int32:
            BSON_APPEND_INT32( doc , element.getKey().toStdString().c_str() , element.getValue().toInt() );
            break;
        case QElementType::b_document:
        {
            auto _element = element.toDocument();
            bson_t _bson_tObj;
            bson_init (&_bson_tObj);
            convert(_element,&_bson_tObj);
            if( !BSON_APPEND_DOCUMENT(doc,element.getKey().toStdString().c_str(),&_bson_tObj) ){
                qDebug() << "Convert Document Error";
            }
        }
            break;

        case QElementType::b_array:
        {
            auto _array = element.toArray();
            bson_t* child = bson_new();
            bson_append_array_begin (doc, element.getKey().toStdString().c_str(), element.getKey().length(),child);
            convertArray(_array,child);
            bson_append_array_end (doc, child);
        }
            break;

        case QElementType::b_bool:
            BSON_APPEND_BOOL(doc , element.getKey().toStdString().c_str() , element.getValue().toBool() );
            break;
        case QElementType::b_binary:
            BSON_APPEND_BINARY( doc , element.getKey().toStdString().c_str() , BSON_SUBTYPE_BINARY, reinterpret_cast<const uint8_t*>( element.getBinary().data() ), element.getBinary().size() );
            break;
        default:
            throw "No Element Type Detected. Skipped Key: " + element.getKey().toStdString();
            break;
        }
    }
    return doc;
}


void convert(QBSON &obj,bson_t* parent){



    for( auto element : obj )
    {
        switch (element.getType()) {
        case QElementType::b_double:
            BSON_APPEND_DOUBLE( parent , element.getKey().toStdString().c_str() , element.getValue().toDouble() );
            break;
        case QElementType::b_utf8:
            BSON_APPEND_UTF8( parent , element.getKey().toStdString().c_str() , element.getValue().toString().toStdString().c_str() );
            break;
        case QElementType::b_oid:
        {
            bson_oid_t oid;
            bson_oid_init_from_string (&oid, element.getOid().oid().toStdString().c_str());
            BSON_APPEND_OID( parent , element.getKey().toStdString().c_str() , &oid );
        }
            break;
        case QElementType::b_int64:
            BSON_APPEND_INT64( parent , element.getKey().toStdString().c_str() , element.getValue().toLongLong()) ;
            break;
        case QElementType::b_int32:
            BSON_APPEND_INT32( parent , element.getKey().toStdString().c_str() , element.getValue().toInt() );
            break;
        case QElementType::b_document:
        {
            auto _element = element.toDocument();
            bson_t _bson_tObj;
            bson_init (&_bson_tObj);
            convert(_element,&_bson_tObj);
            if( !BSON_APPEND_DOCUMENT(parent,element.getKey().toStdString().c_str(),&_bson_tObj) ){
                qDebug() << "Convert Document Error";
            }
        }
            break;

        case QElementType::b_array:
        {
            auto _array = element.toArray();
            bson_t _bson_tObj;
            bson_init (&_bson_tObj);
            bson_append_array_begin (parent, element.getKey().toStdString().c_str(),element.getKey().length(), &_bson_tObj);
            convertArray(_array,&_bson_tObj);
            bson_append_array_end (parent, &_bson_tObj);
        }
            break;

        case QElementType::b_bool:
            BSON_APPEND_BOOL(parent , element.getKey().toStdString().c_str() , element.getValue().toBool() );
            break;
        case QElementType::b_binary:
            BSON_APPEND_BINARY( parent , element.getKey().toStdString().c_str() , BSON_SUBTYPE_BINARY, reinterpret_cast<const uint8_t*>( element.getBinary().data() ), element.getBinary().size() );
            break;
        default:
            throw "No Element Type Detected. Skipped Key: " + element.getKey().toStdString();
            break;
        }
    }


}

void convertArray(QArray &array , bson_t* child){


    for( auto element : array )
    {
        switch (element.getType()) {
        case QElementType::b_double:
            BSON_APPEND_DOUBLE( child , element.getKey().toStdString().c_str() , element.getValue().toDouble() );
            break;
        case QElementType::b_utf8:
            BSON_APPEND_UTF8( child , element.getKey().toStdString().c_str() , element.getValue().toString().toStdString().c_str() );
            break;
        case QElementType::b_oid:
        {
            bson_oid_t oid;
            bson_oid_init_from_string (&oid, element.getOid().oid().toStdString().c_str());
            BSON_APPEND_OID( child , element.getKey().toStdString().c_str() , &oid );
        }
            break;
        case QElementType::b_int64:
            BSON_APPEND_INT64( child , element.getKey().toStdString().c_str() , element.getValue().toLongLong()) ;
            break;
        case QElementType::b_int32:
            BSON_APPEND_INT32( child , element.getKey().toStdString().c_str() , element.getValue().toInt() );
            break;
        case QElementType::b_document:
        {
            auto _element = element.toDocument();
            bson_t _bson_tObj;
            bson_init (&_bson_tObj);
            convert(_element,&_bson_tObj);
            if( !BSON_APPEND_DOCUMENT(child,element.getKey().toStdString().c_str(),&_bson_tObj) ){
                qDebug() << "Convert Document Error";
            }
        }
            break;

        case QElementType::b_array:
        {
            auto _array = element.toArray();
            bson_t _bson_tObj;
            bson_init (&_bson_tObj);
            bson_append_array_begin (child, element.getKey().toStdString().c_str(),element.getKey().length(), &_bson_tObj);
            convertArray(_array,&_bson_tObj);
            bson_append_array_end (child, &_bson_tObj);
        }
            break;

        case QElementType::b_bool:
            BSON_APPEND_BOOL(child , element.getKey().toStdString().c_str() , element.getValue().toBool() );
            break;
        case QElementType::b_binary:
            BSON_APPEND_BINARY( child , element.getKey().toStdString().c_str() , BSON_SUBTYPE_BINARY, reinterpret_cast<const uint8_t*>( element.getBinary().data() ), element.getBinary().size() );
            break;
        default:
            throw "No Element Type Detected. Skipped Key: " + element.getKey().toStdString();
            break;
        }
    }

}




void RecursiveDocument(  bson_iter_t *iter , QBSON &obj_ ){

    bson_iter_t sub_iter;

    QBSON obj;

    while (bson_iter_next ( iter ) ) {
        if( bson_iter_recurse ( iter , &sub_iter ) )
        {
            QString key_ =  bson_iter_key(iter);

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_ARRAY )
            {
                QArray array;
                RecursiveArray(&sub_iter,array);
                obj_.append(key_,array);
            }

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOCUMENT )
            {
                QBSON obj__;
                RecursiveDocument(&sub_iter,obj__);
                obj_.append(key_,obj__);
            }
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_UTF8 )
        {
            auto value = bson_iter_value(iter);
            obj_.append( bson_iter_key(iter),QString::fromUtf8(value->value.v_utf8.str,value->value.v_utf8.len ) , QElementType::b_utf8 );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOUBLE )
        {
            auto value = bson_iter_value(iter);
            obj_.append( bson_iter_key(iter), value->value.v_double , QElementType::b_double );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_BOOL )
        {
            auto value = bson_iter_value(iter);
            obj_.append( bson_iter_key(iter), value->value.v_bool , QElementType::b_bool );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT32 )
        {
            auto value = bson_iter_value(iter);
            obj_.append( bson_iter_key(iter), value->value.v_int32 , QElementType::b_int32 );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT64 )
        {
            auto value = bson_iter_value(iter);
            obj_.append( bson_iter_key(iter), value->value.v_int64 , QElementType::b_int64 );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_OID )
        {
            auto value = bson_iter_value(iter);
            QString hexCode;
            for( int i = 0 ; i < 12 ; i++ )
            {
                if( value->value.v_oid.bytes[i] < 16 )
                {
                    hexCode += "0"+QString::number( value->value.v_oid.bytes[i] , 16 );
                }else{
                    hexCode += QString::number( value->value.v_oid.bytes[i] , 16 );
                }
            }
            obj_.append( bson_iter_key(iter) , QOid(hexCode) );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_BINARY )
        {
            auto value = bson_iter_value(iter);

            QByteArray binary = QByteArray(reinterpret_cast<const char*>(value->value.v_binary.data),value->value.v_binary.data_len);

            obj_.append( bson_iter_key(iter), binary , QElementType::b_binary );
        }
    }

}

void RecursiveArray( bson_iter_t *iter , QArray &array_ )
{
    bson_iter_t sub_iter;

    while (bson_iter_next (iter) ) {

        if( bson_iter_recurse (iter, &sub_iter) )
        {
            QString key_ =  bson_iter_key(iter);

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_ARRAY )
            {
                QArray array;
                RecursiveArray(&sub_iter,array);
                array_.append(array);
            }

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOCUMENT )
            {
                QBSON obj;
                RecursiveDocument(&sub_iter,obj);
                array_.append(obj);
            }
        }

        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_UTF8 )
        {
            auto value = bson_iter_value(iter);
            array_.append( QString::fromUtf8(value->value.v_utf8.str,value->value.v_utf8.len )  );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOUBLE )
        {
            auto value = bson_iter_value(iter);
            array_.append(  value->value.v_double  );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_BOOL )
        {
            auto value = bson_iter_value(iter);
            array_.append(  value->value.v_bool  );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT32 )
        {
            auto value = bson_iter_value(iter);
            array_.append(  value->value.v_int32  );
        }

        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT64 )
        {
            auto value = bson_iter_value(iter);
            array_.append(  value->value.v_int64  );
        }

        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_OID )
        {
            auto value = bson_iter_value(iter);
            QString hexCode;
            for( int i = 0 ; i < 12 ; i++ )
            {
                if( value->value.v_oid.bytes[i] < 16 )
                {
                    hexCode += "0"+QString::number( value->value.v_oid.bytes[i] , 16 );
                }else{
                    hexCode += QString::number( value->value.v_oid.bytes[i] , 16 );
                }
            }
            QElement oid( QElementType::b_oid , QOid(hexCode) , "" );
            array_.append( oid );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_BINARY )
        {
            auto value = bson_iter_value(iter);

            QByteArray binary = QByteArray(reinterpret_cast<const char*>(value->value.v_binary.data),value->value.v_binary.data_len);

            array_.append( binary );
        }
    }

}

void _find(_mongoc_cursor_t* cursor , QVector<QBSON>* list){


    const bson_t *doc;
    bson_iter_t iter;
    bson_iter_t sub_iter;

    while (mongoc_cursor_next (cursor, &doc)) {
        if (bson_iter_init (&iter, doc) ) {
            QBSON obj;
            RecursiveDocument(&iter,obj);
            list->append(obj);
        }
    }
}



#endif




