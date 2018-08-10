#include "qmongodb.h"

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>


static mongoc_client_t* client;

static mongoc_gridfs_t* gridfs;

void _find(QVector<QBson>* list , mongoc_cursor_t* cursor );

const bson_t *convert(QBson &obj);

void convertArray(QArray &array, bson_t *child);


void RecursiveDocument( QString key , bson_iter_t *iter , QBson &obj_ );

void RecursiveArray(QString key , bson_iter_t *iter , QArray &array_ );

QMongoDB::QMongoDB( QString mongodburl , QString database , QObject *parent )
    :QObject ( parent ),
      mUrl( mongodburl ),
      db( database )
{
    client = mongoc_client_new (this->mUrl.toStdString().c_str());

    bson_error_t error;
    gridfs = mongoc_client_get_gridfs(client,db.toStdString().c_str(),"fs",&error);

}

QMongoDB::~QMongoDB()
{
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}

void QMongoDB::instance()
{
    mongoc_init();
}

QVector<QBson> QMongoDB::find(QString collection, QBson filter, QOption option)
{
    QVector<QBson> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    auto _filter = convert(filter);

    QBson bson = option.getBson();
    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(&list,cursor);

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (col);


    return list;
}

QVector<QBson> QMongoDB::find(std::string collection, QBson filter, QOption option)
{
    QVector<QBson> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    auto _filter = convert(filter);

    QBson bson = option.getBson();
    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(&list,cursor);

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (col);

    return list;
}

QVector<QBson> QMongoDB::find(const char *collection, QBson filter, QOption option)
{

    QVector<QBson> list;

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    auto _filter = convert(filter);

    QBson bson = option.getBson();

    auto _option = convert(bson);

    auto cursor = mongoc_collection_find_with_opts (col, _filter, _option , nullptr);

    _find(&list,cursor);

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (col);

    return list;
}

QBson QMongoDB::find_one(QString collection, QBson filter, QOption option)
{

    option.setLimit(1);

    auto list = this->find(collection,filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBson();
    }

}

QBson QMongoDB::find_one(std::string collection, QBson filter, QOption option)
{
    option.setLimit(1);

    auto list = this->find(collection.c_str(),filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBson();
    }
}

QBson QMongoDB::find_one(const char *collection, QBson filter, QOption option)
{
    option.setLimit(1);

    auto list = this->find(collection,filter,option);

    if( list.count() )
    {
        return list.first();
    }else{
        return QBson();
    }
}

