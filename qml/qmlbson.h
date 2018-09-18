#ifndef QMLBSON_H
#define QMLBSON_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include "qbson.h"
#include "qmlelement.h"
#include <QJsonArray>



class QMLBSON : public QObject , public QBSON
{
    Q_OBJECT
    Q_PROPERTY(QJsonArray KeyList READ getKeys )
public:
    explicit QMLBSON(QObject *parent = nullptr);
    QMLBSON(const QMLBSON& bson);
    QMLBSON(const QBSON& bson);

    QMLBSON* operator=( QMLBSON* bson);
    QMLBSON& operator=( const QMLBSON& bson);


    ///
    /// \brief newBSON
    /// \return
    /// New instance of QMLBSON
    Q_INVOKABLE static QMLBSON* newBSON();

    Q_INVOKABLE void addString( const QString& key , const QString& value );
    Q_INVOKABLE void addOid( const QString& key , const QString& oid );
    Q_INVOKABLE void addInt( const QString& key , const int& value );
    Q_INVOKABLE void addInt64( const QString& key , const qint64& value );
    Q_INVOKABLE void addDouble( const QString& key , const double& value );
    Q_INVOKABLE void addBool( const QString& key , const bool& value );
    Q_INVOKABLE void addBson(const QString& key , QMLBSON *value );
    Q_INVOKABLE void addArray(const QString& key , QMLArray *value );


    Q_INVOKABLE QMLBSON& add( QString key , QString value , const QMLElement::Type& type = QMLElement::B_utf8 );
    Q_INVOKABLE QMLBSON& add( QString key , qreal value , const QMLElement::Type& type = QMLElement::B_double );
    Q_INVOKABLE QMLBSON& add( QString key , bool value  );
    Q_INVOKABLE QMLBSON& add( QString key , QMLBSON *value  );


    Q_INVOKABLE bool containsKey( const QString& key );
    Q_INVOKABLE QMLElement* getElement( const QString& key );


    Q_INVOKABLE QMLElement::Type getElementType( const QString& key );
    Q_INVOKABLE QJsonArray getKeys();

    QBSON getQBSON();


signals:

public slots:


private:


};


Q_DECLARE_METATYPE(QMLBSON)

#endif // QMLBSON_H
