#ifndef QMLMONGODB_H
#define QMLMONGODB_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qmongodb.h"

class QMLMongoDB : public QMongoDB
{
    Q_OBJECT
public:
    explicit QMLMongoDB( QString mongourl , QString database );

signals:

public slots:
};

#endif // QMLMONGODB_H