bool QMongoDB::insert_one(QString collection, QBson document)
{

    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _document = convert(document);

    if( !mongoc_collection_insert(col,mongoc_insert_flags_t::MONGOC_INSERT_NONE,_document,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::insert_one(std::string collection, QBson document)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    bson_error_t error;
    auto _document = convert(document);

    if( !mongoc_collection_insert(col,mongoc_insert_flags_t::MONGOC_INSERT_NONE,_document,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::insert_one(const char *collection, QBson document)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    bson_error_t error;
    auto _document = convert(document);

    if( !mongoc_collection_insert(col,mongoc_insert_flags_t::MONGOC_INSERT_NONE,_document,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::update_one(QString collection, QBson filter, QBson updateDocument)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update(col,mongoc_update_flags_t::MONGOC_UPDATE_UPSERT,_filter,_updateDocument,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::update_one(std::string collection, QBson filter, QBson updateDocument)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update(col,mongoc_update_flags_t::MONGOC_UPDATE_UPSERT,_filter,_updateDocument,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::update_one(const char *collection, QBson filter, QBson updateDocument)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    bson_error_t error;
    auto _filter = convert(filter);
    auto _updateDocument = convert(updateDocument);

    if( !mongoc_collection_update(col,mongoc_update_flags_t::MONGOC_UPDATE_UPSERT,_filter,_updateDocument,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::Delete(QString collection, QBson filter)
{


    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.toStdString().c_str());

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }

}

bool QMongoDB::Delete(std::string collection, QBson filter)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection.c_str());

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

bool QMongoDB::Delete(const char *collection, QBson filter)
{
    auto col = mongoc_client_get_collection(client,db.toStdString().c_str(),collection);

    bson_error_t error;
    auto _filter = convert(filter);


    if( !mongoc_collection_delete (col,mongoc_delete_flags_t::MONGOC_DELETE_SINGLE_REMOVE,_filter,nullptr,&error) )
    {
        return false;
    }else{
        return true;
    }
}

QElement QMongoDB::uploadfile(QString filename , QString key)
{

    bson_error_t error;


    auto stream = mongoc_stream_file_new_for_path (filename.toStdString().c_str(), O_RDONLY, 0);

    mongoc_gridfs_file_opt_t opt = {nullptr};

    QFileInfo info(filename);


    QFile qfile(filename);
    QByteArray ar;

    if( qfile.open(QIODevice::ReadOnly) )
    {
        ar = qfile.readAll();
        qfile.close();
    }


    opt.filename = info.fileName().toStdString().c_str();

    auto hash = QCryptographicHash::hash(ar,QCryptographicHash::Algorithm::Md5);
    opt.md5 = hash.toHex().toStdString().c_str();

    auto file = mongoc_gridfs_create_file_from_stream (gridfs, stream, &opt);



    bson_value_t id;

    id.value_type = BSON_TYPE_INT32;
    id.value.v_int32 = 1;

    if (!mongoc_gridfs_file_set_id (file, &id, &error)) {

    }

    auto rid = mongoc_gridfs_file_get_id(file);

    QString hexCode;
    for( int i = 0 ; i < 12 ; i++ )
    {
        if( rid->value.v_oid.bytes[i] < 16 )
        {
            hexCode += "0"+QString::number( rid->value.v_oid.bytes[i] , 16 );
        }else{
            hexCode += QString::number( rid->value.v_oid.bytes[i] , 16 );
        }
    }

    mongoc_gridfs_file_save (file);

    return QElement(QElementType::b_oid,hexCode,key);

}

QString QMongoDB::downloadfile(QElement fileoid, bool fileNametoOid)
{
    bson_error_t error;

    QBson filter;
    filter.append("_id",fileoid);
    auto _filter = convert(filter);
    auto file = mongoc_gridfs_find_one(gridfs,_filter,&error);
    const char* filename = mongoc_gridfs_file_get_filename(file);

    auto stream = mongoc_stream_gridfs_new(file);

    QDir dir;
    if( !dir.exists("temp")){
        dir.mkdir("temp");
    }



    QString _filename = QString("temp/")+filename;

    if( fileNametoOid ) {
        QFileInfo info(filename);
        _filename = QString("temp/")+fileoid.getValue().toString()+"."+info.suffix();
    }

    QFile qfile(_filename);

    if( qfile.open(QIODevice::ReadWrite) )
    {
        ssize_t r;
        char buf[4096];
        mongoc_iovec_t iov;

#if defined(Q_OS_ANDROID)
        iov.iov_base = static_cast<void*>(buf);
        iov.iov_len = sizeof (buf);
#elif defined(Q_OS_WIN)
        iov.iov_base = static_cast<char*>(buf);
        iov.iov_len = sizeof (buf);
#elif defined(Q_OS_LINUX)
        iov.iov_base = static_cast<void*>(buf);
        iov.iov_len = sizeof (buf);
#else

#endif

        QByteArray wbyte;
        for(;;)
        {
            r = mongoc_stream_readv (stream, &iov, 1, -1, 0);
            if (r == 0) {
                break;
            }

#if defined(Q_OS_ANDROID)
            QByteArray ar = QByteArray::fromRawData(static_cast<char*>(iov.iov_base),iov.iov_len);
            wbyte +=ar;
            emit gridfsbytereceived(wbyte.size());
            r = 0;
#elif defined(Q_OS_WIN)
            QByteArray ar = QByteArray::fromRawData(iov.iov_base,iov.iov_len);
            wbyte +=ar;
            emit gridfsbytereceived(wbyte.size());
            r = 0;
#elif defined(Q_OS_LINUX)
            QByteArray ar = QByteArray::fromRawData(static_cast<char*>(iov.iov_base),iov.iov_len);
            wbyte +=ar;
            emit gridfsbytereceived(wbyte.size());
            r = 0;
#else

#endif




        }
        qfile.write(wbyte);
        qfile.close();
    }

    return _filename;
}

QByteArray QMongoDB::downloadByteArray(QElement fileoid)
{
    bson_error_t error;

//    auto gridfs = mongoc_client_get_gridfs(client,db.toStdString().c_str(),"fs",&error);

    QBson filter;
    filter.append("_id",fileoid);
    auto _filter = convert(filter);
    auto file = mongoc_gridfs_find_one(gridfs,_filter,&error);
    const char* filename = mongoc_gridfs_file_get_filename(file);

    auto stream = mongoc_stream_gridfs_new(file);

    QByteArray wbyte;

    ssize_t r;
    char buf[4096];
    mongoc_iovec_t iov;
#if defined(Q_OS_ANDROID)
    iov.iov_base = static_cast<void*>(buf);
    iov.iov_len = sizeof (buf);
#elif defined(Q_OS_WIN)
    iov.iov_base = static_cast<char*>(buf);
    iov.iov_len = sizeof (buf);
#elif defined(Q_OS_LINUX)
#else

#endif


    for(;;)
    {
        r = mongoc_stream_readv (stream, &iov, 1, -1, 0);
        if (r == 0) {
            break;
        }
#if defined(Q_OS_ANDROID)
        QByteArray ar = QByteArray::fromRawData(static_cast<char*>(iov.iov_base),iov.iov_len);
        wbyte +=ar;
        emit gridfsbytereceived(wbyte.size());
        r = 0;
#elif defined(Q_OS_WIN)
        QByteArray ar = QByteArray::fromRawData(iov.iov_base,iov.iov_len);
        wbyte +=ar;
        emit gridfsbytereceived(wbyte.size());
        r = 0;
#elif defined(Q_OS_LINUX)
#else

#endif
        emit gridfsbytereceived(wbyte.size());
        r = 0;
    }
    return wbyte;
}

qlonglong QMongoDB::getfilesize(QElement fileoid)
{

    if( fileoid.getType() != QElementType::b_oid )
    {
        throw QString("Element type must be a QElementType::b_oid");
        return 0;
    }

    bson_error_t error;

    QBson filter;
    filter.append("_id",fileoid);
    auto _filter = convert(filter);

    auto file = mongoc_gridfs_find_one(gridfs,_filter,&error);

    auto size = mongoc_gridfs_file_get_length(file);

    return size;

}

QString QMongoDB::getfilename(QElement fileoid)
{
    if( fileoid.getType() != QElementType::b_oid )
    {
        throw QString("Element type must be a QElementType::b_oid");
        return nullptr;
    }

    bson_error_t error;

    QBson filter;
    filter.append("_id",fileoid);
    auto _filter = convert(filter);

    auto file = mongoc_gridfs_find_one(gridfs,_filter,&error);

    auto filename = mongoc_gridfs_file_get_filename(file);

    return filename;

}


const bson_t *convert(QBson &obj){

    auto doc = bson_new();

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
            bson_oid_init_from_string (&oid, element.getValue().toString().toStdString().c_str());
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
            auto _bson_tObj = convert(_element);
            BSON_APPEND_DOCUMENT( doc , element.getKey().toStdString().c_str() , _bson_tObj );
        }
            break;

        case QElementType::b_array:
        {
            auto _array = element.toArray();
            bson_t* child = bson_new();
            BSON_APPEND_ARRAY_BEGIN (doc, element.getKey().toStdString().c_str(), child);
            convertArray(_array,child);
            bson_append_array_end (doc, child);
        }
            break;

        case QElementType::b_bool:
            BSON_APPEND_BOOL(doc , element.getKey().toStdString().c_str() , element.getValue().toBool() );
            break;
        default:
            throw "No Element Type Detected. Skipped Key: " + element.getKey().toStdString();
            break;
        }
    }


    return doc;
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
            bson_oid_init_from_string (&oid, element.getValue().toString().toStdString().c_str());
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
            auto _bson_tObj = convert(_element);
            BSON_APPEND_DOCUMENT( child , element.getKey().toStdString().c_str() , _bson_tObj );
        }
            break;

        case QElementType::b_array:
        {
            auto _array = element.toArray();
            bson_t* child_ = bson_new();
            BSON_APPEND_ARRAY_BEGIN (child, element.getKey().toStdString().c_str(), child_);
            convertArray(_array,child);
            bson_append_array_end (child, child_);
        }
            break;

        case QElementType::b_bool:
            BSON_APPEND_BOOL(child , element.getKey().toStdString().c_str() , element.getValue().toBool() );
            break;
        default:
            throw "No Element Type Detected. Skipped Key: " + element.getKey().toStdString();
            break;
        }
    }
}

void RecursiveDocument( QString key , bson_iter_t *iter , QBson &obj_ ){

    bson_iter_t sub_iter;
    QBson obj;

    while (bson_iter_next (iter) ) {

        if( bson_iter_recurse (iter, &sub_iter) )
        {
            QString key_ =  bson_iter_key(&sub_iter);

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_ARRAY )
            {
                QArray array;
                RecursiveArray(key_,&sub_iter,array);
            }

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOCUMENT )
            {
                RecursiveDocument(key_,&sub_iter,obj);
            }
        }

        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_UTF8 )
        {
            auto value = bson_iter_value(iter);
            obj.append( bson_iter_key(iter),QString::fromUtf8(value->value.v_utf8.str,value->value.v_utf8.len ) , QElementType::b_utf8 );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOUBLE )
        {
            auto value = bson_iter_value(iter);
            obj.append( bson_iter_key(iter), value->value.v_double , QElementType::b_double );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_BOOL )
        {
            auto value = bson_iter_value(iter);
            obj.append( bson_iter_key(iter), value->value.v_bool , QElementType::b_bool );
        }
        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT32 )
        {
            auto value = bson_iter_value(iter);
            obj.append( bson_iter_key(iter), value->value.v_int32 , QElementType::b_int32 );
        }

        if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_INT64 )
        {
            auto value = bson_iter_value(iter);
            obj.append( bson_iter_key(iter), value->value.v_int64 , QElementType::b_int64 );
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
            obj.append( bson_iter_key(iter),hexCode , QElementType::b_oid );
        }
    }
    obj_.append(key,obj);
}





