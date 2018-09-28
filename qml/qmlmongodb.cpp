#include "qmlmongodb.h"
#include <QtQml/QQmlApplicationEngine>
#include "qmlelement.h"
#include "qmlbson.h"
#include "qmlarray.h"

#include <QQmlContext>





static QObject *QMLElementSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static QMLElement *singletonClass = new QMLElement();
    return singletonClass;
}

static QObject *QMLBSONSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static QMLBSON *singletonClass = new QMLBSON();
    return singletonClass;
}

static QObject *QMLArraySingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static QMLArray *singletonClass = new QMLArray();
    return singletonClass;
}

static void registerQmlMongoTypes() {


    qmlRegisterType<QMLElement>("com.mongodb", MAJOR, MINOR, "QMLElement");
    qmlRegisterType<QMLMongoDB>("com.mongodb", MAJOR, MINOR, "MongoDB");
    qmlRegisterType<QMLBSON>("com.mongodb", MAJOR, MINOR, "QMLBSON");
    qmlRegisterType<QMLArray>("com.mongodb", MAJOR, MINOR, "QMLArray");


    qmlRegisterSingletonType<QMLElement>("com.mongodb", MAJOR, MINOR, "QElement", QMLElementSingletonProvider);
    qmlRegisterSingletonType<QMLBSON>("com.mongodb", MAJOR, MINOR, "QBSON", QMLBSONSingletonProvider);
    qmlRegisterSingletonType<QMLArray>("com.mongodb", MAJOR, MINOR, "QArray", QMLArraySingletonProvider);

}

Q_COREAPP_STARTUP_FUNCTION(registerQmlMongoTypes)



namespace MongoInstanceVariable {
///
/// \brief mDBUrl
/// Connection db url
static QString mDBUrl;

///
/// \brief mDBName
/// Connection db Name
static QString mDBName;

///
/// \brief instanceCalled
/// if this true Start Using QMongoDB else Nothing
static bool instanceCalled = false;
}





QMLMongoDB::QMLMongoDB()
    :QObject (), mStarted(false)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        this->start(MongoInstanceVariable::mDBUrl,MongoInstanceVariable::mDBName);
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
    }
}

QMLMongoDB &QMLMongoDB::operator=(const QMLMongoDB &db)
{
    return *this;
}

QMLMongoDB::~QMLMongoDB()
{
    qDebug() << "QMLMongoDB Destructor";
}

bool QMLMongoDB::isValid() const
{
    if( MongoInstanceVariable::instanceCalled )
    {
        return mStarted ;
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return false;
    }

}

void QMLMongoDB::start(const QString &mUrl, const QString &database)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        db = new QMongoDB(mUrl,database);
        mStarted = true;
    }else{
        mStarted = false;
    }

}

qint64 QMLMongoDB::count(const QString &collection, QMLBSON *filter)
{
    if( MongoInstanceVariable::instanceCalled )
    {
            return this->db->count( collection , filter->getQBSON() );
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return 0;
    }

}


QVariantList QMLMongoDB::find(const QString &collection, QMLBSON *filter, QMLBSON *option)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        QBSON _filter;
        if( filter )
        {
            _filter.append(filter->getMaplist());
        }
        QOption option_ ;
        if( option )
        {
            option_.setBson(option->getQBSON());
        }
        auto cursor = this->db->find(collection,filter->getQBSON(),option_);
        QVariantList bsonlist;
        for( auto element : cursor )
        {
            auto bson = new QMLBSON(element);
            bsonlist.append(QVariant::fromValue(bson));
        }
        return bsonlist;
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
    }

}

QMLBSON *QMLMongoDB::find_one(const QString &collection, QMLBSON *filter, QMLBSON *option)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        option->addInt("limit",1);
        QOption option_ ;
        if( option )
        {
            option_.setBson(option->getQBSON());
        }
        auto cursor = this->db->find_one( collection,filter->getQBSON() , option_ );

        auto bson = new QMLBSON(cursor);
        return bson;
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return new QMLBSON();
    }

}

bool QMLMongoDB::insert_one(const QString &collection, QMLBSON *bson)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        return this->db->insert_one(collection,bson->getQBSON());
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return false;
    }
}

bool QMLMongoDB::update_one(const QString &collection, QMLBSON *filter, QMLBSON *updatebson)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        return this->db->update_one(collection,filter->getQBSON(),updatebson->getQBSON());
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return false;
    }
}

bool QMLMongoDB::delete_one(const QString &collection, QMLBSON *filter)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        return this->db->Delete( collection , filter->getQBSON() );
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return false;
    }
}

QString QMLMongoDB::fileurl(const QString &oid, bool fileNametoOid)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        QString url = this->db->downloadfile(QOid(oid),fileNametoOid);
        return QUrl::fromLocalFile(url).toString();
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return "";
    }

}

QMLElement *QMLMongoDB::uploadfile(const QString &filename, QString key)
{
    if( MongoInstanceVariable::instanceCalled )
    {
        auto element = this->db->uploadfile(filename,key);
        return new QMLElement(element);
    }else{
        qDebug() << "Call QMLMongoDB::instance(url,dbname); before using Driver";
        return new QMLElement();
    }

}

QMongoDB *QMLMongoDB::getDb() const
{
    return db;
}

void QMLMongoDB::instance(const QString &url, const QString dbName)
{
    MongoInstanceVariable::mDBUrl = url;
    MongoInstanceVariable::mDBName = dbName;
    QMongoDB::instance();
    MongoInstanceVariable::instanceCalled = true;
}


