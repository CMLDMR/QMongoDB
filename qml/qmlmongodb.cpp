#include "qmlmongodb.h"
#include <QtQml/QQmlApplicationEngine>
#include "qml/qmlelement.h"
#include "qml/qmlbson.h"

#define MAJOR   0
#define MINOR   5

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

static void registerQmlMongoTypes() {

    qDebug() << "REGISTER QML COMPONENT";
    qmlRegisterType<QMLElement>("com.mongodb", MAJOR, MINOR, "QMLElement");
    qmlRegisterType<QMLMongoDB>("com.mongodb", MAJOR, MINOR, "MongoDB");
    qmlRegisterType<QMLBSON>("com.mongodb", MAJOR, MINOR, "QMLBSON");

    qmlRegisterSingletonType<QMLElement>("com.mongodb", MAJOR, MINOR, "Element", QMLElementSingletonProvider);
    qmlRegisterSingletonType<QMLBSON>("com.mongodb", MAJOR, MINOR, "Bson", QMLBSONSingletonProvider);

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

QString QMLMongoDB::find_one( const QString &collection , const QString &filter , const QString &option )
{

    try {
        auto val = this->db->find_one("TEST",QBSON());
        try {
            qDebug() << val["birim"].getValue().toString();
            return val["birim"].getValue().toString();
        } catch (QError &e) {
            return e.what();
        }

    } catch (QError &e) {
        return e.what();
    }




}




QString QMLMongoDB::test()
{
    return "TEST";
}

