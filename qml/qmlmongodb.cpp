#include "qmlmongodb.h"
#include <QtQml/QQmlApplicationEngine>
#include "qmlelement.h"
#include "qmlbson.h"
#include "qmlarray.h"

#include <QQmlContext>



#define MAJOR   0
#define MINOR   6

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






QMLMongoDB::QMLMongoDB()
    :QObject (), mStarted(false)
{
    this->start("mongodb://<serikbeltr>:><<sErikBeltR>>1926><@195.175.200.2:41112/?authSource=SERIKBELTR","SERIKBELTR");
}

bool QMLMongoDB::isValid() const
{
    return mStarted ;
}

void QMLMongoDB::start(const QString &mUrl, const QString &database)
{
    qDebug() << "start function";
    db = new QMongoDB(mUrl,database);
    mStarted = true;
}


QVariantList QMLMongoDB::find(const QString &collection, QMLBSON *filter, QMLBSON *option)
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
}

QMLBSON *QMLMongoDB::find_one(const QString &collection, QMLBSON *filter, QMLBSON *option)
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
}

bool QMLMongoDB::insert_one(const QString &collection, QMLBSON *bson)
{
    return this->db->insert_one(collection,bson->getQBSON());
}

bool QMLMongoDB::update_one(const QString &collection, QMLBSON *filter, QMLBSON *updatebson)
{
    return this->db->update_one(collection,filter->getQBSON(),updatebson->getQBSON());
}

bool QMLMongoDB::delete_one(const QString &collection, QMLBSON *filter)
{
    return this->db->Delete( collection , filter->getQBSON() );
}




QString QMLMongoDB::test()
{
    return "TEST";
}

