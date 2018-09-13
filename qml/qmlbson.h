#ifndef QMLBSON_H
#define QMLBSON_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qbson.h"
#include "qml/qmlelement.h"

class QMLBSON : public QObject , public QBSON
{
    Q_OBJECT
public:
    explicit QMLBSON(QObject *parent = nullptr);
    QMLBSON(const QMLBSON& bson);
    QMLBSON(const QBSON& bson);



    Q_INVOKABLE static QVariant newBSON();

    Q_INVOKABLE QMLBSON& add( QString key , QString value , const QMLElement::Type& type = QMLElement::B_utf8 );
    Q_INVOKABLE QMLBSON& add( QString key , qreal value , const QMLElement::Type& type = QMLElement::B_double );
    Q_INVOKABLE QMLBSON& add( QString key , bool value  );
    Q_INVOKABLE QMLBSON& add(QString key , QMLBSON *value  );




    Q_INVOKABLE QVariant getElement( const QString& key );
    Q_INVOKABLE QVariant getBson( const QString& key );


signals:

public slots:


private:


};


Q_DECLARE_METATYPE(QMLBSON)

#endif // QMLBSON_H
