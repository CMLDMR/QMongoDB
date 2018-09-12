#ifndef QMLMONGODB_H
#define QMLMONGODB_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class QMLMongoDB : public QObject
{
    Q_OBJECT
public:
    explicit QMLMongoDB(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QMLMONGODB_H