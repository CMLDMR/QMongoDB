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

    virtual ~QMLBSON();

    QMLBSON* operator=( QMLBSON* bson);
    QMLBSON& operator=( const QMLBSON& bson);


    Q_INVOKABLE static void insertString( QMLBSON* bson , const QString& key , const QString& str );
    Q_INVOKABLE static void insertOid( QMLBSON* bson , const QString& key , const QString& oid );
    Q_INVOKABLE static void insertInt( QMLBSON* bson , const QString& key , const int& value );
    Q_INVOKABLE static void insertInt64( QMLBSON* bson , const QString& key , const int& value );
    Q_INVOKABLE static void insertDouble( QMLBSON* bson , const QString& key , const double& value );
    Q_INVOKABLE static void insertBool( QMLBSON* bson , const QString& key , const bool& value );
    Q_INVOKABLE static void insertBson( QMLBSON* bson , const QString& key , QMLBSON *value );
    Q_INVOKABLE static void insertArray( QMLBSON* bson , const QString& key , QMLArray *value );


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


    Q_INVOKABLE void removeAll();



    Q_INVOKABLE bool containsKey( const QString& key );
    Q_INVOKABLE QMLElement* getElement( const QString& key );


    Q_INVOKABLE QMLElement::Type getElementType( const QString& key );
    Q_INVOKABLE QJsonArray getKeys();

    QBSON getQBSON();

    Q_INVOKABLE void print();



signals:

public slots:


private:


};


Q_DECLARE_METATYPE(QMLBSON)

#endif // QMLBSON_H
