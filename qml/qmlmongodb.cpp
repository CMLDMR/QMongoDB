#include "qmlmongodb.h"
#include <QtQml/QQmlApplicationEngine>

#define MAJOR   0
#define MINOR   5


static void registerQmlMongoTypes() {
    qmlRegisterType<QMLMongoDB>("com.mongodb", MAJOR, MINOR, "Mongo");
}

Q_COREAPP_STARTUP_FUNCTION(registerQmlMongoTypes)




QMLMongoDB::QMLMongoDB(QString mongourl, QString database)
    : QMongoDB(mongourl,database)
{

}