void RecursiveArray( QString key , bson_iter_t *iter , QArray &array_ )
{
    bson_iter_t sub_iter;

    while (bson_iter_next (iter) ) {

        if( bson_iter_recurse (iter, &sub_iter) )
        {
            QString key_ =  bson_iter_key(&sub_iter);

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_ARRAY )
            {
                QArray array;
                RecursiveArray(key_,&sub_iter,array);
                array_.append(array);
            }

            if( bson_iter_type(iter) == bson_type_t::BSON_TYPE_DOCUMENT )
            {
                QBson obj;
                RecursiveDocument(key_,&sub_iter,obj);
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
            QElement oid(QElementType::b_oid);
            oid.setKey( "_id" );
            oid.setType( QElementType::b_oid );
            oid.setValue( hexCode );
            array_.append( oid );
        }
    }
}






void _find(QVector<QBson>* list , mongoc_cursor_t* cursor ){


    qDebug() << "_find";
    const bson_t *doc;
    bson_iter_t iter;
    bson_iter_t sub_iter;

    while (mongoc_cursor_next (cursor, &doc)) {

        if (bson_iter_init (&iter, doc) ) {
            QBson obj;
            while (bson_iter_next (&iter) ) {

                if( bson_iter_recurse (&iter, &sub_iter) )
                {
                    QString key = bson_iter_key(&iter);
                    if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_ARRAY )
                    {
                        QArray array;
                        RecursiveArray(key,&sub_iter,array);
                        obj.append(key,array);
                    }
                    if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_DOCUMENT )
                    {
                        RecursiveDocument(key,&sub_iter,obj);
                    }
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_UTF8 )
                {
                    auto value = bson_iter_value(&iter);
                    obj.append( bson_iter_key(&iter),QString::fromUtf8(value->value.v_utf8.str,value->value.v_utf8.len ) , QElementType::b_utf8 );
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_DOUBLE )
                {
                    auto value = bson_iter_value(&iter);
                    obj.append( bson_iter_key(&iter), value->value.v_double , QElementType::b_double );
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_BOOL )
                {
                    auto value = bson_iter_value(&iter);
                    obj.append( bson_iter_key(&iter), value->value.v_bool , QElementType::b_bool );
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_INT32 )
                {
                    auto value = bson_iter_value(&iter);
                    obj.append( bson_iter_key(&iter), value->value.v_int32 , QElementType::b_int32 );
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_INT64 )
                {
                    auto value = bson_iter_value(&iter);
                    obj.append( bson_iter_key(&iter), value->value.v_int64 , QElementType::b_int64 );
                }
                if( bson_iter_type(&iter) == bson_type_t::BSON_TYPE_OID )
                {
                    auto value = bson_iter_value(&iter);
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
                    obj.append( bson_iter_key(&iter),hexCode , QElementType::b_oid );
                }
            }
            list->append(obj);
        }
    }




}
